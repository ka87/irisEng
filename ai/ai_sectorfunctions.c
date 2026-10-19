#include "../sectors.h"
#include "../collision.h"
#include "../engine.h"
#include "ai_debugfunctions.h"
#include "ai_sectorfunctions.h"

void AI_EvalFunc_GRPSectUpdateMe_C(tGameObjectMain* a2)
{
    int i;
    uint16_t sectorid;
    uint16_t sectorida;

    if ((a2->flags2 & 0x10000000) == 0)
    {
        for (i = 0; i < gi_NumSectors; ++i)
        {
            if (COL_BV_PivotCollide(gapst_AllSectors[i], a2, FALSE))
            {
                sectorid = a2->extended->sectorID;
                if (i == sectorid)
                {
                    a2->if_this_not0_wont_render_gao = gapst_AllSectors[i]->if_this_not0_wont_render_gao;
                }
                else
                {
                    if (sectorid != 0xFFFF && sectorid != -1)
                    {
                        gapst_AllObjSectors[(100 * sectorid) + gai_NumObjSectors[sectorid] - 1]->extended->sectorElemIdx = a2->extended->sectorElemIdx;
                        gapst_AllObjSectors[(100 * sectorid) + a2->extended->sectorElemIdx] = gapst_AllObjSectors[(100 * sectorid) + (+gai_NumObjSectors[sectorid]--) - 1];
                    }
                    gapst_AllObjSectors[(100 * i) + gai_NumObjSectors[i]] = a2;
                    a2->extended->sectorID = i;
                    a2->extended->sectorElemIdx = gai_NumObjSectors[i];
                    ++gai_NumObjSectors[i];
                    a2->if_this_not0_wont_render_gao = gapst_AllSectors[i]->if_this_not0_wont_render_gao;
                }
                return;
            }
        }
    }
    sectorida = a2->extended->sectorID;
    if (sectorida != 0xFFFF && sectorida != -1)
    {
        gapst_AllObjSectors[(100 * sectorida) + gai_NumObjSectors[sectorida] - 1]->extended->sectorElemIdx = a2->extended->sectorElemIdx;
        gapst_AllObjSectors[(100 * sectorida) + a2->extended->sectorElemIdx] = gapst_AllObjSectors[(100 * sectorida) + (gai_NumObjSectors[sectorida]--) - 1];

        a2->extended->sectorID = 0xffff; // -1
        a2->extended->sectorElemIdx = 0xffff; // -1
        a2->if_this_not0_wont_render_gao = 0;
    }
}

int AI_EvalFunc_GRPSectRegister_C(struct sGameObjectMain* a1)
{
    gapst_AllSectors[gi_NumSectors] = a1;
    gai_NumObjSectors[gi_NumSectors++] = 0;
    return gi_NumSectors - 1;
}

tAINode* AI_EvalFunc_GRPSectRegister(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    if (*(int32_t*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack])
    {
        v1 = AI_EvalFunc_GRPSectRegister_C(v3);
        AI_PutIntToStack(v1);
    }
    else
    {
        AI_PutIntToStack(-1);
    }
    return a1 + 1;
}

void AI_EvalFunc_GRPSectInvisibleSet_C(struct sGameObjectMain* a1, int a2, int a3)
{
    tGameObjectMain* v4;
    tGameObjectMain* v5;
    tGroup* v6;
    tableElemStructPF* v7;
    tableElemStructPF* v8;
    int i;

    if (a3 == -1)
    {
        a1->if_this_not0_wont_render_gao = 0;
    }
    else if (a3)
    {
        ++a1->if_this_not0_wont_render_gao;
    }
    else if (a1->if_this_not0_wont_render_gao)
    {
        --a1->if_this_not0_wont_render_gao;
    }
    if ((a1->flags & 0x800000) != 0 && a1->extended && a1->extended->grp)
    {
        v6 = a1->extended->grp;
        v7 = v6->tablePF->firstElemPtr;
        v8 = ((tableElemStructPF*)(v6->tablePF->nextElemPtr)) - 1;
        while (v7 <= v8)
        {
            v4 = (tGameObjectMain*)v7->objectPtr;
            if (v7->objectPtr && ((int)v7->objectPtr & 3) != 1)
            {
                if (a3 == -1)
                {
                    v4->if_this_not0_wont_render_gao = 0;
                }
                else if (a3)
                {
                    ++v4->if_this_not0_wont_render_gao;
                }
                else if (v4->if_this_not0_wont_render_gao)
                {
                    --v4->if_this_not0_wont_render_gao;
                }
            }
            ++v7;
        }
    }
    if (a2 != 0xFFFF && a2 != -1)
    {
        for (i = 0; a2 < gi_NumSectors && i < gai_NumObjSectors[a2]; ++i)
        {
            v5 = gapst_AllObjSectors[(100 * a2) + i];
            if (v5)
                v5->if_this_not0_wont_render_gao = a1->if_this_not0_wont_render_gao;
        }
    }
}

tAINode* AI_EvalFunc_GRPSectInvisibleSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_GRPSectInvisibleSet_C(v2, a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_GRPSectInactiveSet(tAINode* a1)
{
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_GetIntFromStack();
    AI_GetIntFromStack();
    AI_EvalFunc_DbgTraceInt_C();
    return a1 + 1;
}

void AI_EvalFunc_GRPSectSetIsIn_C(struct sGameObjectMain* a1)
{
    tGroup* v2;
    tableElemStructPF* v3;
    tableElemStructPF* v4;

    if ((a1->flags & 0x800000) != 0 && a1->extended && a1->extended->grp)
    {
        v2 = a1->extended->grp;
        v3 = v2->tablePF->firstElemPtr;
        v4 = ((tableElemStructPF*)(v2->tablePF->nextElemPtr)) - 1;
        while (v3 <= v4)
        {
            if (v3->objectPtr && ((int)v3->objectPtr & 3) != 1)
                ((tGameObjectMain*)v3->objectPtr)->flags2 |= 0x10000u;
            ++v3;
        }
    }
}

tAINode* AI_EvalFunc_GRPSectSetIsIn(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_GRPSectSetIsIn_C(v2);
    return a1 + 1;
}

// TODO FIXME: itt a decompile során eléggé elszáll a nagy stack miatt, megnézni ohgy jó-e így, lehet nem
void AI_EvalFunc_GRPCullDoor_C(tGameObjectMain* a2, tGameObjectMain** a3, int a4, int a5)
{
    double v6;
    Vec3 v7;
    Vec3 v8;
    int m;
    Vec3* v10;
    Vec3* v11;
    Vec3* v12;
    int v13;
    float v14;
    Vec3* v15 = NULL;
    Vec3* v16 = NULL;
    int v17;
    int l;
    float v19;
    Vec3* v20;
    Vec3 v21;
    int k;
    Vec3* v23;
    Vec3* v24;
    Vec3* v25;
    Vec3* v26;
    Vec3* v27;
    Vec3* v28;
    Vec3* v29;
    Vec3 v30[8];
    Vec3* v31;
    int j;
    Vec3* v33;
    Vec3* v34;
    Vec3* v35;
    Vec3* v36;
    Vec3* v37;
    Vec3* v38;
    Vec3* v39;
    Vec3* v40;
    Vec3 v41;
    Vec3 v42;
    Vec3 v43;
    Vec3 v44[17]; // TODO FIXME ennek nem kéne hogy array legyen
    int v45;
    int i;
    tGameObjectMain* v48;
    int v49;
    tableElemStructPF* v50;
    tableElemStructPF* v51;
    tGroup* v52;
    tDDCamStruct viewCtCopy;
    tDDContainerStruct* v54;
    tGdi_DisplayData* displayDataPtr;
    tWorld* v56;
    tGameObjectMain* sectorObjs[1000];
    tGameObjectMain* suppliedObjs[50];

    if ((a2->flags & 0x800000) != 0)
    {
        if (a2->extended)
        {
            if (a2->extended->grp)
            {
                v56 = WOR_gpst_CurrentWorld;
                displayDataPtr = WOR_gpst_CurrentWorld->viewPtr[a4].displayData;
                if (displayDataPtr)
                {

                    v56->viewPtr[a4].viewFunctionPtr(&v56->viewPtr[a4]);

                    v54 = &displayDataPtr->ct;

                    CAM_SetCameraMatrix(&displayDataPtr->ct, &v56->viewPtr[a4].cameraMtx);
                    CAM_Inverse(&displayDataPtr->ct);
                    CAM_Engine_ComputePlans(&displayDataPtr->ct);
                    OBJ_UpdateCullingVars(&displayDataPtr->ct.cam);
                    memcpy(&viewCtCopy, &displayDataPtr->ct, sizeof(viewCtCopy));
                    v52 = a2->extended->grp;
                    v51 = v52->tablePF->firstElemPtr;
                    v50 = ((tableElemStructPF*)(v52->tablePF->nextElemPtr)) - 1;
                    v49 = 0;
                    while (v51 <= v50)
                    {
                        v48 = (tGameObjectMain*)v51->objectPtr;
                        if (v48 && ((uint8_t)v48 & 3) != 1)
                            sectorObjs[v49++] = v48;
                        ++v51;
                    }
                    if (a5 != 0xFFFF && a5 != -1)
                    {
                        for (i = 0; i < gai_NumObjSectors[a5]; ++i)
                            sectorObjs[v49++] = gapst_AllObjSectors[100 * a5 + i];
                    }
                    v45 = 0;
                    while (*a3)
                    {
                        if (ENG_gb_ActiveSectorization && !OBJ_CullingObject(*a3) && ((*a3)->flags2 & 0x200000) == 0)
                            suppliedObjs[v45++] = *a3;
                        ++a3;
                    }
                    if (v45)
                    {
                        v40 = &v41;
                        v39 = &g_stGlobalNormPlaneLeft;
                        v41 = g_stGlobalNormPlaneLeft;
                        v38 = &v42;
                        v37 = &g_stGlobalNormPlaneUp;
                        v42 = g_stGlobalNormPlaneUp;
                        v36 = &v43;
                        v35 = &g_stGlobalNormPlaneRight;
                        v43 = g_stGlobalNormPlaneRight;
                        v34 = v44;
                        v33 = &g_stGlobalNormPlaneDown;
                        v44[0] = g_stGlobalNormPlaneDown;
                        for (j = 0; j < v45; ++j)
                        {
                            v31 = &suppliedObjs[j]->boundingVolume->BVMax;
                            v29 = v30;
                            v28 = v31;
                            v30[0] = *v31;
                            v27 = &suppliedObjs[j]->boundingVolume->BVMin;
                            v26 = &v30[1];
                            v25 = v27;
                            v30[1] = *v27;
                            v30[2].x = v30[0].x;
                            v30[2].y = v30[1].y;
                            v30[2].z = v30[0].z;
                            v30[3].x = v30[1].x;
                            v30[3].y = v30[1].y;
                            v30[3].z = v30[0].z;
                            v30[4].x = v30[1].x;
                            v30[4].y = v30[0].y;
                            v30[4].z = v30[0].z;
                            v30[5].x = v30[0].x;
                            v30[5].y = v30[0].y;
                            v30[5].z = v30[1].z;
                            v30[6].x = v30[0].x;
                            v30[6].y = v30[1].y;
                            v30[6].z = v30[1].z;
                            v30[7].x = v30[1].x;
                            v30[7].y = v30[0].y;
                            v30[7].z = v30[1].z;
                            for (i = 0; i < 8; ++i)
                            {
                                v24 = &suppliedObjs[j]->matrices->Pos;
                                v23 = &v30[i];
                                v23->x = v23->x + v24->x;
                                v23->y = v23->y + v24->y;
                                v23->z = v23->z + v24->z;
                            }
                            for (k = 0; k < 8; ++k)
                            {
                                MATH_SubstractVec(&v21, &v30[k], &v54->cam.objectMtx.Pos);
                                MATH_NormalizeVector(&v21, &v21);
                                v20 = &v54->cam.objectMtx.v3;
                                v19 = v54->cam.objectMtx.v3.z * v21.z
                                    + v54->cam.objectMtx.v3.y * v21.y
                                    + v21.x * v54->cam.objectMtx.v3.x;
                                if (v19 < 0.30000001)
                                    return;
                            }
                            for (l = 0; l < 4; ++l)
                            {
                                v17 = l;
                                switch (l)
                                {
                                case 0:
                                    v16 = &v41;
                                    v15 = &viewCtCopy.planeLeft;
                                    break;
                                case 1:
                                    v16 = &v42;
                                    v15 = &viewCtCopy.planeUp;
                                    break;
                                case 2:
                                    v16 = &v43;
                                    v15 = &viewCtCopy.planeRight;
                                    break;
                                case 3:
                                    v16 = v44;
                                    v15 = &viewCtCopy.planeDown;
                                    break;
                                default:
                                    break;
                                }
                                v14 = 10.0;
                                v13 = -1;
                                for (k = 0; k < 8; ++k)
                                {
                                    MATH_SubstractVec(&v21, &v30[k], &v54->cam.objectMtx.Pos);
                                    v19 = v16->z * v21.z + v16->y * v21.y + v21.x * v16->x;
                                    v6 = MATH_VecLen(&v21);
                                    v19 = (float)(v19 / v6);
                                    if (v19 < 0.0)
                                    {
                                        v13 = -1;
                                        break;
                                    }
                                    if (v19 < (double)v14)
                                    {
                                        v14 = v19;
                                        v13 = k;
                                    }
                                }
                                if (v13 == -1)
                                {
                                    v11 = v15;
                                    v10 = v16;
                                    v15->x = v16->x;
                                    v11->y = v10->y;
                                    v11->z = v10->z;
                                }
                                else
                                {
                                    MATH_SubstractVec(&v21, &v30[v13], &v54->cam.objectMtx.Pos);
                                    MATH_NormalizeEqualVector(&v21);
                                    if (l && l != 2)
                                        MATH_CrossProduct(v15, &v21, &v54->cam.objectMtx.v1);
                                    else
                                        MATH_CrossProduct(v15, &v21, &v54->cam.objectMtx.v2);
                                    if (!l || l == 3)
                                    {
                                        v12 = v15;
                                        v15->x = -v15->x;
                                        v12->y = -v15->y;
                                        v12->z = -v15->z;
                                    }
                                }
                                MATH_TransformVector(v15, &v54->cam.cameraMtx, v15);
                            }
                            viewCtCopy.planeDown.x = v54->cam.planeDown.x;
                            viewCtCopy.planeUp.x = v54->cam.planeUp.x;
                            viewCtCopy.planeLeft.y = v54->cam.planeLeft.y;
                            viewCtCopy.planeRight.y = v54->cam.planeRight.y;
                            OBJ_UpdateCullingVars(&viewCtCopy);
                            for (m = 0; m < v49; ++m)
                            {
                                if (sectorObjs[m])
                                {
                                    MATH_SubstractVec(&v8, &sectorObjs[m]->matrices->Pos, &suppliedObjs[j]->matrices->Pos);
                                    MATH_NormalizeEqualVector(&v8);
                                    MATH_SubstractVec(&v7, &sectorObjs[m]->matrices->Pos, &v54->cam.objectMtx.Pos);
                                    MATH_NormalizeEqualVector(&v7);
                                    if (!OBJ_CullingObject(sectorObjs[m]))
                                    {
                                        if (sectorObjs[m]->if_this_not0_wont_render_gao)
                                            --sectorObjs[m]->if_this_not0_wont_render_gao;
                                        sectorObjs[m] = 0;
                                    }
                                }
                            }
                        }
                    }
                    for (i = 0; i < v49; ++i)
                    {
                        if (sectorObjs[i])
                            ++sectorObjs[i]->if_this_not0_wont_render_gao;
                    }
                }
            }
        }
    }
}

tAINode* AI_EvalFunc_GRPCullDoor(tAINode* a1)
{
    tGameObjectMain* v2;
    char dest[52];
    int a4;
    int a5;
    tVarStackElem v6;
    tGameObjectMain* a2;

    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    AI_GetVarFromStack(dest, &v6);
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = v2;
    AI_EvalFunc_GRPCullDoor_C(v2, (tGameObjectMain**)v6.ptr, a4, a5);
    return a1 + 1;
}


int AI_EvalFunc_GRPFillArray_C(tGameObjectMain* a1, tGameObjectMain** a2)
{
    tGroup* v4;
    tableElemStructPF* v5;
    tableElemStructPF* v6;
    int v7;

    if ((a1->flags & 0x800000) == 0)
        return 0;
    if (!a1->extended || !a1->extended->grp)
        return 0;
    v4 = a1->extended->grp;
    v5 = v4->tablePF->firstElemPtr;
    v6 = ((tableElemStructPF*)(v4->tablePF->nextElemPtr)) - 1;
    v7 = 0;
    while (v5 <= v6)
    {
        if (v5->objectPtr && ((int)v5->objectPtr & 3) != 1)
            a2[v7++] = (tGameObjectMain*)v5->objectPtr;
        ++v5;
    }
    return v7;
}

tAINode* AI_EvalFunc_GRPFillArray(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    char dest[52];
    tGameObjectMain** v5;
    tVarStackElem a2;
    tGameObjectMain* a1a;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1a = v3;
    AI_GetVarFromStack(dest, &a2);
    v5 = (tGameObjectMain**)a2.ptr;
    v1 = AI_EvalFunc_GRPFillArray_C(a1a, (tGameObjectMain**)a2.ptr);
    AI_PutIntToStack(v1);
    return a1 + 1;
}