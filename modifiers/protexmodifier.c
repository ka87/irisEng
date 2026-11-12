#include "../math.h"
#include "../gameobject.h"
#include "../modifiers.h"
#include "../memory.h"
#include "../loading.h"
#include "../materialanddraw.h"
#include "../textures.h"
#include "protexmodifier.h"

/* The packed colour ramp PROTEX_InitModifier builds and the water
   surface reads back. It used to be written into f_RandomTable, which
   is the sprite generator's noise table: the entries here are colours
   reinterpreted as floats, so sharing the array left sprite noise
   multipliers of up to 1e38 and put the vertices they displace far
   off screen. The sprite table is also filled only once, behind a
   guard, while this one is rebuilt on every protex init, so the
   sprites never got their own values back. */
static float PROTEX_gaf_ColorRamp[256];

void PROTEX_Disturb(struct sGameObjectMain* gao, struct sGEOGRO* geogro)
{
    tProtexMdfStruct* protex;
    Mtx4x4* objMtx;
    Mtx4x4 inv;
    Vec3 origin;
    Vec3 axisU;
    Vec3 axisV;
    Vec3 normal;
    Vec3* vert;
    double along;
    double sumU;
    double sumV;
    double invCount;
    long double len;
    float u;
    float v;
    float scale;
    unsigned int hits;
    unsigned int centreU;
    unsigned int centreV;
    int dim1;
    int texU;
    int texV;
    int size;
    int half;
    int du;
    int dv;
    int i;

    if (!gao || (gao->flags & 5) == 0)
        return;

    for (protex = p_FirstActiveProtex; protex; protex = protex->prevProtex)
    {
        if (protex->gao == gao || !protex->unkPtrFreedInUnapply)
            continue;

        objMtx = gao->matrices;
        if ((objMtx->mtxflag & 8) != 0)
            continue;

        MATH_InvertMatrix(&inv, objMtx);
        MATH_TransformVertex(&origin, &inv, &protex->v1);
        inv.Pos.x = 0.0;
        inv.Pos.y = 0.0;
        inv.Pos.z = 0.0;
        MATH_TransformVertex(&axisU, &inv, &protex->v2);
        MATH_TransformVertex(&axisV, &inv, &protex->v3);

        MATH_CrossProduct(&normal, &axisV, &axisU);
        MATH_NormalizeEqualVector(&normal);

        len = sqrt(axisU.y * axisU.y + axisU.z * axisU.z + axisU.x * axisU.x);
        scale = (float)(1.0 / len * (1.0 / len));
        MATH_VecMultScalarWithoutBuffer(&axisU, scale);

        len = sqrt(axisV.z * axisV.z + axisV.y * axisV.y + axisV.x * axisV.x);
        scale = (float)(1.0 / len * (1.0 / len));
        MATH_VecMultScalarWithoutBuffer(&axisV, scale);

        hits = 0;
        sumU = 0.0;
        sumV = 0.0;
        vert = gpst_DX_stDD->verticesCurrent;
        for (i = 0; i < geogro->vertexNum; i++)
        {
            Vec3 d;

            d.x = vert[i].x - origin.x;
            d.y = vert[i].y - origin.y;
            d.z = vert[i].z - origin.z;
            along = normal.z * d.z + d.y * normal.y + d.x * normal.x;
            if (along * along >= 0.02)
                continue;

            u = (float)(axisU.z * d.z + d.y * axisU.y + d.x * axisU.x);
            v = (float)(d.z * axisV.z + d.y * axisV.y + d.x * axisV.x);
            if (u > 0.0 && v > 0.0 && u < 1.0 && v < 1.0)
            {
                ++hits;
                sumU = sumU + u;
                sumV = sumV + v;
            }
        }

        if (!hits)
            continue;

        invCount = 1.0 / (double)hits;
        centreU = (unsigned int)(invCount * sumU * (double)(unsigned int)protex->unkProbablyDim1 + 1048576.0);
        centreV = (unsigned int)(invCount * sumV * (double)(unsigned int)protex->unkProbablyDim2 + 1048576.0);

        for (size = 3; size < 11; size += 2)
        {
            half = (size - 1) / 2;
            for (du = 0; du < size; du++)
            {
                texU = (protex->unkProbablyDim1 - 1)
                     & (protex->unkProbablyDim2 + centreU + du - half);
                for (dv = 0; dv < size; dv++)
                {
                    dim1 = protex->unkProbablyDim1;
                    texV = (dim1 - 1) & (protex->unkProbablyDim2 + centreV + dv - half);
                    protex->unkPtrFreedInUnapply[texU + texV * dim1] += 512;
                    protex->unkPtr2[texU + texV * dim1] -= 512;
                }
            }
        }
    }
}

void PROTEX_Modifier_Reinit(struct sModifier* mdf)
{
    tProtexMdfStruct* protex;

    protex = (tProtexMdfStruct*)mdf->mdfPrivateDataPtr;
    if ((protex->flags & 0x40) == 0)
        return;

    if (protex->unkPtrFreedInUnapply)
        memset(protex->unkPtrFreedInUnapply, 0,
               2 * protex->unkProbablyDim1 * protex->unkProbablyDim2);
    if (protex->unkPtr2)
        memset(protex->unkPtr2, 0,
               2 * protex->unkProbablyDim1 * protex->unkProbablyDim2);
}

void PROTEX_Modifier_Destroy(struct sModifier* mdf)
{
    tProtexMdfStruct* protex;
    tProtexMdfStruct** link;

    protex = (tProtexMdfStruct*)mdf->mdfPrivateDataPtr;
    if ((protex->flags & 0x20) != 0)
    {
        link = &p_FirstActiveProtex;
        while (*link)
        {
            if (*link == protex)
                *link = (*link)->prevProtex;
            else
                link = &(*link)->prevProtex;
        }
    }

    if (protex->ptr3)
    {
        MEM_Free(protex->ptr3);
        protex->ptr3 = 0;
    }

    if (protex->unkPtrFreedInUnapply)
    {
        if (protex->unkPtrFreedInUnapply <= protex->unkPtr2)
            MEM_FreeAlign(protex->unkPtrFreedInUnapply);
        else
            MEM_FreeAlign(protex->unkPtr2);
        protex->unkPtrFreedInUnapply = 0;
        protex->unkPtr2 = 0;
    }

    if (protex->i1)
    {
        MEM_Free(protex->ptrCharArray);
        protex->ptrCharArray = 0;
        protex->i1 = 0;
        MEM_Free(protex->ptrCharArray2);
        protex->ptrCharArray2 = 0;
        protex->i2 = 0;
    }

    MEM_Free(protex);
}

void PROTEX_SetStUstVstP(tProtexMdfStruct* protex, Vec3* axisU, Vec3* axisV, Vec3* origin)
{
    protex->v1 = *origin;
    protex->v2 = *axisU;
    protex->v3 = *axisV;
}

void PROTEX_Apply2DMatrix(tProtexMdfStruct* protex, tMultiTextureElemStruct* elem)
{
    tMatDecompressedStruct uv;
    Vec3 axisU;
    Vec3 axisV;
    long double len;
    long double invU;
    long double invV;
    long double scaleU;

    if (MAT_VUISIdentity(elem))
        return;

    axisU = protex->v2;
    axisV = protex->v3;
    MAT_UVDecompress(elem, &uv);

    len = sqrt(uv.f3 * uv.f3 + uv.f1 * uv.f1);
    invU = 1.0 / len * (1.0 / len);
    scaleU = uv.f1 * invU;
    uv.f3 = (float)(uv.f3 * invU);

    len = sqrt(uv.f4 * uv.f4 + uv.f2 * uv.f2);
    invV = 1.0 / len * (1.0 / len);
    uv.f2 = (float)(uv.f2 * invV);
    uv.f4 = (float)(uv.f4 * invV);

    MATH_VecMultScalar(&protex->v2, &axisU, (float)scaleU);
    MATH_AddScaleVector(&protex->v2, &protex->v2, &axisV, uv.f3);
    MATH_VecMultScalar(&protex->v3, &axisU, uv.f2);
    MATH_AddScaleVector(&protex->v3, &protex->v3, &axisV, uv.f4);
    MATH_AddScaleVector(&protex->v1, &protex->v1, &protex->v2, -uv.speedU);
    MATH_AddScaleVector(&protex->v1, &protex->v1, &protex->v3, -uv.speedV);
}

void PROTEX_InitModifier(tProtexMdfStruct* protex)
{
    unsigned int i;
    long double wave;
    uint32_t level;
    uint32_t packed;

    i = 256;
    do
    {
        wave = fabs(sin((double)--i * 0.049087387));
        if (i <= 128)
            level = 0;
        else
            level = (uint32_t)(wave * wave * (wave * wave) * 128.0);
        packed = level | (level << 8) | (level << 16)
               | ((uint32_t)(255.0 * wave) << 24);
        PROTEX_gaf_ColorRamp[i] = (*((float*)(&packed)));
    } while (i);

    memset(protex, 0, sizeof(tProtexMdfStruct));
}

int PROTEX_ul_Modifier_Load(tModifier* mdf, char* data)
{
    char* start;
    tProtexMdfStruct* protex;
    int hasArrays;

    start = data;
    protex = (tProtexMdfStruct*)mdf->mdfPrivateDataPtr;
    LOA_ReadLong(&data, 0, 1);
    hasArrays = LOA_ReadLong(&data, 0, 1);
    PROTEX_InitModifier(protex);
    protex->gao = mdf->gao;
    if (hasArrays)
    {
        protex->i1 = LOA_ReadLong(&data, 0, 1);
        protex->ptrCharArray = (char*)MEM_p_Alloc(protex->i1);
        LOA_ReadCharArray(&data, protex->ptrCharArray, protex->i1, 1);
        protex->i2 = LOA_ReadLong(&data, 0, 1);
        protex->ptrCharArray2 = (char*)MEM_p_Alloc(protex->i2);
        LOA_ReadCharArray(&data, protex->ptrCharArray2, protex->i2, 1);
    }
    return data - start;
}

void PROTEX_Modifier_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tProtexMdfStruct* protex;

    protex = (tProtexMdfStruct*)MEM_p_Alloc(sizeof(tProtexMdfStruct));
    mdf->mdfPrivateDataPtr = protex;
    if (initData)
    {
        memcpy(protex, initData, sizeof(tProtexMdfStruct));
        protex->gao = gao;
    }
    else
    {
        PROTEX_InitModifier(protex);
    }
}

static void PROTEX_l_WaterUpdate(tProtexMdfStruct* protex)
{
    int16_t* cur;
    int16_t* prev;
    int16_t* older;
    const uint16_t* heights;
    uint8_t* bump;
    int width;
    int height;
    int cells;
    int lastRow;
    int row;
    int col;
    int h;
    int i;

    width = protex->unkProbablyDim1;
    height = protex->unkProbablyDim2;
    cells = width * height;
    lastRow = 2 * width * (height - 1);

    memset(protex->unkPtrFreedInUnapply, 0, 2 * width);
    memset(protex->unkPtr2, 0, 2 * width);
    memset((char*)protex->unkPtrFreedInUnapply + lastRow, 0, 2 * width);
    memset((char*)protex->unkPtr2 + lastRow, 0, 2 * width);

    cur = &protex->unkPtrFreedInUnapply[width + 1];
    prev = &protex->unkPtr2[width + 1];
    for (row = 1; row < height - 1; row++)
    {
        for (col = 1; col < width - 1; col++)
        {
            h = ((prev[-1] + prev[0] + prev[1] + prev[-1] + prev[1]
                  + prev[width - 1] + prev[width] + prev[width + 1]) >> 2)
                - *cur;
            h = h - (h >> 8);
            if (h > 0x2000)
                h = 0x2000;
            else if (h < -0x2000)
                h = -0x2000;
            *cur++ = (int16_t)h;
            ++prev;
        }
        cur[0] = 0;
        cur[1] = 0;
        prev[0] = 0;
        prev[1] = 0;
        cur += 2;
        prev += 2;
    }

    older = protex->unkPtr2;
    protex->unkPtr2 = protex->unkPtrFreedInUnapply;
    protex->unkPtrFreedInUnapply = older;

    heights = (const uint16_t*)older;
    bump = protex->unkNumProtexWater;
    for (i = 0; i < cells; i++)
        bump[i] = (uint8_t)(((heights[i] - heights[i + 1]) >> 6) + 0x80);

    for (i = 0; i < cells; i++)
        protex->ptr3[i] = PROTEX_gaf_ColorRamp[bump[i]];

    gpst_DX_stDD->Request(37, (void*)protex->ptr3); // does nothing
}

void PROTEX_UpdateWaterEffect(tProtexMdfStruct* protex)
{
    uint8_t* buffer;
    int texIdx;
    int cells;

    if ((protex->flags & 0x40) == 0)
        return;

    if (!protex->unkPtrFreedInUnapply)
    {
        texIdx = protex->texElemsI22field;
        protex->unkProbablyDim1 = TEX_gst_GlobalList.texElems[texIdx].normalData.width;
        protex->unkProbablyDim2 = TEX_gst_GlobalList.texElems[texIdx].normalData.height;
        cells = protex->unkProbablyDim1 * protex->unkProbablyDim2;

        buffer = (uint8_t*)MEM_p_AllocAligned(5 * cells, 64);
        protex->unkPtrFreedInUnapply = (int16_t*)buffer;
        if (!buffer)
            return;

        protex->unkPtr2 = (int16_t*)&buffer[2 * cells];
        protex->unkNumProtexWater = &buffer[4 * cells];
        memset(buffer, 0, 5 * cells);
        protex->ptr3 = (float*)MEM_p_Alloc(4 * cells);
    }

    PROTEX_l_WaterUpdate(protex);
}

void PROTEX_BeforeDraw(void)
{
    tProtexMdfStruct** link;

    QueryPerformanceCounter(&PerfCounterStruct[20]);
    link = &p_FirstActiveProtex;
    while (*link)
    {
        if (((*link)->flags & 8) != 0)
        {
            PROTEX_UpdateWaterEffect(*link);
            (*link)->flags &= ~8u;
            (*link)->flags |= 0x10u;
            link = &(*link)->prevProtex;
        }
        else
        {
            (*link)->flags &= ~0x20u;
            *link = (*link)->prevProtex;
        }
    }
    PRO_QPC_RecordTimeDecRef(20);
}

static void PROTEX_l_Basis(Mtx4x4* out, tGameObjectMain* gao, Mtx4x4* uvMtx, int haveUvMatrix)
{
    Mtx4x4* abs;

    abs = OBJ_pst_GetAbsoluteMatrix(gao);
    if (haveUvMatrix)
        MATH_MulMatrixMatrix(out, abs, uvMtx);
    else
        MATH_MakeOGLMatrix(out, abs);
}

void PROTEX_ComputeStUstVstP(tProtexMdfStruct* protex, tMultiTextureElemStruct* elem,
                             tGameObjectMain* gao)
{
    Mtx4x4 basis;
    Mtx4x4 uvMtx;
    Vec3 axisU;
    Vec3 axisV;
    uint16_t param;
    int mode;
    int haveUvMatrix;

    if ((elem->subTextureFlags & 0xF00000) != 0x600000)
        return;

    param = elem->phongOrPlanarParam;
    mode = (param >> 4) & 3;
    haveUvMatrix = ((param & 0xC0) != 0);
    if (haveUvMatrix)
        MAT_UV_COMPUTE_GetMAtrix(gpst_DX_stDD, gao, param, &uvMtx);

    if (mode == 3)
    {
        if (haveUvMatrix)
        {
            axisU.x = (float)(uvMtx.v3.x * 0.0 - uvMtx.v2.x * 0.70710677 + uvMtx.v1.x * 0.70710677);
            axisU.y = (float)(uvMtx.v3.y * 0.0 - uvMtx.v2.y * 0.70710677 + uvMtx.v1.y * 0.70710677);
            axisU.z = (float)(uvMtx.v3.z * 0.0 - uvMtx.v2.z * 0.70710677 + uvMtx.v1.z * 0.70710677);
            axisV.x = (float)(uvMtx.v3.x * 0.57735026 - (uvMtx.v2.x + uvMtx.v1.x) * 0.57735026);
            axisV.y = (float)(uvMtx.v3.y * 0.57735026 - (uvMtx.v2.y + uvMtx.v1.y) * 0.57735026);
            axisV.z = (float)(uvMtx.v3.z * 0.57735026 - (uvMtx.v2.z + uvMtx.v1.z) * 0.57735026);
            PROTEX_SetStUstVstP(protex, &axisU, &axisV, &OBJ_pst_GetAbsoluteMatrix(gao)->Pos);
        }
        PROTEX_Apply2DMatrix(protex, elem);
        return;
    }

    if (mode == 0)
    {
        PROTEX_l_Basis(&basis, gao, &uvMtx, haveUvMatrix);
        PROTEX_SetStUstVstP(protex, &basis.v2, &basis.v3,
                            &OBJ_pst_GetAbsoluteMatrix(gao)->Pos);
    }

    PROTEX_l_Basis(&basis, gao, &uvMtx, haveUvMatrix);
    if (mode == 2)
        PROTEX_SetStUstVstP(protex, &basis.v1, &basis.v2,
                            &OBJ_pst_GetAbsoluteMatrix(gao)->Pos);
    else
        PROTEX_SetStUstVstP(protex, &basis.v1, &basis.v3,
                            &OBJ_pst_GetAbsoluteMatrix(gao)->Pos);
    PROTEX_Apply2DMatrix(protex, elem);
}

void PROTEX_Modifier_Unapply(tModifier* mdf, void* unk1)
{
    tProtexMdfStruct* protex;
    tGameObjectMain* gao;
    tGameObjectVisualObject* visual;
    tGEOMAT* mat;
    tMultiTextureElemStruct* elem;
    tMultiTextureElemStruct* next;
    int remaining;
    int matIdx;
    int texIdx;

    protex = (tProtexMdfStruct*)mdf->mdfPrivateDataPtr;
    if ((protex->flags & 0x20) == 0)
    {
        protex->prevProtex = p_FirstActiveProtex;
        protex->flags |= 0x20;
        p_FirstActiveProtex = protex;
    }
    protex->flags |= 8u;

    gao = mdf->gao;
    if (!gao || !gao->basic)
        return;

    visual = (tGameObjectVisualObject*)gao->basic->actorObjectOrVisualObject;
    if (!visual)
        return;

    mat = (tGEOMAT*)visual->GROMAT;
    if (!mat)
        return;

    if (mat->intf->id == 4)
    {
        matIdx = mat->multiMaterialNumIfType4ThenAmbiantColor - 1;
        if (protex->maxMaterialID <= matIdx)
            matIdx = protex->maxMaterialID;
        mat = mat->multiMaterialPtrIfType4ThenDiffuseColor[matIdx];
    }
    if (mat->intf->id != 5)
        return;

    elem = (tMultiTextureElemStruct*)mat->idxOrPtr;
    if (!elem)
        return;

    remaining = protex->unkMatCnt;
    for (next = elem->nextTextureKey; next && remaining; remaining--)
    {
        elem = next;
        next = next->nextTextureKey;
    }

    PROTEX_ComputeStUstVstP(protex, elem, gao);

    texIdx = elem->texElemIdx;
    if (texIdx == -1)
        return;

    if ((TEX_gst_GlobalList.texElems[texIdx].texElemFlag & 0x40) != 0)
    {
        protex->texElemIdx = texIdx;
        texIdx = TEX_gst_GlobalList.texElems[texIdx].proceduralData.paletteID;
    }
    if (texIdx == -1)
        return;

    if ((protex->flags & 0x40) != 0
        && texIdx != (protex->texElemsI22field & 0xffff)
        && protex->unkPtrFreedInUnapply)
    {
        MEM_FreeAlign(protex->unkPtrFreedInUnapply);
        protex->unkPtrFreedInUnapply = 0;
    }
    protex->texElemsI22field = texIdx;
    if (texIdx < TEX_gst_GlobalList.actTexElemNum)
        protex->flags |= 0x40u;
}

void PROTEX_Modifier_Apply(tModifier* mdf, void* unk1)
{
}
