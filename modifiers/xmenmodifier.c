#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "../soft.h"
#include "../materialanddraw.h"
#include "../dbg.h"
#include "xmenmodifier.h"

int bFirst_286 = 0; // game doesn't set this explicitly
int XMEN_STACK_PTR = 0; // game doesn't set this explicitly
float xmenSmoothBuffer_flt_9684BC = 0.5f;

void XMEN_FirstInit(void)
{
    if (bFirst_286)
        return;

    bFirst_286 = 1;
    memset(&XMEN_OBJECT, 0, sizeof(XMEN_OBJECT));
    GRO_Struct_Init((tGroGeneric*)&XMEN_OBJECT, 1);
    XMEN_OBJECT.vertexNum = 16;
    XMEN_OBJECT.uvNum = 16;
    XMEN_OBJECT.triangleChunks = XMEN_ELEMENT;
    XMEN_OBJECT.vertices = XMEN_OBJECT_Points_Smoothed;
    XMEN_OBJECT.normals = XMEN_OBJECT_Points_Smoothed;
    memset(XMEN_ELEMENT, 0, sizeof(XMEN_ELEMENT));
    memset(XMEN_OBJECT_TRIS, 0, sizeof(XMEN_OBJECT_TRIS));
    XMEN_OBJECT.UVs = XMEN_UVs;
    XMEN_OBJECT.triangleChunkCount = 1;
    XMEN_ELEMENT[0].triangleChunkElems = XMEN_OBJECT_TRIS;
    XMEN_ELEMENT[0].triangleNum = 14;
    XMEN_ELEMENT[0].materialNum = 0;
}

void GAO_ModifierXMEN_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    struct sXmenMdfStruct* src = (struct sXmenMdfStruct*)initData;
    tXmenMdfStruct* xmen;

    XMEN_FirstInit();
    xmen = (tXmenMdfStruct*)MEM_p_Alloc(sizeof(tXmenMdfStruct));
    mdf->mdfPrivateDataPtr = xmen;
    mdf->modifierFlags = 0x1000000;

    if (!src)
    {
        memset(xmen, 0, sizeof(tXmenMdfStruct));
        xmen->geoKey = -1;
    }
    else
    {
        memcpy(xmen, src, sizeof(tXmenMdfStruct));
        if (xmen->num)
        {
            xmen->sizes = (tXmenSize*)MEM_p_Alloc(sizeof(tXmenSize) * xmen->num);
            memcpy(xmen->sizes, src->sizes, sizeof(tXmenSize) * xmen->num);
            xmen->ptr1 = (tXmenMdfInner2*)MEM_p_Alloc(sizeof(tXmenMdfInner2) * xmen->num);
            memcpy(xmen->ptr1, src->ptr1, sizeof(tXmenMdfInner2) * xmen->num);
            xmen->ptr2 = (int*)MEM_p_Alloc(8 * 4 * xmen->num);
            memcpy(xmen->ptr2, src->ptr2, 8 * 4 * xmen->num);
        }
        if (xmen->geomat)
            xmen->geomat->intf->AddRef((tGroGeneric*)xmen->geomat, 1);
    }

    xmen->gao = gao;
}

void GAO_ModifierXMEN_Destroy(struct sModifier* mdf)
{
    tXmenMdfStruct* xmen;
    int slot;

    xmen = (tXmenMdfStruct*)mdf->mdfPrivateDataPtr;
    if (!xmen)
        return;

    for (slot = XMEN_STACK_PTR; slot > 0; slot--)
    {
        if (p_XMEN_STACK[slot - 1] == xmen)
            p_XMEN_STACK[slot - 1] = 0;
    }

    if (xmen->geomat)
    {
        xmen->geomat->intf->AddRef((tGroGeneric*)xmen->geomat, -1);
        xmen->geomat->intf->Destroy((tGroGeneric*)xmen->geomat);
    }
    if (xmen->num)
    {
        MEM_Free(xmen->sizes);
        MEM_Free(xmen->ptr1);
        MEM_Free(xmen->ptr2);
    }
    MEM_Free(xmen);
}

void GAO_ModifierXMEN_Apply(struct sModifier* mdf, void* unk1)
{
    // The stack holds XMEN_STACK_SLOTS entries. Stop the counter at that,
    // so the walks below can never index past the array; the surplus
    // modifiers for this frame are dropped instead.
    if (XMEN_STACK_PTR >= XMEN_STACK_SLOTS)
    {
        if (XMEN_STACK_PTR == XMEN_STACK_SLOTS)
            DBG_OutputDebugString("XMENSTACK: more than %d applied in one frame, dropping the rest\n",
                                  XMEN_STACK_SLOTS);
        XMEN_STACK_PTR = XMEN_STACK_SLOTS;
        return;
    }
    p_XMEN_STACK[XMEN_STACK_PTR] = (tXmenMdfStruct*)mdf->mdfPrivateDataPtr;
    XMEN_STACK_PTR = XMEN_STACK_PTR + 1;
}

int GAO_ModifierXMEN_Load(struct sModifier* mdf, char* data)
{
    char* start;
    tXmenMdfStruct* xmen;
    tXmenSize* size;
    unsigned int version;
    int i;

    XMEN_FirstInit();
    start = data;
    xmen = (tXmenMdfStruct*)mdf->mdfPrivateDataPtr;
    LOA_ReadLong(&data, 0, 2);
    version = LOA_ReadLong(&data, 0, 1);
    xmen->flags = LOA_ReadLong(&data, 0, 1) & ~8u;
    xmen->num = LOA_ReadLong(&data, 0, 1);

    xmen->sizes = 0;
    if (xmen->num)
        xmen->sizes = (tXmenSize*)MEM_p_Alloc(sizeof(tXmenSize) * xmen->num);
    size = xmen->sizes;
    for (i = 0; i < xmen->num; i++, size++)
    {
        size->multipartDataID = LOA_ReadLong(&data, 0, 1);
        size->size = LOA_ReadFloat(&data, 0, 1);
    }

    xmen->ptr1 = 0;
    xmen->ptr2 = 0;
    if (xmen->num)
    {
        xmen->ptr1 = (tXmenMdfInner2*)MEM_p_Alloc(sizeof(tXmenMdfInner2) * xmen->num);
        memset(xmen->ptr1, 0, sizeof(tXmenMdfInner2) * xmen->num);
        xmen->ptr2 = (int*)MEM_p_Alloc(8 * 4 * xmen->num);
        memset(xmen->ptr2, 0, 8 * 4 * xmen->num);
    }

    xmen->geoKey = LOA_ReadLong(&data, 0, 1);
    xmen->geomat = 0;
    if (version)
        xmen->xmenType = LOA_ReadLong(&data, 0, 1);
    if (version > 1)
        xmen->deathTime = LOA_ReadFloat(&data, 0, 1);
    if (version > 2)
        xmen->id = LOA_ReadLong(&data, 0, 1);

    xmen->gao = mdf->gao;
    xmen->frameCounter = 0;
    xmen->num2 = 0;
    if ((xmen->flags & 8) == 0 && xmen->geoKey && xmen->geoKey != -1)
        LOA_MakeFileRef(xmen->geoKey, &xmen->geomat, GEO_ul_Load_ObjectCallback, 1);

    return data - start;
}

void GAO_ModifierXMEN_Reinit(tModifier* mdf)
{
    tXmenMdfStruct* xmen;

    xmen = (tXmenMdfStruct*)mdf->mdfPrivateDataPtr;
    mdf->modifierFlags |= 0x1000000u;

    if ((xmen->flags & 8) != 0)
    {
        memset(xmen->ptr1, 0, sizeof(tXmenMdfInner2) * xmen->num);
        memset(xmen->ptr2, 0, 8 * 4 * xmen->num);
        return;
    }

    if (xmen->geomat)
    {
        xmen->geomat->intf->AddRef((tGroGeneric*)xmen->geomat, 1);
        xmen->flags |= 8u;
    }

    if (!xmen->num)
    {
        xmen->ptr1 = 0;
        xmen->ptr2 = 0;
        return;
    }

    if (!xmen->ptr1)
        xmen->ptr1 = (tXmenMdfInner2*)MEM_p_Alloc(sizeof(tXmenMdfInner2) * xmen->num);
    if (!xmen->ptr2)
        xmen->ptr2 = (int*)MEM_p_Alloc(8 * 4 * xmen->num);
}

static Mtx4x4* GAO_l_ModifierXMEN_BoneMatrix(tGameObjectMain* gao, int multiPartDataID,
                                             Mtx4x4* relative)
{
    tGameObjectMultiPartData* parts;
    tMultiPartGameObjectLinks* link;
    Mtx4x4* mtx;
    Mtx4x4 inverse;

    if (!gao->basic)
        return &MATH_gst_IdentityMatrix;

    parts = gao->basic->multiPartData;
    // Unsigned, so that the element count rejects a negative id as well as
    // one past the end. -1 is the caller's "no particular element" and
    // arrives here routinely; the identity matrix is its answer.
    if (!parts || (unsigned)multiPartDataID >= (unsigned)parts->elemNum)
        return &MATH_gst_IdentityMatrix;

    if ((gao->flags & 0x1000000) == 0)
    {
        mtx = &((Mtx4x4*)parts->multipartGaoObjectPartsOrMatrices)[multiPartDataID];
        return mtx ? mtx : &MATH_gst_IdentityMatrix;
    }

    link = &((tMultiPartGameObjectLinks*)parts->multipartGaoObjectPartsOrMatrices)[multiPartDataID];
    if (link->mtxIdx != -1 && (!link->gao || (link->gao->flags & 0x1000000) == 0))
        return link->mtxPtr ? link->mtxPtr : &MATH_gst_IdentityMatrix;

    if (link->gao)
    {
        MATH_InvertMatrix(&inverse, gao->matrices);
        MATH_MulMatrixMatrix(relative, link->gao->matrices, &inverse);
    }
    else
    {
        MATH_SetIdentityMatrix(relative);
    }
    return relative;
}

void GAO_ModifierXMEN_GetBonePosFromNum(tGameObjectMain* gao, int multiPartDataID, Vec3* posRet,
                                        Vec3* mtxVecRetByType, unsigned int type, Vec3* offsetVec)
{
    Mtx4x4 relative;
    Mtx4x4* mtx;
    Vec3 offset;
    Vec3 tangent;

    mtx = GAO_l_ModifierXMEN_BoneMatrix(gao, multiPartDataID, &relative);
    *posRet = mtx->Pos;

    switch (type)
    {
    case 0u:
    case 6u:
        *mtxVecRetByType = mtx->v3;
        break;
    case 1u:
    case 5u:
        *mtxVecRetByType = mtx->v2;
        break;
    case 2u:
    case 4u:
        *mtxVecRetByType = mtx->v1;
        break;
    case 3u:
        mtxVecRetByType->x = gpst_DX_stDD->matrixStack.actMatrixElem->v1.z;
        mtxVecRetByType->y = gpst_DX_stDD->matrixStack.actMatrixElem->v2.z;
        mtxVecRetByType->z = gpst_DX_stDD->matrixStack.actMatrixElem->v3.z;
        break;
    default:
        break;
    }

    MATH_TransformVertex(posRet, gao->matrices, posRet);
    if (type != 3)
        MATH_TransformVector(mtxVecRetByType, gao->matrices, mtxVecRetByType);

    if (type >= 3 && type <= 6)
    {
        MATH_SubstractVec(&offset, posRet, offsetVec);
        MATH_CrossProduct(&tangent, mtxVecRetByType, &offset);
        MATH_NormalizeVectorWithNullVectorCheck(mtxVecRetByType, &tangent);
    }
}

void GAO_ModifierXMEN_SMOOTH_BUFFER_BendVectors_sub_4235A0(Vec3* dest, Vec3* from, Vec3* to)
{
    float weight;
    Vec3 mid;

    weight = xmenSmoothBuffer_flt_9684BC;
    MATH_BlendVector(&mid, &from[0], &to[0], 0.5f);
    MATH_BlendVector(&dest[0], &dest[0], &mid, weight);
    MATH_BlendVector(&mid, &from[1], &to[1], 0.5f);
    MATH_BlendVector(&dest[1], &dest[1], &mid, weight);
}

void GAO_ModifierXMEN_SMOOTH_BUFFER(Vec3* buff, unsigned int passNum)
{
    Vec3* last;
    Vec3* dst;
    Vec3* src;
    int step;
    int half;

    step = 2 * passNum;
    half = 2 * (passNum >> 1);
    last = &buff[2 * (32 - passNum)];

    MATH_BlendVector(&last[half], &buff[0], &last[0], 0.5f);
    MATH_BlendVector(&last[half + 1], &buff[1], &last[1], 0.5f);

    if (buff < last)
    {
        dst = &buff[half];
        src = buff;
        do
        {
            MATH_BlendVector(&dst[0], &src[0], &src[step], 0.5f);
            MATH_BlendVector(&dst[1], &src[1], &src[step + 1], 0.5f);
            dst += step;
            src += step;
        } while (src < last);
    }

    GAO_ModifierXMEN_SMOOTH_BUFFER_BendVectors_sub_4235A0(buff, &last[half], &buff[half]);
    for (dst = &buff[step]; dst <= last; dst += step)
        GAO_ModifierXMEN_SMOOTH_BUFFER_BendVectors_sub_4235A0(dst, &dst[-half], &dst[half]);
}

static tGeoTriangleChunkElem* GAO_l_ModifierXMEN_Quad(tGeoTriangleChunkElem* tris,
                                                      int cur, int prev)
{
    tris[0].v1 = 2 * cur;
    tris[0].uv1 = 2 * cur;
    tris[0].v2 = 2 * cur + 1;
    tris[0].uv2 = 2 * cur + 1;
    tris[0].v3 = 2 * prev + 1;
    tris[0].uv3 = 2 * prev + 1;
    tris[1].v1 = 2 * cur;
    tris[1].uv1 = 2 * cur;
    tris[1].v2 = 2 * prev + 1;
    tris[1].uv2 = 2 * prev + 1;
    tris[1].v3 = 2 * prev;
    tris[1].uv3 = 2 * prev;
    return tris + 2;
}

static void GAO_l_ModifierXMEN_TrailUVs(Vec3* point, tGeoUVElem* uv, int count, Vec3** prevPoint,
                                        float* run, float* step, float invLength, int alongU)
{
    while (count--)
    {
        if (*prevPoint)
            *step = MATH_DistanceOfTwoPoints(*prevPoint, point) * invLength;
        *run = *run + *step;
        *prevPoint = point;

        if (alongU)
        {
            uv[0].u = *run;
            uv[0].v = 0.0f;
            uv[1].u = *run;
            uv[1].v = 1.0f;
        }
        else
        {
            uv[0].u = 0.0f;
            uv[0].v = *run;
            uv[1].u = 1.0f;
            uv[1].v = *run;
        }

        uv += 2;
        point += 2;
    }
}

static void GAO_l_ModifierXMEN_TrailLength(Vec3* point, int count, Vec3** prevPoint,
                                           float* total)
{
    while (count--)
    {
        if (*prevPoint)
            *total = *total + MATH_DistanceOfTwoPoints(*prevPoint, point);
        *prevPoint = point;
        point += 2;
    }
}

void GAO_ModifierXMEN_DRAW(tXmenMdfStruct* xmen)
{
    tXmenSize* size;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uv;
    Vec3* head;
    Vec3* point;
    Vec3* prevPoint;
    Vec3* prevSlot;
    Vec3* next;
    Vec3* slot;
    Vec3* prev;
    Vec3* dst;
    Vec3* src;
    Vec3* verts;
    Vec3 from;
    Vec3 posRet;
    Vec3 mtxVec;
    Vec3 camDir;
    Vec3 delta;
    Vec3 unsmoothed;
    float nearScale;
    float farScale;
    float invLength;
    float run;
    float step;
    int entry;
    int num2;
    int prevNum2;
    int k;
    int n;
    int shift;
    int slots;
    int cur;
    int prevCur;
    int headIdx;
    int tailCount;
    int pass;

    if (!xmen->num)
        return;

    if (xmen->frameCounter != g_ulFrameCounter - 1)
    {
        xmen->num2 = 0;
        xmen->time = 9.9999998e12f;
    }
    if (xmen->deathTime < 0.00050000002f)
        xmen->deathTime = 0.00050000002f;
    xmen->time = TIM_gf_dt + xmen->time;
    if (xmen->time >= xmen->deathTime)
        xmen->time = 0.0;
    else
        xmen->num2 = (xmen->num2 - 1) & 7;

    num2 = xmen->num2;
    prevNum2 = (num2 + 7) & 7;
    for (entry = xmen->num - 1; entry >= 0; entry--)
    {
        size = &xmen->sizes[entry];
        prev = &xmen->ptr1[entry].data[2 * prevNum2];
        slot = &xmen->ptr1[entry].data[2 * num2];

        if (xmen->xmenType == 3)
        {
            MATH_BlendVector(&from, &prev[0], &prev[1], 0.5f);
            GAO_ModifierXMEN_GetBonePosFromNum(xmen->gao, size->multipartDataID,
                                               &posRet, &mtxVec, 3u, &from);
            nearScale = 0.0f;
            farScale = 0.0f;
        }
        else if ((xmen->flags & 0x10) != 0)
        {
            MATH_BlendVector(&from, &prev[0], &prev[1], 0.5f);
            GAO_ModifierXMEN_GetBonePosFromNum(xmen->gao, size->multipartDataID,
                                               &posRet, &mtxVec, xmen->xmenType, &from);
            nearScale = size->size * -0.5f;
            farScale = size->size * 0.5f;
        }
        else
        {
            from = prev[0];
            GAO_ModifierXMEN_GetBonePosFromNum(xmen->gao, size->multipartDataID,
                                               &posRet, &mtxVec, xmen->xmenType, &from);
            nearScale = 0.0f;
            farScale = size->size;
        }

        MATH_AddScaleVector(&slot[0], &posRet, &mtxVec, nearScale);
        MATH_AddScaleVector(&slot[1], &posRet, &mtxVec, farScale);
    }

    xmen->num2 = (xmen->num2 + 1) & 7;
    if (xmen->frameCounter != g_ulFrameCounter - 1)
    {
        xmen->num2 = 0;
        xmen->time = 9.9999998e12f;
        dst = xmen->ptr1->data;
        for (n = 8 * xmen->num; n; n--)
        {
            memcpy(dst, xmen->ptr1->data, 24u);
            dst += 2;
        }
    }

    if (xmen->xmenType == 3)
    {
        for (entry = xmen->num - 1; entry >= 0; entry--)
        {
            size = &xmen->sizes[entry];
            head = &xmen->ptr1[entry].data[2 * xmen->num2];

            if ((xmen->flags & 0x10) != 0)
            {
                point = xmen->ptr1[entry].data;
                for (k = 0; k < 8; k++, point += 2)
                    MATH_BlendVector(&point[0], &point[0], &point[1], 0.5f);
            }

            camDir = gpst_DX_stDD->ct.cam.objectMtx.v3;
            prevSlot = &xmen->ptr1[entry].data[14];
            point = xmen->ptr1[entry].data;
            for (k = 7; k >= 0; k--)
            {
                if (point == head)
                {
                    next = k ? &point[2] : xmen->ptr1[entry].data;
                    MATH_SubstractVec(&delta, next, point);
                }
                else
                {
                    MATH_SubstractVec(&delta, point, prevSlot);
                }
                MATH_CrossProduct(&point[1], &camDir, &delta);
                MATH_NormalizeVectorWithNullVectorCheck(&point[1], &point[1]);
                prevSlot = point;
                point += 2;
            }

            point = xmen->ptr1[entry].data;
            for (k = 0; k < 8; k++, point += 2)
            {
                if ((xmen->flags & 0x10) != 0)
                {
                    unsmoothed = point[0];
                    MATH_AddScaleVector(&point[0], &unsmoothed, &point[1], size->size * -0.5f);
                    MATH_AddScaleVector(&point[1], &unsmoothed, &point[1], size->size * 0.5f);
                }
                else
                {
                    MATH_AddScaleVector(&point[1], &point[0], &point[1], size->size);
                }
            }
        }
    }

    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
    if ((xmen->flags & 2) == 0)
        SOFT_l_MatrixStack_Push(&gpst_DX_stDD->matrixStack,
                                OBJ_pst_GetAbsoluteMatrix(xmen->gao));
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);

    for (entry = xmen->num - 1; entry >= 0; entry--)
    {
        memset(XMEN_OBJECT_TRIS, 0, sizeof(XMEN_OBJECT_TRIS));
        memset(XMEN_OBJECT_Points_Smoothed, 0, sizeof(XMEN_OBJECT_Points_Smoothed));

        if ((xmen->flags & 1) == 0)
        {
            // The entry stride here is half the 192 bytes an entry really
            // occupies, so every entry past the first draws a window that
            // straddles two of them. Left as it stands: the smoothed path
            // below uses the full stride, and which of the two the author
            // meant is not answerable from this tree.
            verts = (Vec3*)((char*)xmen->ptr1->data + 96 * entry);
            shift = 0;
            XMEN_ELEMENT[0].triangleNum = 14;
            XMEN_OBJECT.vertexNum = 16;
            XMEN_OBJECT.uvNum = 16;
        }
        else
        {
            src = xmen->ptr1[entry].data;
            dst = XMEN_OBJECT_Points_Smoothed;
            for (k = 0; k < 8; k++)
            {
                memcpy(dst, src, 24u);
                src += 2;
                dst += 8;
            }

            num2 = xmen->num2;
            prevNum2 = num2 ? num2 - 1 : 7;
            for (pass = 2; pass; pass--)
            {
                GAO_ModifierXMEN_SMOOTH_BUFFER(XMEN_OBJECT_Points_Smoothed, 1 << pass);
                memcpy(&XMEN_OBJECT_Points_Smoothed[8 * num2],
                       &xmen->ptr1[entry].data[2 * num2], 24u);
                memcpy(&XMEN_OBJECT_Points_Smoothed[8 * prevNum2],
                       &xmen->ptr1[entry].data[2 * prevNum2], 24u);
            }

            verts = XMEN_OBJECT_Points_Smoothed;
            shift = 2;
            XMEN_ELEMENT[0].triangleNum = 56;
            XMEN_OBJECT.vertexNum = 64;
            XMEN_OBJECT.uvNum = 64;
        }

        XMEN_OBJECT.vertices = verts;
        gpst_DX_stDD->verticesCurrent = verts;
        XMEN_ELEMENT[0].materialNum = 0;

        slots = 1 << (shift + 3);
        tris = XMEN_OBJECT_TRIS;
        cur = 0;
        prevCur = slots - 1;
        for (n = (1 << shift) - 1; n < (xmen->num2 << shift); n++)
        {
            tris = GAO_l_ModifierXMEN_Quad(tris, cur, prevCur);
            prevCur = cur++;
        }

        if (!shift || xmen->num2)
        {
            cur = (1 << shift) + cur;
            prevCur = cur - 1;
        }
        else
        {
            cur = 1;
            prevCur = 0;
        }
        while (cur < slots)
        {
            tris = GAO_l_ModifierXMEN_Quad(tris, cur, prevCur);
            prevCur = cur++;
        }

        num2 = xmen->num2;
        headIdx = (2 * num2) << shift;
        tailCount = (8 - num2) << shift;
        prevPoint = 0;
        invLength = 0.0f;
        GAO_l_ModifierXMEN_TrailLength(&XMEN_OBJECT_Points_Smoothed[headIdx],
                                       tailCount, &prevPoint, &invLength);
        GAO_l_ModifierXMEN_TrailLength(XMEN_OBJECT_Points_Smoothed,
                                       num2 << shift, &prevPoint, &invLength);
        if (invLength != 0.0)
            invLength = 1.0f / invLength;

        run = 0.0f;
        step = 0.0f;
        prevPoint = 0;
        uv = &XMEN_UVs[headIdx];
        GAO_l_ModifierXMEN_TrailUVs(&XMEN_OBJECT_Points_Smoothed[headIdx], uv, tailCount,
                                    &prevPoint, &run, &step, invLength,
                                    (xmen->flags & 0x20) != 0);
        GAO_l_ModifierXMEN_TrailUVs(XMEN_OBJECT_Points_Smoothed, XMEN_UVs, num2 << shift,
                                    &prevPoint, &run, &step, invLength,
                                    (xmen->flags & 0x20) != 0);

        if ((xmen->flags & 4) != 0)
            GAO_XMN_Render_TransparentTriangles(&XMEN_OBJECT, xmen->geomat);
        else
            MAT_DrawIndexedTriangle(&XMEN_OBJECT, xmen->geomat, XMEN_ELEMENT);
    }

    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
}

void GAO_ModifierXMEN_DRAW_ALL(void)
{
    tXmenMdfStruct* xmen;
    int slot;
    int frame;

    gpst_DX_stDD->MRMPtr = 0;
    gpst_DX_stDD->flags &= ~0x400;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x800;
    gpst_DX_stDD->flags |= 2u;

    frame = g_ulFrameCounter + 1;
    g_ulFrameCounter = frame;

    slot = XMEN_STACK_PTR;
    while (slot)
    {
        xmen = p_XMEN_STACK[--slot];
        XMEN_STACK_PTR = slot;
        if (!xmen)
            continue;

        if (xmen->frameCounter != frame)
        {
            GAO_ModifierXMEN_DRAW(xmen);
            frame = g_ulFrameCounter;
        }
        // Mark the entry just drawn. Re-reading the slot instead would
        // pick a different one when the draw itself applied modifiers,
        // and at a full stack that index is off the end of the array.
        xmen->frameCounter = frame;
        slot = XMEN_STACK_PTR;
    }
    XMEN_STACK_PTR = 0;
}

void GAO_ModifierXMEN_Unapply(struct sModifier* mdf, void* unk1)
{
}

typedef struct sXmenGaoScan
{
    tGameObjectMain* gao;
    tGroup* skeleton;
    tableElemStructPF* cur;
    tableElemStructPF* last;
} tXmenGaoScan;

static void XMEN_l_ScanBegin(tXmenGaoScan* scan, struct sGameObjectMain* gao)
{
    tGameObjectActorObject* actor;

    scan->gao = gao;
    scan->skeleton = 0;
    scan->cur = 0;
    scan->last = 0;
    if (!gao->basic || (gao->flags & 2) == 0)
        return;

    actor = (tGameObjectActorObject*)gao->basic->actorObjectOrVisualObject;
    if (!actor || !actor->skeletonGroup)
        return;

    scan->skeleton = actor->skeletonGroup;
    scan->last = ((tableElemStructPF*)scan->skeleton->tablePF->nextElemPtr) - 1;
}

static int XMEN_l_ScanNext(tXmenGaoScan* scan, tGameObjectMain** out)
{
    if (scan->cur > scan->last)
        return 0;

    if (scan->cur)
    {
        *out = (tGameObjectMain*)scan->cur->objectPtr;
    }
    else
    {
        *out = scan->gao;
        if (scan->last)
            scan->cur = ((tableElemStructPF*)scan->skeleton->tablePF->firstElemPtr) - 1;
    }
    ++scan->cur;
    return 1;
}

static int XMEN_l_ScanUsable(tGameObjectMain* obj)
{
    if (!obj || ((uintptr_t)obj & 3) == 1)
        return 0;
    return obj->extended != 0;
}

void XMEN_OnOff(struct sGameObjectMain* gao, int id, int onOff)
{
    tXmenGaoScan scan;
    tGameObjectMain* obj;
    tModifier* mdf;
    int disableBit;

    if (!gao)
        return;

    disableBit = onOff ? 0 : 0x2000000;
    XMEN_l_ScanBegin(&scan, gao);
    while (XMEN_l_ScanNext(&scan, &obj))
    {
        if (!XMEN_l_ScanUsable(obj))
            continue;

        for (mdf = obj->extended->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId != 9)
                continue;
            if (((tXmenMdfStruct*)mdf->mdfPrivateDataPtr)->id == id || id == -1)
            {
                mdf->modifierFlags &= ~0x2000000u;
                mdf->modifierFlags |= disableBit;
            }
        }
    }
}

void XMEN_SetSize(struct sGameObjectMain* gao, int id, int multipartDataID, float size)
{
    tXmenGaoScan scan;
    tGameObjectMain* obj;
    tModifier* mdf;
    tXmenMdfStruct* xmen;
    int i;

    if (!gao)
        return;

    XMEN_l_ScanBegin(&scan, gao);
    while (XMEN_l_ScanNext(&scan, &obj))
    {
        if (!XMEN_l_ScanUsable(obj))
            continue;

        for (mdf = obj->extended->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId != 9)
                continue;
            xmen = (tXmenMdfStruct*)mdf->mdfPrivateDataPtr;
            if (xmen->id != id && id != -1)
                continue;
            for (i = 0; i < xmen->num; i++)
            {
                if (multipartDataID == -1 || xmen->sizes[i].multipartDataID == multipartDataID)
                    xmen->sizes[i].size = size;
            }
        }
    }
}
