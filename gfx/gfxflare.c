#include "../memory.h"
#include "../time.h"
#include "../soft.h"
#include "gfxflare.h"

float GFXF_f_depth = 0.0f; // TODO FIXME, ez jo így, kidebuggolni? a jaték nem állítja sehol úgy néz ki

float f_9Factor_296[18] = { -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.2f };

static const float GFX_l_FlareSample4[8] =
{
    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f
};

struct sGfxFlareData* GFX_Flare_Create(int withSubFlares)
{
    tGfxFlareData* flare;

    if (withSubFlares)
        flare = (tGfxFlareData*)MEM_p_Alloc(sizeof(tGfxFlareData));
    else
        flare = (tGfxFlareData*)MEM_p_Alloc(sizeof(tGfxFlareDataBasic));

    MATH_SetNullVector(&flare->basic.v1);
    MATH_SetNullVector(&flare->basic.v2);
    flare->basic.unkFactor = 0.5;
    flare->basic.colorFactor = 0.0;
    flare->basic.timeColorFactorDivider = 3.0;
    flare->basic.flags = 0x100000E;
    flare->basic.color1 = 0;

    if (!withSubFlares)
    {
        flare->basic.materialNum2 = 0;
        flare->basic.color2 = 0xFFFFFFFF;
        return flare;
    }

    flare->basic.flags = 0x300000E;
    flare->i24 = 0.5f;
    flare->materialNum1 = 3;
    flare->i41 = 3;
    flare->i42 = 0;
    flare->i43 = 4;
    flare->i44 = 4;
    flare->i45 = 4;
    flare->i46 = 4;
    flare->i39 = 2;
    flare->i40 = 1;
    flare->color3 = 0xFF009999;
    flare->i53 = 0xFF009999;
    flare->i48 = 0xFF006666;
    flare->i49 = 0xFF005050;
    flare->i50 = 0xFF005050;
    flare->i51 = 0xFF005050;
    flare->i52 = 0xFF008080;
    flare->i54 = 0xFF006666;
    flare->i55 = 0xFF003333;
    flare->i29 = 0.059999999f;
    flare->i30 = 0.15000001f;
    flare->i31 = 0.30000001f;
    flare->i32 = 0.07f;
    flare->i33 = 0.059999999f;
    flare->i34 = 0.11f;
    flare->i35 = 0.029999999f;
    flare->i36 = 0.059999999f;
    flare->i37 = 0.039999999f;
    flare->basic.color2 = 0xBE19999A;
    flare->i22 = 0.25f;
    flare->i23 = 0.40000001f;
    flare->i25 = 0.625f;
    flare->i26 = 0.69999999f;
    flare->i27 = 0.80000001f;
    flare->i28 = 1.0f;
    return flare;
}

static int GFX_l_FlareOccluded(tGfxFlareData* flare)
{
    flare->basic.flags |= 0x1000000u;
    flare->basic.colorFactor = 0.0;
    return 1;
}

static int GFX_l_FlareOnScreen(Vec3* screen, float left, float right, float top, float bottom)
{
    return screen->x >= (double)left && screen->x <= (double)right
        && screen->y >= (double)top && screen->y <= (double)bottom;
}

int GFX_i_Flare_TestDepth(tGfxFlareData* flare)
{
    Vec3 screen;
    Vec3 walk;
    Vec3 world;
    Vec3 moved;
    const float* dir;
    float left;
    float right;
    float top;
    float bottom;
    int clipRight;
    int clipBottom;
    int viewX;
    int viewY;
    int i;

    if ((flare->basic.flags & 0x10) == 0)
    {
        MATH_SubstractVec(&flare->basic.v3, &flare->basic.v3, &flare->basic.v1);
        moved = flare->basic.v3;
        flare->basic.v3 = flare->basic.v1;
        if (MATH_VecLenSquared(&moved) > 0.2)
            return GFX_l_FlareOccluded(flare);
    }

    MATH_VecAddVec(&world, &flare->basic.v2, &flare->basic.v1);
    SOFT_TransformAndProject(&screen, &world, 1, &gpst_DX_stDD->ct.cam);
    if (screen.z < 0.0)
        return GFX_l_FlareOccluded(flare);

    viewX = gpst_DX_stDD->ct.cam.viewPortRealPosX;
    left = (float)(viewX < 0 ? 0 : viewX);
    clipRight = viewX + (unsigned int)gpst_DX_stDD->ct.cam.width;
    if (clipRight > gpst_DX_stDD->winWidth)
        clipRight = gpst_DX_stDD->winWidth;
    right = (float)clipRight;

    viewY = gpst_DX_stDD->ct.cam.viewPortRealPosY;
    top = (float)(viewY <= 0 ? 0 : viewY);
    clipBottom = gpst_DX_stDD->winHeight;
    if (clipBottom > (int)(viewY + (unsigned int)gpst_DX_stDD->ct.cam.height))
        clipBottom = viewY + (unsigned int)gpst_DX_stDD->ct.cam.height;
    bottom = (float)clipBottom;

    if ((flare->basic.flags & 0x300) == 0)
    {
        if (!GFX_l_FlareOnScreen(&screen, left, right, top, bottom))
            return 1;

        MATH_TransformVertex(&world, gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender,
                             &flare->basic.v1);
        gpst_DX_stDD->Request(27, &screen); // does nothing
        if (world.z <= (double)GFXF_f_depth)
        {
            flare->basic.i12 = 1.0;
            return 0;
        }
        return 1;
    }

    MATH_TransformVertex(&walk, gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender, &world);
    MATH_TransformVertex(&world, gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender,
                         &flare->basic.v1);
    flare->basic.i12 = 0.0;

    if ((flare->basic.flags & 0x300) == 256)
    {
        for (i = 0; i < 4; i++)
        {
            walk.x = GFX_l_FlareSample4[2 * i] * flare->basic.i11 + walk.x;
            walk.y = GFX_l_FlareSample4[2 * i + 1] * flare->basic.i11 + walk.y;
            SOFT_Project(&screen, &walk, 1, &gpst_DX_stDD->ct);
            if (GFX_l_FlareOnScreen(&screen, left, right, top, bottom))
            {
                gpst_DX_stDD->Request(27, &screen);
                if (world.z <= (double)GFXF_f_depth)
                    flare->basic.i12 = (float)(flare->basic.i12 + 0.25);
            }
        }
    }
    else
    {
        // The cursor starts at the head of the table and reads one float
        // behind it, so the first of the nine samples takes its x from the
        // float in front of f_9Factor_296 and never reads the last entry.
        // Left as it stands; see docs/cleanup/gfx.md.
        dir = f_9Factor_296;
        do
        {
            walk.x = dir[-1] * flare->basic.i11 + walk.x;
            walk.y = dir[0] * flare->basic.i11 + walk.y;
            SOFT_Project(&screen, &walk, 1, &gpst_DX_stDD->ct);
            if (GFX_l_FlareOnScreen(&screen, left, right, top, bottom))
            {
                gpst_DX_stDD->Request(27, &screen);
                if (world.z <= (double)GFXF_f_depth)
                    flare->basic.i12 = (float)(flare->basic.i12 + 0.1111);
            }
            dir += 2;
        } while (dir <= &f_9Factor_296[17]);
    }

    if (flare->basic.i12 != 0.0)
        return 0;
    return 1;
}

void GFX_Flare_Test(tGfxFlareData* flare)
{
    if ((flare->basic.flags & 8) == 0 || !GFX_gpst_Current->aiGameObject->distance)
        return;

    flare->basic.flags &= ~0x8000000u;
    if (GFX_i_Flare_TestDepth(flare) == 0)
        flare->basic.flags &= ~0x8000000u;
    else
        flare->basic.flags |= 0x8000000;
}

static void GFX_l_FlareQuad(Vec3* verts, Vec3* centre, Vec3* diag1, Vec3* diag2)
{
    MATH_SubstractVec(&verts[0], centre, diag2);
    MATH_VecAddVec(&verts[1], centre, diag1);
    MATH_VecAddVec(&verts[2], centre, diag2);
    MATH_SubstractVec(&verts[3], centre, diag1);
}

static int GFX_l_FlareColor(tGfxFlareData* flare, int from, int to)
{
    if (flare->basic.colorFactor == 1.0f || (flare->basic.flags & 2) == 0)
        return to;
    return LIGHT_ul_Interpol2Colors(from, to, flare->basic.colorFactor);
}

void GFX_Flare_Render(struct sGfxFlareData* flare)
{
    const float* subSize;
    const float* subAlong;
    const int* subColor;
    const int* subMaterial;
    Vec3* verts;
    Vec3 world;
    Vec3 toCamera;
    Vec3 pos;
    Vec3 right;
    Vec3 up;
    Vec3 diag1;
    Vec3 diag2;
    Vec3 scaled1;
    Vec3 scaled2;
    Vec3 axisPoint;
    Vec3 step;
    Vec3 centre;
    double size;
    double invDepth;
    int occluded;
    int color;
    int flags;
    int savedMask;
    int i;

    if (!GFX_gpst_Current->aiGameObject->distance)
    {
        flare->basic.flags |= 0x1000000u;
        flare->basic.colorFactor = 0.0;
        return;
    }

    if ((flare->basic.flags & 8) == 0)
    {
        flare->basic.flags &= ~0x1000000u;
        flare->basic.colorFactor = 1.0;
    }
    else
    {
        if ((gpst_DX_stDD->presentationFlags & 0x20000000) != 0)
            occluded = flare->basic.flags & 0x8000000;
        else
            occluded = GFX_i_Flare_TestDepth(flare);

        if (occluded)
        {
            if ((flare->basic.flags & 0x1000000) != 0)
            {
                flare->basic.colorFactor = flare->basic.colorFactor
                                         - TIM_gf_dt / flare->basic.timeColorFactorDivider;
            }
            else
            {
                flare->basic.flags |= 0x1000000;
                if (flare->basic.timeColorFactorDivider == 0.0)
                    flare->basic.colorFactor = 0.0;
                else
                    flare->basic.colorFactor = 1.0;
            }
            if (flare->basic.colorFactor <= 0.0)
            {
                if ((flare->basic.flags & 0x4000000) != 0)
                    GFX_Del(GFX_ppst_CurrentList, GFX_gpst_Current->id);
                return;
            }
        }
        else if ((flare->basic.flags & 0x1000000) != 0)
        {
            flare->basic.flags &= ~0x1000000u;
            flare->basic.colorFactor = 1.0;
        }
    }

    flags = flare->basic.flags;
    size = f_9Factor_296[17] /* 0.2 */ * flare->basic.unkFactor;
    if ((flags & 1) != 0)
        size = size * flare->basic.colorFactor;

    MATH_VecAddVec(&world, &flare->basic.v2, &flare->basic.v1);
    MATH_SubstractVec(&toCamera, &world, &gpst_DX_stDD->ct.cam.objectMtx.Pos);
    invDepth = 1.0 / MATH_DotProduct(&gpst_DX_stDD->ct.cam.objectMtx.v3, &toCamera);
    MATH_AddScaleVector(&pos, &gpst_DX_stDD->ct.cam.objectMtx.Pos, &toCamera,
                        (float)invDepth);
    if ((flags & 4) != 0)
        size = size * (float)invDepth;

    if ((flags & 0x2000000) != 0)
    {
        pos = world;
        MATH_SubstractVec(&toCamera, &pos, &gpst_DX_stDD->ct.cam.objectMtx.Pos);
        size = size * MATH_DotProduct(&gpst_DX_stDD->ct.cam.objectMtx.v3, &toCamera);
    }

    MATH_VecMultScalar(&right, &gpst_DX_stDD->ct.cam.objectMtx.v1, (float)size);
    MATH_VecMultScalar(&up, &gpst_DX_stDD->ct.cam.objectMtx.v2, (float)size);
    MATH_SubstractVec(&diag1, &right, &up);
    MATH_VecAddVec(&diag2, &right, &up);

    GFX_NeedGeom(4u, 4u, 2u, 1);
    GFX_SetQuadUVs();
    GFX_SetQuadIndices();
    verts = GFX_gpst_Geo->vertices;

    // one mask window around every render this call makes, which is nine
    // of them for a sub-flare chain
    savedMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask & ~0x9000900u;

    if ((flags & 0x2000000) == 0)
    {
        GFX_l_FlareQuad(verts, &pos, &diag1, &diag2);
        color = GFX_l_FlareColor(flare, flare->basic.color1, flare->basic.color2);
        GFX_gpst_Geo->vertexColors[1] = color;
        GFX_gpst_Geo->vertexColors[2] = color;
        GFX_gpst_Geo->vertexColors[3] = color;
        GFX_gpst_Geo->vertexColors[4] = color;
        GFX_gpst_Geo->triangleChunks->materialNum = flare->basic.materialNum2;
        p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists + 1;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask;
        return;
    }

    // The nine sub-flares are four runs of nine fields each, read in step:
    // the sizes at i29, the colours at color3, the materials at
    // materialNum1, and the distance along the line at basic.color2 -- so
    // the first two of those nine are the floats -0.15 and 0.0 stored in
    // the colour fields, and the rest are i22 to i28.
    subSize = &flare->i29;
    subAlong = (const float*)&flare->basic.color2;
    subColor = &flare->color3;
    subMaterial = &flare->materialNum1;

    MATH_VecAddVec(&axisPoint, &gpst_DX_stDD->ct.cam.objectMtx.v3,
                   &gpst_DX_stDD->ct.cam.objectMtx.Pos);
    MATH_SubstractVec(&step, &axisPoint, &pos);
    MATH_VecMultScalarWithoutBuffer(&step, 2.0f);

    for (i = 0; i < 9; i++)
    {
        MATH_VecMultScalar(&scaled1, &diag1, subSize[i]);
        MATH_VecMultScalar(&scaled2, &diag2, subSize[i]);
        MATH_AddScaleVector(&centre, &pos, &step, subAlong[i]);
        GFX_l_FlareQuad(verts, &centre, &scaled1, &scaled2);

        color = GFX_l_FlareColor(flare, 0, subColor[i]);
        GFX_gpst_Geo->vertexColors[1] = color;
        GFX_gpst_Geo->vertexColors[2] = color;
        GFX_gpst_Geo->vertexColors[3] = color;
        GFX_gpst_Geo->vertexColors[4] = color;
        GFX_gpst_Geo->triangleChunks->materialNum = subMaterial[i];
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    }

    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask;
}
