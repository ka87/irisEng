#include "cameraengine.h"
#include "memory.h"
#include "gdi.h"
#include "materialanddraw.h"
#include "modifiers/xmenmodifier.h"
#include "time.h"
#include "soft.h"
#include "water.h"

tZlistStruct* SOFT_gpc_BigBufferZlists = SOFT_gpc_BigBufferOnStack.zlists;
tBigBufferStruct* SOFT_gpc_BigBuffer = &SOFT_gpc_BigBufferOnStack;
tZListElemDataStruct* zListData = &(SOFT_gpc_BigBufferOnStack.ZListElemData);

void SOFT_MatrixStack_Reset(tMatrixStackStruct* mts, Mtx4x4* mtx)
{
    if (mts && mts->MaxElemNum)
    {
        if (mtx)
        {
            if (mts->elems != mtx)
            {
                memcpy(mts->elems, mtx, sizeof(Mtx4x4));
                mts->currElemNum = 0;
                mts->actMatrixElem = mts->elems;
                return;
            }
        }
        else
        {
            MATH_SetIdentityMatrix(mts->elems);
        }
        mts->currElemNum = 0;
        mts->actMatrixElem = mts->elems;
    }
}

void SOFT_l_MatrixStack_Create(tMatrixStackStruct* mts, int num)
{
    uint16_t tmpNum;
    Mtx4x4* mtxArray;

    tmpNum = num;
    if (num > 0 && mts)
    {
        mtxArray = MATH_Matrix_AllocArray(num);
        mts->elems = mtxArray;
        if (!mtxArray)
            tmpNum = 0;
        mts->MaxElemNum = tmpNum;
        SOFT_MatrixStack_Reset(mts, 0);
    }
}

int SOFT_l_MatrixStack_Push(tMatrixStackStruct* mts, Mtx4x4* mtx)
{
    if (mts->currElemNum + 2 == mts->MaxElemNum)
        return 0;
    
    MATH_MulMatrixMatrix(mts->actMatrixElem + 1, mtx, mts->actMatrixElem);
    ++mts->actMatrixElem;
    
    mts->actMatrixElem->w = 1.0f;
    
    return ++mts->currElemNum;
}

int SOFT_l_MatrixStack_Pop(tMatrixStackStruct* mts)
{
    uint16_t ret;

    ret = mts->currElemNum;
    if (!mts->currElemNum)
        return 0;
    
    --mts->actMatrixElem;    
    --mts->currElemNum;
    
    return ret;
}

void SOFT_Project(Vec3* dstVecs, Vec3* srcVecs, int verticesNum, tDDContainerStruct* cam)
{
    int i;
    float invVal;
   
    if ((cam->cam.flags & 1) != 0)
    {
        for (i = 0; i < verticesNum; i++)
        {
            invVal = MATH_GetInverse(srcVecs[i].z);
            dstVecs[i].x = cam->cam.centerX + (srcVecs[i].x * cam->cam.factorX * invVal);
            dstVecs[i].y = cam->cam.centerY - (srcVecs[i].y * cam->cam.factorY * invVal);
            dstVecs[i].z = invVal;
        }
    }
    else
    {
        for (i = 0; i < verticesNum; i++)
        {
            invVal = MATH_GetInverse(srcVecs[i].z);
            dstVecs[i].x = cam->cam.centerX + (srcVecs[i].x * cam->cam.factorX);
            dstVecs[i].y = cam->cam.centerY - (srcVecs[i].y * cam->cam.factorY);
            dstVecs[i].z = invVal;
        }
    }
}

void SOFT_TransformAndProject(Vec3* dstVecs, Vec3* srcVecs, int verticesNum, tDDCamStruct* cam)
{
    int i;
    float invVal;

    if ((cam->flags & 1) != 0)
    {
        for (i = 0; i < verticesNum; i++)
        {
            MATH_TransformVertex(&dstVecs[i], cam->viewMtxPtr_setInGRORender, &srcVecs[i]);
            if (dstVecs[i].z > 0.0f)
            {
                invVal = MATH_GetInverse(dstVecs[i].z);
                dstVecs[i].x = cam->centerX + cam->factorX * dstVecs[i].x * invVal;
                dstVecs[i].y = cam->centerY - cam->factorY * dstVecs[i].y * invVal;
                dstVecs[i].z = invVal;
            }
            else
            {
                dstVecs[i].x = (float)INT_MAX;
                dstVecs[i].y = (float)INT_MAX;
            }
        }
    }
    else
    {
        for (i = 0; i < verticesNum; i++)
        {
            MATH_TransformVertex(&dstVecs[i], cam->viewMtxPtr_setInGRORender, &srcVecs[i]);
            dstVecs[i].x = cam->centerX + cam->factorX * dstVecs[i].x;
            dstVecs[i].y = cam->centerY - cam->factorY * dstVecs[i].y;
            dstVecs[i].z = MATH_GetInverse(dstVecs[i].z);
        }
    }
}

void SOFT_ZList_Clear()
{
    int i;
    
    for (i = 8; i > 0; i--)
    {
        p_Current_SOFT_gst_ZList = &SOFT_gpc_BigBufferZlists[i - 1];
        p_Current_SOFT_gst_ZList->maxZ = 0;
        p_Current_SOFT_gst_ZList->minZ = 16384;
    }

    zListData->currPtr = zListData->elems;
    zListData->currMeshOffset = zListData->meshElems;
    zListData->currObjOffset = zListData->objElems;
}

void SOFT_ZList_Init()
{
    int i;

    MEM_gp_AllocTmpNext = &SOFT_gpc_BigBufferOnStack.data1LightColors[44];
    MEM_gp_AllocTmpFirst = &SOFT_gpc_BigBufferOnStack.data1LightColors[44];
    MEM_gp_AllocTmpLast = &SOFT_gpc_BigBufferOnStack.lastElem;// + 2c van a végéig, teháét ez az egész 44-gyel van eltolva a 0x10 (data1-tõl)

    for (i = 8; i > 0; i--)
    {
        p_Current_SOFT_gst_ZList = &SOFT_gpc_BigBufferZlists[i - 1];
        memset(&SOFT_gpc_BigBufferZlists[i - 1], 0, sizeof(tZlistStruct));
    }
    
    SOFT_ZList_Clear();
    
    memset(&ZList_Obj, 0, sizeof(tGEOGRO)); // TODO FIXME: 92 vs 96
    
    ZList_Obj.vertexNum = 3;
    ZList_Obj.uvNum = 3;
    ZList_Obj.triangleChunks = &ZList_Element;
    ZList_Obj.triangleChunkCount = 1;
    ZList_Obj.vertices = ZList_Points;
    ZList_Obj.UVs = ZListUVs;
    ZList_Obj.vertexColors = ZListVertexColors;
    ZList_Obj.intf = &GRO_gast_Interface[1];
    ZList_Element.triangleChunkElems = &ZListTriangleChunkElem;
    ZList_Element.materialNum = 0;
    ZList_Element.triangleNum = 1;
    ZList_Element.MRMUVIndexElems = NULL;
    ZList_Element.triangleStripBlock = NULL;
    ZList_Element.MRMUVIndices = NULL;
    ZList_Element.MRMUVIndicesNum = 0;
    ZListTriangleChunkElem.v1 = 0;
    ZListTriangleChunkElem.v2 = 1;
    ZListTriangleChunkElem.v3 = 2;
    ZListTriangleChunkElem.uv1 = 0;
    ZListTriangleChunkElem.uv2 = 1;
    ZListTriangleChunkElem.uv3 = 2;
    ZList_Locked = FALSE;
}

void SOFT_ZList_Lock()
{
    ZList_Locked = TRUE;
}

void SOFT_ZList_Unlock()
{
    ZList_Locked = FALSE;
}

void SOFT_TransformInComputingBuffer(tBigBufferStruct* a1, Vec3* vertices, int vertexNum, tDDContainerStruct* a4)
{
    int i;
    Vec3* buffer = (Vec3*)a1->data6;

    for (i = 0; i < vertexNum; i++) {
        MATH_TransformVertex(&buffer[i], a4->cam.viewMtxPtr_setInGRORender, &vertices[i]);
    }
}

struct sZListElem* SOFT_pst_ZList_AddNode(float z, int num, int isObject)
{
    tZListElem* ret;
    int zListGroup;

    if (zListData->currPtr >= &zListData->elems[1023] || ZList_Locked)
        return NULL;

    if (z < 0.0f)
    {
        if (isObject != 1)
            return NULL;
        
        z = 0.0f;
    }

    zListGroup = (*((uint32_t *)&z)) >> 17;
    
    if (zListGroup >= 16384)
        return NULL;
    
    if (p_Current_SOFT_gst_ZList->minZ > zListGroup)
        p_Current_SOFT_gst_ZList->minZ = zListGroup;

    if (p_Current_SOFT_gst_ZList->maxZ < zListGroup)
        p_Current_SOFT_gst_ZList->maxZ = zListGroup;

    if (!isObject)
    {
        if ((int)(zListData->objElems) > (int)zListData->currMeshOffset) // a meshelems után az objelems jön, szóval ha megtelt a meshElems, akkor a currMeshOffset rálóg az objelemsre. Ezzel nézi meg ohgy megtelt-e
        {
            zListData->currPtr->data = zListData->currMeshOffset;
            zListData->currMeshOffset += num;
            zListData->currPtr->currZElemGroup = p_Current_SOFT_gst_ZList->zElemGroup[zListGroup];
            p_Current_SOFT_gst_ZList->zElemGroup[zListGroup] = zListData->currPtr;
            ret = zListData->currPtr;
            zListData->currPtr = ret + 1;
            return ret;
        }
        return NULL;
    }

    if (isObject == 1)
    {
        if ((int)(&zListData[1]) > (int)zListData->currObjOffset) // ezzel azt nézi meg, hogy mivel az objElems amire az objoffset mutat nem nagyobb-e mint az egé struktúra. Az objElems a struktúravégén van, szóval ha már több nem fér bele akkor túllóg a struktúrán
        {
            zListData->currPtr->data = zListData->currObjOffset;
            ++zListData->currObjOffset;
            zListData->currPtr->currZElemGroup = p_Current_SOFT_gst_ZList->zElemGroup[zListGroup];
            p_Current_SOFT_gst_ZList->zElemGroup[zListGroup] = zListData->currPtr;
            ret = zListData->currPtr;
            zListData->currPtr = ret + 1;
            return ret;
        }
        return NULL;
    }

    ret = zListData->currPtr;
    zListData->currPtr = ret + 1;
    return ret;
}

void SOFT_AddCurrentObjectInZList(struct sGdi_DisplayData* dd, struct sGameObjectMain* gao)
{
    tZListElem* zle;
    tZListObjElem* zleObj;

    if ((dd->flags & 0x1000) == 0)
    {
        zle = SOFT_pst_ZList_AddNode((1.0f / dd->matrixStack.actMatrixElem->Pos.z), 1, TRUE);
        if (zle)
        {
            zle->renderingFlags = dd->drawMaskAllowedStuffCurrent;
            zleObj = (tZListObjElem*)zle->data;
            zleObj->gao = gao;

            if (&zleObj->worldMtx != dd->matrixStack.actMatrixElem)
                memcpy(&zleObj->worldMtx, dd->matrixStack.actMatrixElem, sizeof(Mtx4x4));

            if (&zleObj->gaoMatrix != dd->currTargetGao->matrices)
                memcpy(&zleObj->gaoMatrix, dd->currTargetGao->matrices, sizeof(Mtx4x4));
        }
        dd->flags |= 0x1000;
    }
}

void SOFT_pst_InitSpecularColors(int num)
{
    memset(gpst_DX_stDD->bigBuffer->data2SpecularColors, 0, sizeof(uint32_t) * num);
    gpst_DX_stDD->bigBuffer->flags |= 0x4;
}

void SOFT_ComputeSpecularVectors(tGEOGRO* gro)
{
    Vec3* specularVecs;
    float tmp;
    float invLen;
    Vec3 transformedPos;
    Vec3 localPos;
    Mtx4x4 invMtx;
    unsigned int i;

    QueryPerformanceCounter(&PerfCounterStruct[9]);
    transformedPos.z = 0.0f;
    transformedPos.y = 0.0f;
    transformedPos.x = 0.0f;

    MATH_InvertMatrix(&invMtx, gpst_DX_stDD->matrixStack.actMatrixElem);
    MATH_TransformVertex(&transformedPos, &invMtx, &transformedPos);
    GEO_UseNormals(gro);

    specularVecs = (Vec3*)gpst_DX_stDD->bigBuffer->data8SpecularVectors;

    if ((gpst_DX_stDD->drawMaskAllowedStuffCurrent & 0x8000) != 0)
    {
        for (i = 0; i < gro->vertexNum; i++)
        {
            localPos.x = gpst_DX_stDD->verticesCurrent[i].x - transformedPos.x;
            localPos.y = gpst_DX_stDD->verticesCurrent[i].y - transformedPos.y;
            localPos.z = gpst_DX_stDD->verticesCurrent[i].z - transformedPos.z;
            invLen = MATH_GetInverseSqrt(localPos.z * localPos.z + localPos.y * localPos.y + localPos.x * localPos.x);
            tmp = (localPos.z * gro->normals[i].z + localPos.y * gro->normals[i].y + localPos.x * gro->normals[i].x) * -1.9f;
            specularVecs[i].x = (tmp * gro->normals[i].x + localPos.x) * invLen;
            specularVecs[i].y = (tmp * gro->normals[i].y + localPos.y) * invLen;
            specularVecs[i].z = (tmp * gro->normals[i].z + localPos.z) * invLen;
        }
    }
    else
    {
        for (i = 0; i < gro->vertexNum; i++)
        {
            localPos.x = -gpst_DX_stDD->verticesCurrent[i].x - transformedPos.x;
            localPos.y = gpst_DX_stDD->verticesCurrent[i].y - transformedPos.y;
            localPos.z = gpst_DX_stDD->verticesCurrent[i].z - transformedPos.z;
            invLen = MATH_GetInverseSqrt(localPos.z * localPos.z + localPos.y * localPos.y + localPos.x * localPos.x);
            tmp = (localPos.z * gro->normals[i].z - localPos.x * gro->normals[i].x + localPos.y * gro->normals[i].y) * -1.9f;
            specularVecs[i].x = -((-tmp * gro->normals[i].x + localPos.x) * invLen);
            specularVecs[i].y =   ( tmp * gro->normals[i].y + localPos.y) * invLen;
            specularVecs[i].z =   ( tmp * gro->normals[i].z + localPos.z) * invLen;
        }
    }

    gpst_DX_stDD->bigBuffer->flags |= 0x1;
    PRO_QPC_RecordTimeDecRef(9);
}

Vec3* SOFT_pst_GetSpecularVectors()
{
    if ((gpst_DX_stDD->bigBuffer->flags & 1) == 0)
    {
        if (!gpst_DX_stDD->GROGEO)
            return NULL;
        SOFT_ComputeSpecularVectors(gpst_DX_stDD->GROGEO);
    }

    return (Vec3*)gpst_DX_stDD->bigBuffer->data8SpecularVectors;
}

void SOFT_ZList_Draw()
{
    tZListElem** ppCurrZElem, **ppLastZElem;
    tZListElem* pCurrZElem;
    tGameObjectMain* currGao;
    Mtx4x4* matricesBackup;
    tGEOGRO* groGeneric;

    QueryPerformanceCounter(&PerfCounterStruct[13]);
    
    gpst_DX_stDD->flags |= 0x4;

    ppLastZElem = &p_Current_SOFT_gst_ZList->zElemGroup[p_Current_SOFT_gst_ZList->maxZ];

    if (p_Current_SOFT_gst_ZList->maxZ >= p_Current_SOFT_gst_ZList->minZ)
    {
        for (ppCurrZElem = &p_Current_SOFT_gst_ZList->zElemGroup[p_Current_SOFT_gst_ZList->minZ]; ppCurrZElem <= ppLastZElem; ppCurrZElem++)
        {
            pCurrZElem = *ppCurrZElem;
            if (pCurrZElem)
            {
                do
                {
                    if ((int)(pCurrZElem->data) >= (int)(zListData->objElems)) // check type by offset in struct
                    {
                        if (ZList_V != GFX_gpst_Geo->vertices)
                        {
                            GFX_gpst_Geo->vertexNum = ZList_V - GFX_gpst_Geo->vertices;
                            GFX_gpst_Geo->uvNum = ZList_UVs - GFX_gpst_Geo->UVs;
                            GFX_gpst_Geo->triangleChunks->triangleNum = ZList_T - GFX_gpst_Geo->triangleChunks->triangleChunkElems;
                            gpst_DX_stDD->verticesCurrent = GFX_gpst_Geo->vertices;
                            MAT_DrawIndexedTriangle(GFX_gpst_Geo, ZList_Mat, GFX_gpst_Geo->triangleChunks);
                            ZList_T = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
                            ZList_V = GFX_gpst_Geo->vertices;
                            ZList_UVs = GFX_gpst_Geo->UVs;
                            ZList_RLIs = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
                        }

                        ++gpst_DX_stDD->rasters[11].unk2;

                        currGao = ((tZListObjElem*)(pCurrZElem->data))->gao;
                        gpst_DX_stDD->drawMaskAllowedStuffCurrent = pCurrZElem->renderingFlags;
                        SOFT_l_MatrixStack_Push(&gpst_DX_stDD->matrixStack, &(((tZListObjElem*)(pCurrZElem->data))->worldMtx));
                        gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
                        gpst_DX_stDD->bigBuffer->flags &= ~0x1;
                        gpst_DX_stDD->currTargetGao = currGao;
                        matricesBackup = currGao->matrices;
                        currGao->matrices = &(((tZListObjElem*)(pCurrZElem->data))->gaoMatrix);

                        if ((currGao->flags & 0x4000) != 0
                            && ((gpst_DX_stDD->extraMaterialToGeo = currGao->basic->extraMaterial,
                                groGeneric = (tGEOGRO*)((tGameObjectVisualObject*)(currGao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse,
                                   groGeneric->intf->id == 8)
                                || groGeneric->intf->id == 1
                                || groGeneric->intf->id == 11))
                        {
                            gpst_DX_stDD->SetViewMatrix(&(((tZListObjElem*)(pCurrZElem->data))->worldMtx));
                            i_Identity = FALSE;
                            groGeneric->intf->Render(currGao);
                            ZList_T = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
                            ZList_V = GFX_gpst_Geo->vertices;
                            ZList_UVs = GFX_gpst_Geo->UVs;
                            ZList_RLIs = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
                        }

                        gpst_DX_stDD->flags &= ~0x400;
                        SOFT_l_MatrixStack_Pop(&gpst_DX_stDD->matrixStack);
                        currGao->matrices = matricesBackup;
                    }
                    else
                    {

                        ++gpst_DX_stDD->rasters[12].unk2;

                        if (pCurrZElem->renderingFlags != gpst_DX_stDD->drawMaskAllowedStuffCurrent
                            || ZList_Mat != pCurrZElem->geomat
                            || ZList_V - GFX_gpst_Geo->vertices >= 768)
                        {
                            if (ZList_V != GFX_gpst_Geo->vertices)
                            {
                                GFX_gpst_Geo->vertexNum = ZList_V - GFX_gpst_Geo->vertices;
                                GFX_gpst_Geo->uvNum = ZList_UVs - GFX_gpst_Geo->UVs;
                                GFX_gpst_Geo->triangleChunks->triangleNum = ZList_T - GFX_gpst_Geo->triangleChunks->triangleChunkElems;
                                gpst_DX_stDD->verticesCurrent = GFX_gpst_Geo->vertices;
                                MAT_DrawIndexedTriangle(GFX_gpst_Geo, ZList_Mat, GFX_gpst_Geo->triangleChunks);
                                ZList_T = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
                                ZList_V = GFX_gpst_Geo->vertices;
                                ZList_UVs = GFX_gpst_Geo->UVs;
                                ZList_RLIs = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
                            }
                        }
                        gpst_DX_stDD->drawMaskAllowedStuffCurrent = pCurrZElem->renderingFlags;
                        ZList_Mat = pCurrZElem->geomat;

                        if (!i_Identity)
                        {
                            gpst_DX_stDD->SetViewMatrix(&MATH_gst_IdentityMatrix);
                            gpst_DX_stDD->RLIPtr = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
                            gpst_DX_stDD->bigBuffer->vertexColorArray = gpst_DX_stDD->RLIPtr;
                            gpst_DX_stDD->extraMaterialToGeo = NULL;
                            i_Identity = TRUE;
                        }

                        ZList_T->uv1 = (uint16_t)(((int)ZList_V - (int)(GFX_gpst_Geo->vertices)) / sizeof(Vec3));
                        ZList_T->v1 = ZList_T->uv1;
                        ZList_T->uv2 = ZList_T->v1 + 1;
                        ZList_T->v2 = ZList_T->uv2;
                        ZList_T->uv3 = ZList_T->v1 + 2;
                        ZList_T->v3 = ZList_T->uv3;
                        ++ZList_T;
                        *(ZList_V++) = ((tZListMeshElem*)(pCurrZElem->data))[0].point;
                        *(ZList_V++) = ((tZListMeshElem*)(pCurrZElem->data))[1].point;
                        *(ZList_V++) = ((tZListMeshElem*)(pCurrZElem->data))[2].point;
                        ZList_UVs->u = ((tZListMeshElem*)(pCurrZElem->data))[0].uv.u;
                        ZList_UVs->v = ((tZListMeshElem*)(pCurrZElem->data))[0].uv.v;
                        ++ZList_UVs;
                        ZList_UVs->u = ((tZListMeshElem*)(pCurrZElem->data))[1].uv.u;
                        ZList_UVs->v = ((tZListMeshElem*)(pCurrZElem->data))[1].uv.v;
                        ++ZList_UVs;
                        ZList_UVs->u = ((tZListMeshElem*)(pCurrZElem->data))[2].uv.u;
                        ZList_UVs->v = ((tZListMeshElem*)(pCurrZElem->data))[2].uv.v;
                        ++ZList_UVs;
                        *ZList_RLIs++ = ((tZListMeshElem*)(pCurrZElem->data))[0].vertexColor;
                        *ZList_RLIs++ = ((tZListMeshElem*)(pCurrZElem->data))[1].vertexColor;
                        *ZList_RLIs++ = ((tZListMeshElem*)(pCurrZElem->data))[2].vertexColor;
                    }
                    if (pCurrZElem == pCurrZElem->currZElemGroup)
                        pCurrZElem->currZElemGroup = 0;
                    pCurrZElem = pCurrZElem->currZElemGroup;
                } while (pCurrZElem);
            }

            *ppCurrZElem = NULL;
        }
    }
    
    if (ZList_V != GFX_gpst_Geo->vertices)
    {
        GFX_gpst_Geo->vertexNum = ZList_V - GFX_gpst_Geo->vertices;
        GFX_gpst_Geo->uvNum = ZList_UVs - GFX_gpst_Geo->UVs;
        GFX_gpst_Geo->triangleChunks->triangleNum = ZList_T - GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        gpst_DX_stDD->verticesCurrent = GFX_gpst_Geo->vertices;
        MAT_DrawIndexedTriangle(GFX_gpst_Geo, ZList_Mat, GFX_gpst_Geo->triangleChunks);
        ZList_T = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        ZList_V = GFX_gpst_Geo->vertices;
        ZList_UVs = GFX_gpst_Geo->UVs;
        ZList_RLIs = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
    }

    gpst_DX_stDD->flags &= ~0x4;

    PRO_QPC_RecordTimeDecRef(13);
}

void SOFT_ZList_Send()
{
    int i;

    GAO_ModifierXMEN_DRAW_ALL();

    PRO_PerfCounterStartBlock(&gpst_DX_stDD->rasters[46]);

    i_Identity = FALSE;
    SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &MATH_gst_IdentityMatrix);
    GFX_NeedGeom(768, 768, 256, 0);
    ZList_T = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    ZList_V = GFX_gpst_Geo->vertices;
    ZList_UVs = GFX_gpst_Geo->UVs;
    ZList_RLIs = (int*)gpst_DX_stDD->bigBuffer->data3FinalColorWithShadow;
    
    for (i = 8; i > 0; i--)
    {
        PC_AE_ZListHook(i - 1 - 4);
        p_Current_SOFT_gst_ZList = &SOFT_gpc_BigBufferZlists[i - 1];
        SOFT_ZList_Draw();
    }

    SOFT_ZList_Clear();

    PRO_PerfCounterEndBlock(&gpst_DX_stDD->rasters[46]);
}

void SOFT_l_MatrixStack_Destroy(struct sMatrixStackStruct* mts)
{
    if (mts->elems)
        MEM_FreeAlign(mts->elems);
    mts->MaxElemNum = 0;
}

