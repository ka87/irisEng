#include "../memory.h"
#include "../loading.h"
#include "../time.h"
#include "../soft.h"
#include "../materialanddraw.h"
#include "xmenmodifier.h"

int bFirst_286 = 0; // game doesn't set this explicitly
int XMEN_STACK_PTR = 0; // game doesn't set this explicitly
float xmenSmoothBuffer_flt_9684BC = 0.5f;

void XMEN_FirstInit()
{
    if (!bFirst_286)
    {
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
}

void GAO_ModifierXMEN_Create(struct sGameObjectMain* gao, struct sModifier* a2, void *a3)
{
    tXmenMdfStruct* v3;
    tModifier* v4;
    tXmenMdfStruct* v5;
    int v6;
    tXmenSize* v7;
    unsigned int v8;
    tXmenMdfInner2* v9;
    unsigned int v10;
    int* v11;
    unsigned int v12;
    int* v13;
    int* v14;
    int* v15;
    char v16;
    tGEOMAT* v17;
    struct sXmenMdfStruct* mdfStruct = (struct sXmenMdfStruct*)a3;

    XMEN_FirstInit();
    v3 = (tXmenMdfStruct*)MEM_p_Alloc(sizeof(tXmenMdfStruct));
    v4 = a2;
    v5 = v3;
    a2->mdfPrivateDataPtr = v3;
    if (!mdfStruct)
    {
        memset(v3, 0, sizeof(tXmenMdfStruct));
        ((tXmenMdfStruct*)(a2->mdfPrivateDataPtr))->geoKey = -1;
        a2->modifierFlags = 0x1000000;
    LABEL_3:
        ((tXmenMdfStruct*)(v4->mdfPrivateDataPtr))->gao = gao;
        return;
    }
    a2->modifierFlags = 0x1000000;
    memcpy(v3, mdfStruct, sizeof(tXmenMdfStruct));
    v6 = v3->num;
    if (v6)
    {
        v7 = (tXmenSize*)MEM_p_Alloc(sizeof(tXmenSize) * v6);
        v8 = sizeof(tXmenSize) * v5->num;
        v5->sizes = v7;
        memcpy(v7, mdfStruct->sizes, v8);
        v9 = (tXmenMdfInner2*)MEM_p_Alloc(sizeof(tXmenMdfInner2) * v5->num);
        v10 = sizeof(tXmenMdfInner2) * v5->num;
        v5->ptr1 = v9;
        memcpy(v9, mdfStruct->ptr1, v10);
        v11 = (int*)MEM_p_Alloc(8 * 4 * v5->num);
        v12 = 8 * 4 * v5->num;
        v5->ptr2 = v11;
        v13 = mdfStruct->ptr2;
        memcpy(v11, v13, 4 * (v12 >> 2));
        v15 = &v13[v12 >> 2];
        v14 = &v11[v12 >> 2];
        v16 = v12;
        v4 = a2;
        memcpy(v14, v15, v16 & 3);
    }
    v17 = v5->geomat;
    if (!v17)
        goto LABEL_3;
    v17->intf->AddRef((tGroGeneric*)v17, 1);
    ((tXmenMdfStruct*)(a2->mdfPrivateDataPtr))->gao = gao;
}

void GAO_ModifierXMEN_Destroy(struct sModifier* a1)
{
    tXmenMdfStruct* v1;
    int v2;
    tXmenMdfStruct* v3;
    tGEOMAT* v4;
    tXmenMdfStruct* v5;

    v1 = (tXmenMdfStruct*)(a1->mdfPrivateDataPtr);
    if (v1)
    {
        v2 = XMEN_STACK_PTR;
        while (v2)
        {
            v3 = p_XMEN_STACK[--v2];
            if (v3 == v1)
                p_XMEN_STACK[v2] = 0;
        }
        v4 = v1->geomat;
        if (v4)
        {
            v4->intf->AddRef((tGroGeneric*)(v4), -1);
            v1->geomat->intf->Destroy((tGroGeneric*)(v1->geomat));
        }
        v5 = a1->mdfPrivateDataPtr;
        if (v5->num)
        {
            MEM_Free(v5->sizes);
            MEM_Free(((tXmenMdfStruct*)(a1->mdfPrivateDataPtr))->ptr1);
            MEM_Free(((tXmenMdfStruct*)(a1->mdfPrivateDataPtr))->ptr2);
        }
        MEM_Free(a1->mdfPrivateDataPtr);
    }
}

void GAO_ModifierXMEN_Apply(struct sModifier* a1, void *unk1)
{
    int v1;

    v1 = XMEN_STACK_PTR + 1;
    p_XMEN_STACK[XMEN_STACK_PTR & 0x5F] = (tXmenMdfStruct*)(a1->mdfPrivateDataPtr);
    XMEN_STACK_PTR = v1;
}

int GAO_ModifierXMEN_Load(struct sModifier* arg0, char* a1)
{
    tXmenMdfStruct* v2;
    int v3;
    tXmenSize* v4;
    int i;
    int v6;
    tXmenMdfInner2* v7;
    unsigned int v8;
    int* v9;
    unsigned int v10;
    tGEOMAT** v11;
    BOOL v12;
    int v13;
    unsigned int v15;
    char* v16;

    XMEN_FirstInit();
    v2 = arg0->mdfPrivateDataPtr;
    v16 = a1;
    LOA_ReadLong(&a1, 0, 2);
    v15 = LOA_ReadLong(&a1, 0, 1);
    v2->flags = LOA_ReadLong(&a1, 0, 1) & ~8u;
    v3 = LOA_ReadLong(&a1, 0, 1);
    v2->num = v3;
    v2->sizes = 0;
    if (v3)
        v2->sizes = (tXmenSize*)MEM_p_Alloc(sizeof(tXmenSize) * v3);
    v4 = v2->sizes;
    for (i = 0; i < v2->num; ++v4)
    {
        v4->multipartDataID = LOA_ReadLong(&a1, 0, 1);
        v4->size = LOA_ReadFloat(&a1, 0, 1);
        ++i;
    }
    v6 = v2->num;
    v2->ptr1 = 0;
    v2->ptr2 = 0;
    if (v6)
    {
        v7 = (tXmenMdfInner2*)MEM_p_Alloc(sizeof(tXmenMdfInner2) * v6);
        v8 = sizeof(tXmenMdfInner2) * v2->num;
        v2->ptr1 = v7;
        memset(v7, 0, v8);
        v9 = (int*)MEM_p_Alloc(8 * 4 * v2->num);
        v10 = 8 * 4 * v2->num;
        v2->ptr2 = v9;
        memset(v9, 0, v10);
    }
    v11 = &v2->geomat;
    v2->geoKey = LOA_ReadLong(&a1, 0, 1);
    v2->geomat = 0;
    if (v15)
        v2->xmenType = LOA_ReadLong(&a1, 0, 1);
    if (v15 > 1)
        v2->deathTime = LOA_ReadFloat(&a1, 0, 1);
    if (v15 > 2)
        v2->id = LOA_ReadLong(&a1, 0, 1);
    v12 = (v2->flags & 8) == 0;
    v2->gao = arg0->gao;
    v2->frameCounter = 0;
    v2->num2 = 0;
    if (v12)
    {
        v13 = v2->geoKey;
        if (v13)
        {
            if (v13 != -1)
                LOA_MakeFileRef(v13, v11, GEO_ul_Load_ObjectCallback, 1);
        }
    }
    return a1 - v16;
}

void GAO_ModifierXMEN_Reinit(tModifier* a1)
{
    tXmenMdfStruct* v1;
    tGEOMAT* v2;
    int v3;

    v1 = (tXmenMdfStruct*)(a1->mdfPrivateDataPtr);
    a1->modifierFlags |= 0x1000000u;
    if ((v1->flags & 8) != 0)
    {
        memset(v1->ptr1, 0, sizeof(tXmenMdfInner2) * v1->num);
        memset(v1->ptr2, 0, 32 * v1->num);
    }
    else
    {
        v2 = v1->geomat;
        if (v2)
        {
            v2->intf->AddRef((tGroGeneric*)v2, 1);
            v1->flags |= 8u;
        }
        v3 = v1->num;
        if (v3)
        {
            if (!v1->ptr1)
                v1->ptr1 = (tXmenMdfInner2*)MEM_p_Alloc(sizeof(tXmenMdfInner2) * v3);
            if (!v1->ptr2)
                v1->ptr2 = (int*)MEM_p_Alloc(8 * 4 * v1->num);
        }
        else
        {
            v1->ptr1 = 0;
            v1->ptr2 = 0;
        }
    }
}

void GAO_ModifierXMEN_GetBonePosFromNum(tGameObjectMain* a1, int multiPartDataID, Vec3* posRet, Vec3* mtxVecRetByType, unsigned int type, Vec3* offsetVec)
{
    tGameObjectBasic* v6;
    tGameObjectMultiPartData* v7;
    tMultiPartGameObjectLinks* v8;
    int v9;
    tMultiPartGameObjectLinks* v10;
    Mtx4x4* v11;
    tGameObjectMain* v12;
    tGameObjectMain* v13;
    Mtx4x4* v14; 
    Vec3* v15;
    float v16;
    float v17;
    float v18;
    Vec3 a2;
    Mtx4x4 v20;
    Mtx4x4 a1a;

    v6 = a1->basic;
    if (!v6 || (v7 = v6->multiPartData) == 0 || v7->elemNum <= multiPartDataID)
    {
    LABEL_14:
        v11 = &MATH_gst_IdentityMatrix;
        goto LABEL_15;
    }
    if ((a1->flags & 0x1000000) == 0)
    {
        v11 = &(((Mtx4x4*)(v7->multipartGaoObjectPartsOrMatrices))[multiPartDataID]);
        goto LABEL_13;
    }
    v8 = (tMultiPartGameObjectLinks*)v7->multipartGaoObjectPartsOrMatrices;
    v9 = v8[multiPartDataID].mtxIdx;
    v10 = &v8[multiPartDataID];
    v11 = v10->mtxPtr;
    if (v9 != -1)
    {
        v12 = v10->gao;
        if (!v12 || (v12->flags & 0x1000000) == 0)
        {
        LABEL_13:
            if (v11)
                goto LABEL_15;
            goto LABEL_14;
        }
    }
    v13 = v10->gao;
    if (v13)
    {
        v14 = v13->matrices;
        MATH_InvertMatrix(&a1a, a1->matrices);
        MATH_MulMatrixMatrix(&v20, v14, &a1a);
    }
    else
    {
        MATH_SetIdentityMatrix(&v20);
    }
    v11 = &v20;
LABEL_15:
    *posRet = v11->Pos;
    switch (type)
    {
    case 0u:
    case 6u:
        v15 = &v11->v3;
        goto LABEL_19;
    case 1u:
    case 5u:
        v15 = &v11->v2;
    LABEL_19:
        *mtxVecRetByType = *v15;
        break;
    case 2u:
    case 4u:
        *mtxVecRetByType = v11->v1;
        break;
    case 3u:
        mtxVecRetByType->x = gpst_DX_stDD->matrixStack.actMatrixElem->v1.z;
        mtxVecRetByType->y = gpst_DX_stDD->matrixStack.actMatrixElem->v2.z;
        mtxVecRetByType->z = gpst_DX_stDD->matrixStack.actMatrixElem->v3.z;
        break;
    default:
        break;
    }
    MATH_TransformVertex(posRet, a1->matrices, posRet);
    if (type != 3)
        MATH_TransformVector(mtxVecRetByType, a1->matrices, mtxVecRetByType);
    v16 = posRet->x - offsetVec->x;
    v17 = posRet->y - offsetVec->y;
    v18 = posRet->z - offsetVec->z;
    if (type >= 3 && type <= 6)
    {
        a2.x = v18 * mtxVecRetByType->y - v17 * mtxVecRetByType->z;
        a2.y = v16 * mtxVecRetByType->z - v18 * mtxVecRetByType->x;
        a2.z = v17 * mtxVecRetByType->x - v16 * mtxVecRetByType->y;
        MATH_NormalizeVectorWithNullVectorCheck(mtxVecRetByType, &a2);
    }
}

void GAO_ModifierXMEN_SMOOTH_BUFFER_BendVectors_sub_4235A0(Vec3* dest, Vec3* a2, Vec3* a3)
{
    float v3;
    Vec3 a1a; 

    MATH_BlendVector(&a1a, a2, a3, 0.5);
    v3 = xmenSmoothBuffer_flt_9684BC;
    MATH_BlendVector(dest, dest, &a1a, xmenSmoothBuffer_flt_9684BC);
    MATH_BlendVector(&a1a, a2 + 1, a3 + 1, 0.5);
    MATH_BlendVector(dest + 1, dest + 1, &a1a, v3);
}

void GAO_ModifierXMEN_SMOOTH_BUFFER(Vec3* buff, unsigned int passNum)
{
    Vec3* v2;
    unsigned int v3;
    int v5;
    Vec3* v7;
    Vec3* v10;
    Vec3* v16;
    Vec3* v17;
    Vec3* v18;
    Vec3* v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v27;
    float v28;
    float v29;
    float v30;
    Vec3* a2a;

    v2 = buff;
    v3 = passNum;
    v5 = 2 * (passNum >> 1);
    a2a = &buff[2 * (32 - passNum)];
    MATH_BlendVector(&a2a[v5], buff, &a2a[0], 0.5f);
    MATH_BlendVector(&a2a[v5 + 1], v2 + 1, a2a + 1, 0.5f);
    if (v2 < a2a)
    {
        v7 = &v2[v5];
        v10 = &v2[2 * passNum];
        while (1)
        {
            // 2db blendvector inline a whileban
            v7[0].x = v10[-2 * passNum].x * 0.5f + v10[0].x * 0.5f;
            v7[0].y = v10[-2 * passNum].y * 0.5f + v10[0].y * 0.5f;
            v7[0].z = v10[-2 * passNum].z * 0.5f + v10[0].z * 0.5f;
            v7[1].x = v10[-2 * passNum + 1].x * 0.5f + v10[1].x * 0.5f;
            v7[1].y = v10[-2 * passNum + 1].y * 0.5f + v10[1].y * 0.5f;
            v7[1].z = v10[-2 * passNum + 1].z * 0.5f + v10[1].z * 0.5f;
            v10 += 2 * passNum;
            v7 += 2 * passNum;
            if (&v10[-2 * passNum] >= a2a)
                break;
        }
        v3 = passNum;
        v5 = 2 * (passNum >> 1);
    }
    GAO_ModifierXMEN_SMOOTH_BUFFER_BendVectors_sub_4235A0(v2, &a2a[v5], &v2[v5]);
    v16 = &v2[2 * v3];
    if (v16 <= a2a)
    {
        v17 = v16;
        v18 = &v16[v5];
        v19 = &v16[-v5];
        do
        {
            v29 = v19[0].y * 0.5f + v18[0].y * 0.5f;// 4db blendvector inline elvileg a whileban
            v20 = v19[0].z * 0.5f + v18[0].z * 0.5f;
            v21 = xmenSmoothBuffer_flt_9684BC;
            if (xmenSmoothBuffer_flt_9684BC < 0.0)
                v21 = 0.0;
            if (v21 > 1.0)
                v21 = 1.0f;
            v27 = v19[0].x * 0.5f + v18[0].x * 0.5f;
            v16[0].x = (1.0f - v21) * v16[0].x + v27 * v21;
            v17[0].y = (1.0f - v21) * v17[0].y + v29 * v21;
            v17[0].z = (1.0f - v21) * v17[0].z + v20 * v21;
            v28 = v19[1].x * 0.5f + v18[1].x * 0.5f;
            v30 = v19[1].y * 0.5f + v18[1].y * 0.5f;
            v22 = v19[1].z * 0.5f + v18[1].z * 0.5f;
            v23 = xmenSmoothBuffer_flt_9684BC;
            if (xmenSmoothBuffer_flt_9684BC < 0.0)
                v23 = 0.0;
            if (v23 > 1.0)
                v23 = 1.0;
            v17[1].x = v28 * v23 + (1.0f - v23) * v17[1].x;
            v17[1].y = v30 * v23 + (1.0f - v23) * v17[1].y;
            v17[1].z = v22 * v23 + (1.0f - v23) * v17[1].z;
            v16 += 2 * v3;
            v18 += 2 * v3;
            v19 += 2 * v3;
            v17 += 2 * v3;
        } while (v16 <= a2a);
    }
}

void GAO_ModifierXMEN_DRAW(tXmenMdfStruct* xmen)
{
    int ebx1;
    float st7_6;
    int num2;
    tXmenSize* currSize;
    tXmenSize* v5;
    int v20;
    Vec3* v21;
    Vec3* v22;
    Vec3* v23;
    int v24;
    unsigned int v25;
    int i;
    Vec3* v30;
    int v31;
    float v33;
    Vec3* v34;
    Vec3* v35;
    int v36;
    Vec3* v37;
    Vec3* v38;
    Vec3* v39;
    float v40;
    float v41;
    float v42;
    float v43;
    Vec3* v45;
    int v46;
    Mtx4x4* v54;
    int v55;
    int v56;
    int v57;
    int v58;
    int v59;
    BOOL v60;
    Vec3* v61;
    Vec3* v62;
    int v63;
    Vec3* v64;
    Vec3* v65;
    int v66;
    Vec3* v67; 
    unsigned int v68;
    Vec3* v69; 
    int v70;
    int v71;
    __int16 v72;
    tGeoTriangleChunkElem* v73;
    unsigned int v77;
    __int16 v78;
    int num2_1;
    float v84;
    int v85;
    int v86;
    Vec3* currSmoothedPoint;
    Vec3* v88;
    int v89;
    float v90;
    float v91;
    float v93;
    int v94;
    Vec3* j;
    float v96;
    float v97;
    float v99;
    float v100;
    Vec3* v101;
    float v102;
    Vec3* v103;
    tGeoUVElem* v104;
    float v105;
    float v106;
    float v108;
    int v109;
    tGeoUVElem* v110;
    Vec3* k; 
    float v112;
    float v113;
    float v115;
    float v116;
    float v117;
    float v119;
    int v120;
    tGeoUVElem* v121;
    Vec3* l;
    float v123;
    float v124;
    float v126;
    Vec3* v1;
    tXmenSize* v1a;
    Vec3* v1b;
    int v1c;
    int v2a;
    Vec3* v2b;
    Vec3 posRet;
    Vec3 mtxVecRetByType;
    unsigned int v137;
    int v138;
    int v139;
    Vec3 dest;
    int v147;
    int v148;
    int idx;
    Vec3* v149a;
    Vec3* v138a;
    Vec3* v137a;

    ebx1 = xmen->num;
    idx = ebx1;
    if (ebx1)
    {
        if (xmen->frameCounter != g_ulFrameCounter - 1)
        {
            xmen->num2 = 0;
            xmen->time = 9.9999998e12f;
        }
        if (xmen->deathTime < 0.00050000002f)
            xmen->deathTime = 0.00050000002f;
        st7_6 = TIM_gf_dt + xmen->time;
        xmen->time = st7_6;
        if (st7_6 >= xmen->deathTime)
            xmen->time = 0.0;
        else
            xmen->num2 = ((unsigned __int8)xmen->num2 - 1) & 7;
        num2 = xmen->num2;
        currSize = xmen->sizes;
        v5 = &currSize[ebx1];
        if (v5 > currSize)
        {
            while (1)
            {
                --idx;
                v1 = &xmen->ptr1[idx + (!num2 ? 1 : 0)].data[2 * (num2 - 1)];
                --v5;
                if (xmen->xmenType == 3)
                {
                    MATH_BlendVector(&dest, &v1[0], &v1[1], 0.5f);
                    GAO_ModifierXMEN_GetBonePosFromNum(xmen->gao, v5->multipartDataID, &posRet, &mtxVecRetByType, 3u, &dest);
                    xmen->ptr1[idx].data[2 * num2].x = posRet.x + mtxVecRetByType.x * 0.0f;
                    xmen->ptr1[idx].data[2 * num2].y = posRet.y + mtxVecRetByType.y * 0.0f;
                    xmen->ptr1[idx].data[2 * num2].z = posRet.z + mtxVecRetByType.z * 0.0f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].x = posRet.x + mtxVecRetByType.x * 0.0f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].y = posRet.y + mtxVecRetByType.y * 0.0f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].z = posRet.z + mtxVecRetByType.z * 0.0f;
                }
                else if ((xmen->flags & 0x10) != 0)
                {
                    MATH_BlendVector(&dest, &v1[0], &v1[1], 0.5f);
                    GAO_ModifierXMEN_GetBonePosFromNum(xmen->gao, v5->multipartDataID, &posRet, &mtxVecRetByType, xmen->xmenType, &dest);
                    xmen->ptr1[idx].data[2 * num2].x = posRet.x + mtxVecRetByType.x * v5->size * -0.5f;
                    xmen->ptr1[idx].data[2 * num2].y = posRet.y + mtxVecRetByType.y * v5->size * -0.5f;
                    xmen->ptr1[idx].data[2 * num2].z = posRet.z + mtxVecRetByType.z * v5->size * -0.5f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].x = posRet.x + mtxVecRetByType.x * v5->size * 0.5f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].y = posRet.y + mtxVecRetByType.y * v5->size * 0.5f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].z = posRet.z + mtxVecRetByType.z * v5->size * 0.5f;
                }
                else
                {
                    dest.x = v1->x;
                    dest.z = v1->z;
                    dest.y = v1->y;
                    GAO_ModifierXMEN_GetBonePosFromNum(xmen->gao, v5->multipartDataID, &posRet, &mtxVecRetByType, xmen->xmenType, &dest);
                    xmen->ptr1[idx].data[2 * num2].x = posRet.x + mtxVecRetByType.x * 0.0f;
                    xmen->ptr1[idx].data[2 * num2].y = posRet.y + mtxVecRetByType.y * 0.0f;
                    xmen->ptr1[idx].data[2 * num2].z = posRet.z + mtxVecRetByType.z * 0.0f;
                    xmen->ptr1[idx].data[(2 * num2) + 1].x = posRet.x + mtxVecRetByType.x * v5->size;
                    xmen->ptr1[idx].data[(2 * num2) + 1].y = posRet.y + mtxVecRetByType.y * v5->size;
                    xmen->ptr1[idx].data[(2 * num2) + 1].z = posRet.z + mtxVecRetByType.z * v5->size;
                }
                if (v5 <= xmen->sizes)
                    break;
            }
        }
        v20 = g_ulFrameCounter;
        xmen->num2 = ((unsigned __int8)xmen->num2 + 1) & 7;
        if (xmen->frameCounter != v20 - 1)
        {
            v21 = xmen->ptr1->data;
            v22 = &v21[16 * xmen->num];
            xmen->num2 = 0;
            xmen->time = 9.9999998e12f;
            while (v21 < v22)
            {
                v23 = v21;
                v21 += 2;
                memcpy(v23, xmen->ptr1, 24u);
            }
        }
        if (xmen->xmenType == 3)
        {
            v24 = xmen->num;
            if (v24)
            {
                v25 = v24;
                for (i = 24 * (xmen->num2 + 8 * v24); ; i = v137)
                {
                    v2a = v24 - 1;
                    v137 = i - 192;
                    --v25;
                    v1a = &xmen->sizes[v24 - 1];
                    v149a = (Vec3*)((char *)&xmen->ptr1[-1] + i);
                    v30 = xmen->ptr1[v25].data;
                    v31 = 8;
                    do
                    {
                        if ((xmen->flags & 0x10) != 0)
                        {
                            v30[0].x = v30[1].x * 0.5f + v30[0].x * 0.5f;
                            v30[0].y = v30[1].y * 0.5f + v30[0].y * 0.5f;
                            v30[0].z = v30[1].z * 0.5f + v30[0].z * 0.5f;
                        }
                        v30 += 2;
                        --v31;
                    } while (v31);
                    posRet.x = gpst_DX_stDD->ct.cam.objectMtx.v3.x;
                    v33 = gpst_DX_stDD->ct.cam.objectMtx.v3.z;
                    v34 = xmen->ptr1[v25].data;
                    posRet.y = gpst_DX_stDD->ct.cam.objectMtx.v3.y;
                    v35 = v149a;
                    posRet.z = v33;
                    v36 = 8;
                    v37 = &xmen->ptr1[v25].data[14];
                    v38 = v34;
                    do
                    {
                        --v36;
                        if (v35 == v34)
                        {
                            if (v36)
                                v39 = &v38[2];
                            else
                                v39 = xmen->ptr1[v25].data;
                            v40 = v39->x - v34->x;
                            v41 = v39->y - v38[0].y;
                            v42 = v39->z - v38[0].z;
                        }
                        else
                        {
                            v40 = v34->x - v37->x;
                            v41 = v38[0].y - v37->y;
                            v42 = v38[0].z - v37->z;
                        }
                        v38[1].x = posRet.y * v42 - posRet.z * v41;
                        v38[1].y = posRet.z * v40 - v42 * posRet.x;
                        v38[1].z = v41 * posRet.x - posRet.y * v40;
                        if (fabs(v38[1].x) != 0.0f || fabs(v38[1].y) != 0.0f || fabs(v38[1].z) != 0.0f)
                        {
                            v43 = (float)sqrt(v38[1].z * v38[1].z + v38[1].y * v38[1].y + v38[1].x * v38[1].x);
                            v38[1].x = 1.0f / v43 * v38[1].x;
                            v38[1].y = 1.0f / v43 * v38[1].y;
                            v38[1].z = 1.0f / v43 * v38[1].z;
                        }
                        else
                        {
                            v38[1].x = 0.0f;
                            v38[1].y = 0.0f;
                            v38[1].z = 0.0f;
                        }
                        v37 = v34;
                        v34 += 2;
                        v38 += 2;
                    } while (v36);
                    v45 = xmen->ptr1[v25].data;
                    v46 = 8;
                    do
                    {
                        if ((xmen->flags & 0x10) != 0)
                        {
                            mtxVecRetByType.x = v45[0].x;
                            mtxVecRetByType.y = v45[0].y;
                            mtxVecRetByType.z = v45[0].z;
                            v45[0].x = mtxVecRetByType.x + v1a->size * -0.5f * v45[1].x;
                            v45[0].y = mtxVecRetByType.y + v1a->size * -0.5f * v45[1].y;
                            v45[0].z = mtxVecRetByType.z + v1a->size * -0.5f * v45[1].z;
                            v45[1].x = mtxVecRetByType.x + v1a->size * 0.5f * v45[1].x;
                            v45[1].y = mtxVecRetByType.y + v1a->size * 0.5f * v45[1].y;
                            v45[1].z = mtxVecRetByType.z + v1a->size * 0.5f * v45[1].z;
                        }
                        else
                        {
                            v45[1].x = v45[0].x + v1a->size * v45[1].x;
                            v45[1].y = v45[0].y + v1a->size * v45[1].y;
                            v45[1].z = v45[0].z + v1a->size * v45[1].z;
                        }
                        v45 += 2;
                        --v46;
                    } while (v46);
                    v24 = v2a;
                    if (!v2a)
                        break;
                }
            }
        }
        if ((xmen->flags & 2) != 0)
        {
            SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
        }
        else
        {
            SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
            v54 = OBJ_pst_GetAbsoluteMatrix(xmen->gao);
            SOFT_l_MatrixStack_Push(&gpst_DX_stDD->matrixStack, v54);
        }
        gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
        gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
        v55 = xmen->num;
        if (v55)
        {
            v56 = 96 * v55;
            v57 = 192 * v55;
            while (1)
            {
                v139 = v55 - 1;
                memset(XMEN_OBJECT_TRIS, 0, sizeof(XMEN_OBJECT_TRIS));
                memset(XMEN_OBJECT_Points_Smoothed, 0, sizeof(XMEN_OBJECT_Points_Smoothed));
                v58 = v57 - 192;
                v59 = v56 - 96;
                v60 = (xmen->flags & 1) == 0;
                v147 = v58;
                v148 = v59;
                if (v60)
                {
                    v69 = (Vec3*)((char*)xmen->ptr1->data + v59);
                    XMEN_ELEMENT[0].triangleNum = 14;
                    v70 = 0;
                    XMEN_OBJECT.vertexNum = 16;
                    XMEN_OBJECT.uvNum = 16;
                }
                else
                {
                    v61 = (Vec3*)((char*)xmen->ptr1->data + v58);
                    v62 = XMEN_OBJECT_Points_Smoothed;
                    v63 = 8;
                    do
                    {
                        v64 = v61;
                        v65 = v62;
                        v61 += 2;
                        v62 += 8;
                        --v63;
                        memcpy(v65, v64, 24u);
                    } while (v63);
                    v66 = xmen->num2;
                    v67 = &XMEN_OBJECT_Points_Smoothed[8 * v66];
                    v138a = (Vec3*)((char *)&xmen->ptr1[v139] + 24 * v66);
                    v137 = 2;
                    v2b = v67 - 8;
                    v1b = (Vec3*)((int)v138a - 24);
                    if (!v66)
                    {
                        v2b = v67 + 56;
                        v1b += 16;
                    }
                    do
                    {
                        v68 = v137;
                        GAO_ModifierXMEN_SMOOTH_BUFFER(XMEN_OBJECT_Points_Smoothed, 1 << v137);
                        v137 = --v68;
                        memcpy(v67, v138a, 24u);
                        memcpy(v2b, v1b, 24u);
                    } while (v68);
                    v69 = XMEN_OBJECT_Points_Smoothed;
                    XMEN_ELEMENT[0].triangleNum = 56;
                    v70 = 2;
                    XMEN_OBJECT.vertexNum = 64;
                    XMEN_OBJECT.uvNum = 64;
                }
                XMEN_OBJECT.vertices = v69;
                gpst_DX_stDD->verticesCurrent = v69;
                v71 = 0;
                v137 = 1 << (v70 + 3);
                v72 = v137 - 1;
                v138 = (1 << v70) - 1;
                XMEN_ELEMENT[0].materialNum = 0;
                v73 = XMEN_OBJECT_TRIS;
                if (v138 < xmen->num2 << v70)
                {
                    v1c = (1 << v70) - 1;
                    do
                    {
                        v73[0].v1 = 2 * v71;
                        v73[0].uv1 = 2 * v71;
                        v73[0].v2 = 2 * v71 + 1;
                        v73[0].uv2 = 2 * v71 + 1;
                        v73[0].v3 = 2 * v72 + 1;
                        v73[0].uv3 = 2 * v72 + 1;
                        v73[1].v1 = 2 * v71;
                        v73[1].uv1 = 2 * v71;
                        v73[1].v2 = 2 * v72 + 1;
                        v73[1].uv2 = 2 * v72 + 1;
                        v73[1].v3 = 2 * v72;
                        v73[1].uv3 = 2 * v72;
                        v73 += 2;
                        v72 = v71++;
                        ++v1c;
                    } while (v1c < xmen->num2 << v70);
                }
                if (!v70 || xmen->num2)
                {
                    v77 = (1 << v70) + v71;
                    v78 = v77 - 1;
                }
                else
                {
                    v77 = 1;
                    v78 = 0;
                }
                if (v77 < v137)
                {
                    do
                    {
                        v73[0].v1 = 2 * v77;
                        v73[0].uv1 = 2 * v77;
                        v73[0].v2 = 2 * v77 + 1;
                        v73[0].uv2 = 2 * v77 + 1;
                        v73[0].v3 = 2 * v78 + 1;
                        v73[0].uv3 = 2 * v78 + 1;
                        v73[1].v1 = 2 * v77;
                        v73[1].uv1 = 2 * v77;
                        v73[1].v2 = 2 * v78 + 1;
                        v73[1].uv2 = 2 * v78 + 1;
                        v73[1].v3 = 2 * v78;
                        v73[1].uv3 = 2 * v78;
                        v73 += 2;
                        v78 = v77++;
                    } while (v77 < v137);
                }
                num2_1 = xmen->num2;
                v84 = 0.0;
                v85 = (2 * num2_1) << v70;
                v86 = (8 - num2_1) << v70;
                currSmoothedPoint = &XMEN_OBJECT_Points_Smoothed[v85];
                v88 = 0;
                v138 = v85;
                v137a = currSmoothedPoint;
                if (v86)
                {
                    v89 = (8 - num2_1) << v70;
                    do
                    {
                        if (v88)
                        {
                            v90 = v88->y - currSmoothedPoint->y;
                            v91 = v88->x - currSmoothedPoint->x;
                            v93 = v88->z - currSmoothedPoint->z;
                            v84 = v84 + (float)sqrt(v91 * v91 + v93 * v93 + v90 * v90);
                        }
                        v88 = currSmoothedPoint;
                        currSmoothedPoint += 2;
                        --v89;
                    } while (v89);
                    v85 = v138;
                }
                v94 = num2_1 << v70;
                for (j = XMEN_OBJECT_Points_Smoothed; v94; --v94)
                {
                    if (v88)
                    {
                        v96 = v88->y - j->y;
                        v97 = v88->x - j->x;
                        v99 = v88->z - j->z;
                        v84 = v84 + (float)sqrt(v97 * v97 + v99 * v99 + v96 * v96);
                    }
                    v88 = j;
                    j += 2;
                }
                if (v84 != 0.0)
                    v84 = 1.0f / v84;
                v100 = 0.0f;
                v101 = v137a;
                v102 = 0.0;
                v103 = NULL;
                v104 = &XMEN_UVs[v85];
                if ((xmen->flags & 0x20) != 0)
                {
                    for (; v86; )
                    {
                        if (v103)
                        {
                            v105 = v103->y - v101->y;
                            v106 = v103->x - v101->x;
                            v108 = v103->z - v101->z;
                            v102 = (float)sqrt(v106 * v106 + v108 * v108 + v105 * v105) * v84;
                        }
                        v100 = v100 + v102;
                        v103 = v101;
                        v104[0].v = 0.0f;
                        v104[0].u = v100;
                        v104[1].v = 1.0;
                        v104[1].u = v100;
                        v104 += 2;
                        v101 += 2;
                        --v86;
                    }
                    v109 = xmen->num2 << v70;
                    v110 = XMEN_UVs;
                    for (k = XMEN_OBJECT_Points_Smoothed; v109; )
                    {
                        if (v103)
                        {
                            v112 = v103->y - k->y;
                            v113 = v103->x - k->x;
                            v115 = v103->z - k->z;
                            v102 = (float)sqrt(v113 * v113 + v115 * v115 + v112 * v112) * v84;
                        }
                        v100 = v100 + v102;
                        v103 = k;
                        v110[0].v = 0.0f;
                        v110[0].u = v100;
                        v110[1].v = 1.0f;
                        v110[1].u = v100;
                        v110 += 2;
                        k += 2;
                        --v109;
                    }
                }
                else
                {
                    for (; v86; )
                    {
                        if (v103)
                        {
                            v116 = v103->y - v101->y;
                            v117 = v103->x - v101->x;
                            v119 = v103->z - v101->z;
                            v102 = (float)sqrt(v117 * v117 + v119 * v119 + v116 * v116) * v84;
                        }                        
                        v100 = v100 + v102;
                        v103 = v101;
                        v104[0].u = 0.0f;
                        v104[0].v = v100;
                        v104[1].u = 1.0f;
                        v104[1].v = v100;
                        v104 += 2;
                        v101 += 2;
                        --v86;
                    }
                    v120 = xmen->num2 << v70;
                    v121 = XMEN_UVs;
                    for (l = XMEN_OBJECT_Points_Smoothed; v120; )
                    {
                        if (v103)
                        {
                            v123 = v103->y - l->y;
                            v124 = v103->x - l->x;
                            v126 = v103->z - l->z;
                            v102 = (float)sqrt(v124 * v124 + v126 * v126 + v123 * v123) * v84;
                        }
                        v100 = v100 + v102;
                        v103 = l;
                        v121[0].u = 0.0f;
                        v121[0].v = v100;
                        v121[1].u = 1.0;
                        v121[1].v = v100;
                        v121 += 2;
                        l += 2;
                        --v120;
                    }
                }
                if ((xmen->flags & 4) != 0)
                    GAO_XMN_Render_TransparentTriangles(&XMEN_OBJECT, xmen->geomat);
                else
                    MAT_DrawIndexedTriangle(&XMEN_OBJECT, xmen->geomat, XMEN_ELEMENT);
                if (!v139)
                    break;
                v57 = v147;
                v56 = v148;
                v55 = v139;
            }
        }
        SOFT_MatrixStack_Reset(&gpst_DX_stDD->matrixStack, &gpst_DX_stDD->ct.cam.cameraMtx);
    }
}

void GAO_ModifierXMEN_DRAW_ALL()
{
    int v0;
    int v1;
    int v2;
    tXmenMdfStruct* v3;

    gpst_DX_stDD->MRMPtr = 0;
    gpst_DX_stDD->flags &= ~0x400;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x800;
    v0 = g_ulFrameCounter;
    gpst_DX_stDD->flags |= 2u;
    v1 = XMEN_STACK_PTR;
    v2 = v0 + 1;
    g_ulFrameCounter = v2;
    while (v1)
    {
        v3 = p_XMEN_STACK[--v1];
        XMEN_STACK_PTR = v1;
        if (v3)
        {
            if (v3->frameCounter != v2)
            {
                GAO_ModifierXMEN_DRAW(v3);
                v2 = g_ulFrameCounter;
                v1 = XMEN_STACK_PTR;
            }
            p_XMEN_STACK[v1]->frameCounter = v2;
            v1 = XMEN_STACK_PTR;
        }
    }
    XMEN_STACK_PTR = 0;
}

void GAO_ModifierXMEN_Unapply(struct sModifier* a1, void* unk1) {
}

void XMEN_OnOff(struct sGameObjectMain* a1, int a2, int onOff)
{
    BOOL v3;
    tModifier* i;
    tableElemStructPF* v5;
    tableElemStructPF* v6;
    tGroup* v7;
    tGameObjectMain* v8;
    int a3a;

    v7 = 0;
    v5 = 0;
    v6 = 0;
    if (onOff)
        a3a = 0;
    else
        a3a = 0x2000000;
    if (a1)
    {
        if (a1->basic
            && (a1->flags & 2) != 0
            && a1->basic->actorObjectOrVisualObject
            && ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            v7 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup;
            v6 = ((tableElemStructPF*)(v7->tablePF->nextElemPtr)) - 1;
        }
        while (v5 <= v6)
        {
            if (v5)
            {
                v8 = (tGameObjectMain*)v5->objectPtr;
            }
            else
            {
                v8 = a1;
                if (v6)
                    v5 = ((tableElemStructPF*)(v7->tablePF->firstElemPtr)) - 1;
            }
            v3 = !v8 || ((uint8_t)v8 & 3) == 1;
            if (!v3 && v8->extended && v8->extended->mdf)
            {
                for (i = v8->extended->mdf; i; i = i->nextElem)
                {
                    if (i->modifierIntf->mdfId == 9 && ((((tXmenMdfStruct*)i->mdfPrivateDataPtr))->id == a2 || a2 == -1))
                    {
                        i->modifierFlags &= ~0x2000000u;
                        i->modifierFlags |= a3a;
                    }
                }
            }
            ++v5;
        }
    }
}

void XMEN_SetSize(struct sGameObjectMain* a1, int id, int multipartDataID, float size)
{
    BOOL v4;
    int j;
    tXmenMdfStruct* v6;
    tModifier* i;
    tableElemStructPF* v8;
    tableElemStructPF* v9;
    tGroup* v10;
    tGameObjectMain* v11;

    v10 = 0;
    v8 = 0;
    v9 = 0;
    if (a1)
    {
        if (a1->basic
            && a1->basic->actorObjectOrVisualObject
            && (a1->flags & 2) != 0
            && ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup)
        {
            v10 = ((tGameObjectActorObject*)(a1->basic->actorObjectOrVisualObject))->skeletonGroup;
            v9 = ((tableElemStructPF*)(v10->tablePF->nextElemPtr)) - 1;
        }
        while (v8 <= v9)
        {
            if (v8)
            {
                v11 = (tGameObjectMain*)v8->objectPtr;
            }
            else
            {
                v11 = a1;
                if (v9)
                    v8 = ((tableElemStructPF*)(v10->tablePF->firstElemPtr)) - 1;
            }
            v4 = !v11 || ((uint8_t)v11 & 3) == 1;
            if (!v4 && v11->extended && v11->extended->mdf)
            {
                for (i = v11->extended->mdf; i; i = i->nextElem)
                {
                    if (i->modifierIntf->mdfId == 9)
                    {
                        v6 = (tXmenMdfStruct*)i->mdfPrivateDataPtr;
                        if (v6->id == id || id == -1)
                        {
                            for (j = 0; j < v6->num; ++j)
                            {
                                if (multipartDataID == -1 || v6->sizes[j].multipartDataID == multipartDataID)
                                    v6->sizes[j].size = size;
                            }
                        }
                    }
                }
            }
            ++v8;
        }
    }
}