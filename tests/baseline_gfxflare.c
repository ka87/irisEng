#include "../memory.h"
#include "../time.h"
#include "../soft.h"
#include "gfxflare.h"

float GFXF_f_depth = 0.0f; // TODO FIXME, ez jo így, kidebuggolni? a jaték nem állítja sehol úgy néz ki

float f_9Factor_296[18] = { -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.2f };

struct sGfxFlareData* GFX_Flare_Create(int a1)
{
    tGfxFlareData* result;

    if (a1)
        result = (tGfxFlareData*)MEM_p_Alloc(sizeof(tGfxFlareData));
    else
        result = (tGfxFlareData*)MEM_p_Alloc(sizeof(tGfxFlareDataBasic));
    result->basic.v1.x = 0.0;
    result->basic.v1.y = 0.0;
    result->basic.v1.z = 0.0;
    result->basic.v2.x = 0.0;
    result->basic.v2.y = 0.0;
    result->basic.v2.z = 0.0;
    result->basic.unkFactor = 0.5;
    result->basic.colorFactor = 0.0;
    result->basic.timeColorFactorDivider = 3.0;
    result->basic.flags = 0x100000E;
    result->basic.color1 = 0;
    if (a1)
    {
        result->i29 = 0.059999999f;
        result->i33 = 0.059999999f;
        result->i36 = 0.059999999f;
        result->materialNum1 = 3;
        result->i41 = 3;
        result->i42 = 0;
        result->i24 = 0.5f;
        result->i43 = 4;
        result->i44 = 4;
        result->i45 = 4;
        result->i46 = 4;
        result->color3 = 0xFF009999;
        result->i53 = 0xFF009999;
        result->basic.flags = 0x300000E;
        result->i30 = 0.15000001f;
        result->i31 = 0.30000001f;
        result->i32 = 0.07f;
        result->i34 = 0.11f;
        result->i35 = 0.029999999f;
        result->i37 = 0.039999999f;
        result->basic.color2 = 0xBE19999A;
        result->i22 = 0.25f;
        result->i23 = 0.40000001f;
        result->i25 = 0.625f;
        result->i26 = 0.69999999f;
        result->i27 = 0.80000001f;
        result->i28 = 1.0f;
        result->i39 = 2;
        result->i40 = 1;
        result->i48 = 0xFF006666;
        result->i49 = 0xFF005050;
        result->i50 = 0xFF005050;
        result->i51 = 0xFF005050;
        result->i52 = 0xFF008080;
        result->i54 = 0xFF006666;
        result->i55 = 0xFF003333;
    }
    else
    {
        result->basic.materialNum2 = 0;
        result->basic.color2 = 0xFFFFFFFF;
    }
    return result;
}

int GFX_i_Flare_TestDepth(tGfxFlareData* a1)
{
    BOOL v2;
    int v7;
    int v8;
    int v9;
    int v10;
    float* v11;
    float v12;
    float v13;
    int v14;
    int v15;
    float v16;
    Vec3 a1a; 
    Vec3 v18;
    Vec3 a2; 
    float v20[8];
    float v21;
    float fx, fy, fz;

    v2 = (a1->basic.flags & 0x10) == 0;
    v20[0] = -0.5f;
    v20[1] = -0.5f;
    v20[2] = 1.0f;
    v20[3] = 0.0f;
    v20[4] = 0.0f;
    v20[5] = 1.0f;
    v20[6] = -1.0f;
    v20[7] = 0.0f;
    if (v2)
    {
        a1->basic.v3.x = a1->basic.v3.x - a1->basic.v1.x;
        a1->basic.v3.y = a1->basic.v3.y - a1->basic.v1.y;
        a1->basic.v3.z = a1->basic.v3.z - a1->basic.v1.z;
        fx = a1->basic.v3.x;
        fy = a1->basic.v3.y;
        fz = a1->basic.v3.z;
        a1->basic.v3 = a1->basic.v1;
        if ((fx * fx + fy * fy + fz * fz) > 0.2)
            goto LABEL_4;
    }
    a2.x = a1->basic.v2.x + a1->basic.v1.x;
    a2.y = a1->basic.v2.y + a1->basic.v1.y;
    a2.z = a1->basic.v2.z + a1->basic.v1.z;
    SOFT_TransformAndProject(&a1a, &a2, 1, &gpst_DX_stDD->ct.cam);
    if (a1a.z < 0.0)
    {
    LABEL_4:
        a1->basic.flags |= 0x1000000u;
        a1->basic.colorFactor = 0.0;
        return 1;
    }
    v7 = gpst_DX_stDD->ct.cam.viewPortRealPosX;
    v21 = (float)(v7 < 0 ? 0 : v7);
    v14 = v7 + (unsigned int)gpst_DX_stDD->ct.cam.width;
    if (v14 > gpst_DX_stDD->winWidth)
        v14 = gpst_DX_stDD->winWidth;
    v8 = gpst_DX_stDD->ct.cam.viewPortRealPosY;
    v12 = (float)v14;
    v13 = (float)(v8 <= 0 ? 0 : v8);
    v15 = gpst_DX_stDD->winHeight;
    if (v15 > (int)(v8 + (unsigned int)gpst_DX_stDD->ct.cam.height))
        v15 = v8 + (unsigned int)gpst_DX_stDD->ct.cam.height;
    v16 = (float)v15;
    if ((a1->basic.flags & 0x300) != 0)
    {
        MATH_TransformVertex(&v18, gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender, &a2);
        MATH_TransformVertex(&a2, gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender, &a1->basic.v1);
        v9 = a1->basic.flags & 0x300;
        a1->basic.i12 = 0.0;
        if (v9 == 256)
        {
            v10 = 0;
            do
            {
                v18.x = v20[2 * v10] * a1->basic.i11 + v18.x;
                v18.y = v20[2 * v10 + 1] * a1->basic.i11 + v18.y;
                SOFT_Project(&a1a, &v18, 1, &gpst_DX_stDD->ct);
                if (a1a.x >= (double)v21 && a1a.x <= (double)v12 && a1a.y >= (double)v13 && a1a.y <= (double)v16)
                {
                    gpst_DX_stDD->Request(27, &a1a);
                    if (a2.z <= (double)GFXF_f_depth)
                        a1->basic.i12 = (float)(a1->basic.i12 + 0.25);
                }
                ++v10;
            } while (v10 < 4);
        }
        else
        {
            v11 = f_9Factor_296;
            do
            {
                v18.x = *(v11 - 1) * a1->basic.i11 + v18.x;
                v18.y = *v11 * a1->basic.i11 + v18.y;
                SOFT_Project(&a1a, &v18, 1, &gpst_DX_stDD->ct);
                if (a1a.x >= (double)v21 && a1a.x <= (double)v12 && a1a.y >= (double)v13 && a1a.y <= (double)v16)
                {
                    gpst_DX_stDD->Request(27, &a1a);
                    if (a2.z <= (double)GFXF_f_depth)
                        a1->basic.i12 = (float)(a1->basic.i12 + 0.1111);
                }
                v11 += 2;
            } while ((int)v11 <= (int)&f_9Factor_296[17]);
        }
        if (a1->basic.i12 != 0.0)
            return 0;
    }
    else if (a1a.x >= (double)v21 && a1a.x <= (double)v12 && a1a.y >= (double)v13 && a1a.y <= (double)v16)
    {
        MATH_TransformVertex(&a2, gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender, &a1->basic.v1);
        gpst_DX_stDD->Request(27, &a1a);// does nothing
        if (a2.z <= (double)GFXF_f_depth)
        {
            a1->basic.i12 = 1.0;
            return 0;
        }
    }
    return 1;
}

void GFX_Flare_Test(tGfxFlareData* a1)
{
    int v1;
    BOOL v2;
    int v3;

    v1 = a1->basic.flags;
    if ((v1 & 8) != 0 && GFX_gpst_Current->aiGameObject->distance)
    {
        a1->basic.flags = v1 & ~0x8000000u;
        v2 = GFX_i_Flare_TestDepth(a1) == 0;
        v3 = a1->basic.flags;
        if (v2)
            a1->basic.flags = v3 & ~0x8000000u;
        else
            a1->basic.flags = v3 | 0x8000000;
    }
}

void GFX_Flare_Render(struct sGfxFlareData* a1)
{
    int v2;
    tGdi_DisplayData* v3;
    int v4;
    BOOL v5;
    double v6;
    int v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    double v13;
    tGfxFlareData* v14;
    int v15;
    double v16;
    double v17;
    Vec3* v18; 
    double v19;
    double v20;
    double v21;
    Vec3* v22; 
    Vec3* v23; 
    Vec3* v24; 
    int v25; 
    Vec3* v26;
    Vec3* v27;
    Vec3* v28;
    Vec3* v29;
    int v30; 
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    float v38;
    float v39;
    float v40;
    float v41;
    float v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    float v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    float v56;
    float v57;
    float v58;
    float v59;
    float v60;
    float v61;
    float v62;
    int v63; 

    if (!GFX_gpst_Current->aiGameObject->distance)
    {
        a1->basic.flags |= 0x1000000u;
        a1->basic.colorFactor = 0.0;
        return;
    }
    v2 = a1->basic.flags;
    if ((v2 & 8) == 0)
        goto LABEL_17;
    v3 = gpst_DX_stDD;
    if ((gpst_DX_stDD->presentationFlags & 0x20000000) != 0)
    {
        v4 = v2 & 0x8000000;
    }
    else
    {
        v4 = GFX_i_Flare_TestDepth(a1);
        v3 = gpst_DX_stDD;
    }
    v5 = v4 == 0;
    v2 = a1->basic.flags;
    if (!v5)
    {
        if ((v2 & 0x1000000) != 0)
        {
            a1->basic.colorFactor = a1->basic.colorFactor - TIM_gf_dt / a1->basic.timeColorFactorDivider;
        }
        else
        {
            v6 = a1->basic.timeColorFactorDivider;
            a1->basic.flags = v2 | 0x1000000;
            if (v6 == 0.0)
                a1->basic.colorFactor = 0.0;
            else
                a1->basic.colorFactor = 1.0;
        }
        if (a1->basic.colorFactor <= 0.0)
        {
            if ((a1->basic.flags & 0x4000000) != 0)
                GFX_Del(GFX_ppst_CurrentList, GFX_gpst_Current->id);
            return;
        }
        goto LABEL_18;
    }
    if ((v2 & 0x1000000) != 0)
    {
    LABEL_17:
        a1->basic.flags = v2 & ~0x1000000u;
        a1->basic.colorFactor = 1.0;
    LABEL_18:
        v3 = gpst_DX_stDD;
    }
    v7 = a1->basic.flags;
    v8 = f_9Factor_296[17] /* 0.2 */ * a1->basic.unkFactor;
    if ((v7 & 1) != 0)
        v8 = v8 * a1->basic.colorFactor;
    v36 = a1->basic.v2.z + a1->basic.v1.z;
    v31 = a1->basic.v1.x + a1->basic.v2.x - v3->ct.cam.objectMtx.Pos.x;
    v9 = a1->basic.v2.y + a1->basic.v1.y - v3->ct.cam.objectMtx.Pos.y;
    v10 = v36 - v3->ct.cam.objectMtx.Pos.z;
    v11 = 1.0 / (v3->ct.cam.objectMtx.v3.z * v10 + v3->ct.cam.objectMtx.v3.y * v9 + v31 * v3->ct.cam.objectMtx.v3.x);
    v32 = (float)(v31 * v11);
    v34 = (float)(v9 * v11);
    v33 = v32 + v3->ct.cam.objectMtx.Pos.x;
    v35 = v34 + v3->ct.cam.objectMtx.Pos.y;
    v37 = (float)(v11 * v10 + v3->ct.cam.objectMtx.Pos.z);
    if ((v7 & 4) != 0)
    {
        v62 = (float)v11;
        v8 = v8 * v62;
    }
    if ((v7 & 0x2000000) != 0)
    {
        v33 = a1->basic.v2.x + a1->basic.v1.x;
        v35 = a1->basic.v2.y + a1->basic.v1.y;
        v37 = a1->basic.v2.z + a1->basic.v1.z;
        v8 = v8
            * (v3->ct.cam.objectMtx.v3.z * (v37 - v3->ct.cam.objectMtx.Pos.z)
                + v3->ct.cam.objectMtx.v3.y * (v35 - v3->ct.cam.objectMtx.Pos.y)
                + (v33 - v3->ct.cam.objectMtx.Pos.x) * v3->ct.cam.objectMtx.v3.x);
    }
    v12 = v8 * v3->ct.cam.objectMtx.v1.x;
    v45 = (float)(v3->ct.cam.objectMtx.v1.y * v8);
    v47 = (float)(v3->ct.cam.objectMtx.v1.z * v8);
    v13 = v8 * v3->ct.cam.objectMtx.v2.x;
    v41 = (float)(v3->ct.cam.objectMtx.v2.y * v8);
    v43 = (float)(v3->ct.cam.objectMtx.v2.z * v8);
    v52 = (float)(v12 - v13);
    v53 = v45 - v41;
    v54 = v47 - v43;
    v49 = (float)(v13 + v12);
    v50 = v41 + v45;
    v51 = v43 + v47;
    GFX_NeedGeom(4u, 4u, 2u, 1);
    GFX_gpst_Geo->UVs->u = 0.0;
    GFX_gpst_Geo->UVs->v = 0.0;
    GFX_gpst_Geo->UVs[1].u = 1.0;
    GFX_gpst_Geo->UVs[1].v = 0.0;
    GFX_gpst_Geo->UVs[2].u = 1.0;
    GFX_gpst_Geo->UVs[2].v = 1.0;
    GFX_gpst_Geo->UVs[3].u = 0.0;
    GFX_gpst_Geo->UVs[3].v = 1.0;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->v1 = 0;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->v2 = 1;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->v3 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv1 = 0; // TODO FIXME, ez biztos, jó megnézni
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv2 = 1;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems->uv3 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v1 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v2 = 3;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].v3 = 0;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv1 = 2;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv2 = 3;
    GFX_gpst_Geo->triangleChunks->triangleChunkElems[1].uv3 = 0;
    v63 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v63 & ~0x9000900u;
    if ((a1->basic.flags & 0x2000000) != 0)
    {
        v14 = a1;
        v15 = 9;
        v16 = gpst_DX_stDD->ct.cam.objectMtx.v3.y + gpst_DX_stDD->ct.cam.objectMtx.Pos.y;
        v58 = gpst_DX_stDD->ct.cam.objectMtx.v3.z + gpst_DX_stDD->ct.cam.objectMtx.Pos.z;
        v55 = gpst_DX_stDD->ct.cam.objectMtx.v3.x + gpst_DX_stDD->ct.cam.objectMtx.Pos.x - v33;
        v56 = v55 + v55;
        v57 = (float)(v16 - v35 + v16 - v35);
        v59 = v58 - v37 + v58 - v37;
        do
        {
            v17 = v14->i29;
            v18 = GFX_gpst_Geo->vertices;
            v19 = v52 * v17;
            v46 = (float)(v53 * v17);
            v48 = (float)(v54 * v17);
            v20 = v49 * v17;
            v42 = (float)(v50 * v17);
            v44 = (float)(v51 * v17);
            v21 = *(float *)(&v14->basic.color2); // TODO FIXME ez miert?????
            v60 = (float)(v57 * v21);
            v61 = (float)(v59 * v21);
            v38 = (float)(v56 * v21 + v33);
            v39 = v60 + v35;
            v40 = v61 + v37;
            v18->x = (float)(v38 - v20);
            v18->y = v39 - v42;
            v18->z = v40 - v44;
            v22 = GFX_gpst_Geo->vertices + 1;
            v22->x = (float)(v38 + v19);
            v22->y = v39 + v46;
            v22->z = v40 + v48;
            v23 = GFX_gpst_Geo->vertices + 2;
            v23->x = (float)(v38 + v20);
            v23->y = v39 + v42;
            v23->z = v40 + v44;
            v24 = GFX_gpst_Geo->vertices + 3;
            v24->x = (float)(v38 - v19);
            v24->y = v39 - v46;
            v24->z = v40 - v48;
            if (a1->basic.colorFactor == 1.0f || (a1->basic.flags & 2) == 0)
                v25 = v14->color3;
            else
                v25 = LIGHT_ul_Interpol2Colors(0, v14->color3, a1->basic.colorFactor);
            GFX_gpst_Geo->vertexColors[1] = v25;
            GFX_gpst_Geo->vertexColors[2] = v25;
            GFX_gpst_Geo->vertexColors[3] = v25;
            GFX_gpst_Geo->vertexColors[4] = v25;
            GFX_gpst_Geo->triangleChunks->materialNum = v14->materialNum1;
            GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
            ++v14;
            --v15;
        } while (v15);
    }
    else
    {
        v26 = GFX_gpst_Geo->vertices;
        v26->x = v33 - v49;
        v26->y = v35 - v50;
        v26->z = v37 - v51;
        v27 = GFX_gpst_Geo->vertices + 1;
        v27->x = v52 + v33;
        v27->y = v53 + v35;
        v27->z = v54 + v37;
        v28 = GFX_gpst_Geo->vertices + 2;
        v28->x = v49 + v33;
        v28->y = v50 + v35;
        v28->z = v51 + v37;
        v29 = GFX_gpst_Geo->vertices + 3;
        v29->x = v33 - v52;
        v29->y = v35 - v53;
        v29->z = v37 - v54;
        if (a1->basic.colorFactor == 1.0f || (a1->basic.flags & 2) == 0)
            v30 = a1->basic.color2;
        else
            v30 = LIGHT_ul_Interpol2Colors(a1->basic.color1, a1->basic.color2, a1->basic.colorFactor);
        GFX_gpst_Geo->vertexColors[1] = v30;
        GFX_gpst_Geo->vertexColors[2] = v30;
        GFX_gpst_Geo->vertexColors[3] = v30;
        GFX_gpst_Geo->vertexColors[4] = v30;
        GFX_gpst_Geo->triangleChunks->materialNum = a1->basic.materialNum2;
        p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists + 1;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    }
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v63;
}