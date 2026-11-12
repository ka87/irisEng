#include "../memory.h"
#include "../modifiers/stpmodifier.h"
#include "../graphicsobjects/particle.h"
#include "../time.h"
#include "gfxdesintegrate.h"

void GFX_Desintegrate_Init(tGfxDesintegrateData* data)
{
    tGfxDesintegrateData* v1;
    tGEOGRO* v2;
    int triangleNum;
    tPAGStruct* v4;
    int v5;
    tPAGStruct* v6;
    tGEOGRO* v7;
    Vec3* v8;
    Vec3* v9;
    tModifier* v10;
    tStpMdfStruct* v11;
    int v12;
    Vec3* v13;
    Vec3* i; 
    tGEOGRO* v15;
    Vec3* v16; 
    Vec3* v17; 
    int v18; 
    int v19; 
    tGeoTriangleChunks* v20;
    tGeoTriangleChunkElem* v21;
    tPAGElemStruct* v22;
    int v23;
    Vec3* v24;
    Vec3* v25;
    float v26;
    float v27;
    float v28;
    float v29;
    long double v30; 
    Vec3* v31;
    double v32;
    BOOL v33; 
    int v34; 
    tPAGElemStruct* v35;
    Vec3* a1a;
    tGEOGRO* v37; 
    float v38;
    float v39;
    int v40; 
    unsigned int v41; 
    tGeoTriangleChunks* v42;
    int v43; 
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

    v1 = data;
    v2 = data->geogro;
    data->flags |= 1u;
    v37 = v2;
    triangleNum = GEO_l_GetNumberOfTriangles(v2);
    data->triangleNum = triangleNum;
    data->pointsNum = 3 * triangleNum;
    data->triangleChunkCount = v2->triangleChunkCount;
    v4 = PAG_pst_Create();
    v5 = data->triangleNum;
    data->pagPtr = v4;
    v4->triangleNum = v5;
    data->pagPtr->triangleNum2 = data->pagPtr->triangleNum;
    data->pagPtr->pagElems = (tPAGElemStruct*)MEM_p_Alloc(sizeof(tPAGElemStruct) * data->pagPtr->triangleNum);
    memset(data->pagPtr->pagElems, 0, sizeof(tPAGElemStruct) * data->pagPtr->triangleNum);
    data->pagPtr->flags |= 0x2000u;
    data->pagPtr->flags |= 0x4000u;
    data->pagPtr->v1 = data->v1;
    data->pagPtr->f1 = data->dword1C;
    if (data->f1 != 10000.0f)
    {
        v6 = data->pagPtr;
        data->flags |= 2u;
        v6->flags |= 0x400u;
        data->pagPtr->i3 = data->dword20;
        data->pagPtr->ff = data->f1;
    }
    v7 = GEO_pst_Create(data->pointsNum, 0, data->triangleChunkCount, 0);
    data->geogro = v7;
    v7->uvNum = v2->uvNum;
    data->geogro->UVs = v2->UVs;
    v8 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v2->vertexNum);
    v9 = v2->vertices;
    a1a = v8;
    v10 = MDF_pst_GetByType(data->gao, 21);
    if (v10)
    {
        v11 = (tStpMdfStruct*)v10->mdfPrivateDataPtr;
        if (v11->hasStp)
            v9 = v11->verticesStored;
    }
    v12 = v2->vertexNum;
    v13 = a1a;
    for (i = &v9[v12]; v9 < i; ++v13)
        MATH_TransformVertex(v13, &data->mtx, v9++);
    v15 = data->geogro;
    v16 = v15->vertices;
    v17 = v15->normals;
    v18 = 0;
    v35 = data->pagPtr->pagElems;
    v40 = 0;
    v43 = 0;
    if (data->triangleChunkCount > 0)
    {
        v34 = 0;
        do
        {
            v19 = v37->triangleChunks[v18].triangleNum;
            if (v19)
            {
                v20 = &(v1->geogro->triangleChunks[v18]);
                v20->triangleNum = v19;
                v42 = v20;
                v20->materialNum = v37->triangleChunks[v18].materialNum;
                v20->MRMUVIndicesNum = 0;
                GEO_AllocElementContent(v20);
                memcpy(v20->triangleChunkElems, &(v37->triangleChunks->triangleChunkElems[v34]), sizeof(tGeoTriangleChunkElem) * v20->triangleNum);
                v21 = v20->triangleChunkElems;
                v41 = 0;
                if (v20->triangleNum > 0)
                {
                    v22 = v35;
                    do
                    {
                        v23 = v21->v1;
                        v16->x = a1a[v21->v1].x;
                        v16->y = a1a[v23].y;
                        v16->z = a1a[v23].z;
                        v16[1] = a1a[v21->v2];
                        v24 = v16 + 1;
                        v25 = &a1a[v21->v3];
                        v24[1].x = v25->x;
                        v24[1].y = v25->y;
                        ++v24;
                        v24->z = v25->z;
                        v16 = v24 + 1;
                        v53 = v16[-3].z + v16[-2].z;
                        v46 = v16[-3].x + v16[-2].x + v16[-1].x;
                        v47 = (float)(v46 * 0.33333334);
                        v50 = (float)((v16[-3].y + v16[-2].y + v16[-1].y) * 0.33333334);
                        v54 = (float)((v53 + v16[-1].z) * 0.33333334);
                        if ((data->flags & 2) != 0)
                        {
                            v22->particlePos.x = v47;
                            v26 = v54;
                            v22->particlePos.y = v50;
                        }
                        else
                        {
                            v26 = 0.0;
                            v22->particlePos.x = 0.0;
                            v22->particlePos.y = 0.0;
                        }
                        v22->particlePos.z = v26;
                        v48 = v47 - data->mtx.Pos.x;
                        v51 = v50 - data->mtx.Pos.y;
                        v55 = v54 - data->mtx.Pos.z;
                        if (fabs(v48) != 0 || fabs(v51) != 0 || fabs(v55) != 0)
                        {
                            v30 = 1.0 / sqrt(v55 * v55 + v51 * v51 + v48 * v48);
                            v49 = (float)(v48 * v30);
                            v27 = v49;
                            v52 = (float)(v51 * v30);
                            v28 = v52;
                            v56 = (float)(v55 * v30);
                            v29 = v56;
                        }
                        else
                        {
                            v27 = 0.0;
                            v28 = 0.0;
                            v29 = 0.0;
                            v49 = 0.0;
                            v52 = 0.0;
                            v56 = 0.0;
                        }
                        v17->y = v28;
                        v17->x = v27;
                        v17->z = v29;
                        v17[1].y = v28;
                        v17[1].x = v27;
                        v17[1].z = v29;
                        v31 = v17 + 2;
                        v31->y = v28;
                        v31->x = v27;
                        v31->z = v29;
                        v17 = v31 + 1;
                        v44 = data->rand1Start;
                        v38 = data->rand1End;
                        v32 = (double)rand() * 0.000030518509 * (v44 - v38) + v38;
                        v22->v1.x = (float)(v49 * v32);
                        v22->v1.y = (float)(v52 * v32);
                        v22->v1.z = (float)(v56 * v32);
                        v45 = data->rand2Start;
                        v39 = data->rand2End;
                        ++v21;
                        ++v35;
                        v22->ff2 = (float)((double)rand() * 0.000030518509 * (v45 - v39) + v39);
                        v21[-1].v1 = v40;
                        v21[-1].v2 = v40 + 1;
                        v21[-1].v3 = v40 + 2;
                        v40 += 3;
                        ++v41;
                        ++v22;
                    } while (v41 < v42->triangleNum);
                }
                v1 = data;
                v18 = v34;
            }
            else
            {
                memset(&(v1->geogro->triangleChunks[v18]), 0, sizeof(tGeoTriangleChunks));
            }
            v18++;
            v33 = ++v43 < v1->triangleChunkCount;
            v34 = v18;
        } while (v33);
    }
    MEM_Free(a1a);
}

struct sGfxDesintegrateData* GFX_Desintegrate_Create(struct sGameObjectMain* a1)
{
    tGEOGRO* v1;
    tGfxDesintegrateData* v2;
    Mtx4x4* v3;

    v1 = (tGEOGRO*)OBJ_p_GetGro(a1);
    if (!v1 || v1->intf->id != 1)
        return 0;
    v2 = (tGfxDesintegrateData*)MEM_p_Alloc(sizeof(tGfxDesintegrateData));
    v2->flags = 0;
    v3 = OBJ_pst_GetAbsoluteMatrix(a1);
    if (&v2->mtx != v3)
        memcpy(&v2->mtx, v3, sizeof(v2->mtx));
    v2->gao = a1;
    v2->geogro = v1;
    v2->geomat = OBJ_p_GetGrm(a1);
    v2->timeElapsed = 0.0;
    v2->v1.x = 0.0;
    v2->v1.y = 0.0;
    v2->rand1End = 1.0;
    v2->dword1C = 1.0;
    v2->rand2Start = 5.0f;
    v2->v1.z = -0.5f;
    v2->rand1Start = 2.0f;
    v2->f1 = 10000.0f;
    return v2;
}

void GFX_Desintegrate_Destroy(struct sGfxDesintegrateData* a1)
{
    if (a1)
    {
        a1->geogro->uvNum = 0;
        a1->geogro->UVs = 0;
        GEO_Free((tGroGeneric*)(a1->geogro));
        PAG_Free((tGroGeneric*)(a1->pagPtr));
    }
}

int GFX_i_Desintegrate_Render(struct sGfxDesintegrateData* a1)
{
    double v1;
    tPAGStruct* v3;
    Vec3* v4;
    tPAGElemStruct* v5;
    tPAGElemStruct* v6;
    tPAGElemStruct* v7;
    unsigned int v8;
    double v9;
    Vec3* v10;
    tPAGElemStruct* v11;
    tPAGElemStruct* v12;
    unsigned int v13;
    Vec3* v14;
    Vec3* v15;
    double v16;
    tPAGElemStruct* v17;
    unsigned int v18;
    Vec3* v19;
    Vec3* v20;
    Vec3* v21;
    double v22;
    tGameObjectMain* v23;
    tGameObjectVisualObject* v24;
    tGEOGRO* v25;

    if (!a1)
        return 0;
    v1 = TIM_gf_dt + a1->timeElapsed;
    a1->timeElapsed = (float)v1;
    if (v1 > a1->rand2Start)
        return 0;
    if ((a1->flags & 1) == 0)
        GFX_Desintegrate_Init(a1);
    v3 = a1->pagPtr;
    v4 = a1->geogro->vertices;
    v5 = v3->pagElems;
    v6 = &v5[v3->triangleNum2];
    if ((a1->flags & 2) != 0 && v5 < v6)
    {
        v7 = v5;
        v8 = (sizeof(tPAGElemStruct) * v3->triangleNum2 - 1) / sizeof(tPAGElemStruct) + 1;
        do
        {
            v9 = v4->x - v7->particlePos.x;
            v4 += 3;
            ++v7;
            --v8;
            v4[-3].x = (float)v9;
            v4[-3].y = v4[-3].y - v7[-1].particlePos.y;
            v4[-3].z = v4[-3].z - v7[-1].particlePos.z;
            v4[-2].x = v4[-2].x - v7[-1].particlePos.x;
            v4[-2].y = v4[-2].y - v7[-1].particlePos.y;
            v4[-2].z = v4[-2].z - v7[-1].particlePos.z;
            v4[-1].x = v4[-1].x - v7[-1].particlePos.x;
            v4[-1].y = v4[-1].y - v7[-1].particlePos.y;
            v4[-1].z = v4[-1].z - v7[-1].particlePos.z;
        } while (v8);
    }
    PAG_Update(0, a1->pagPtr, TIM_gf_dt);
    v10 = a1->geogro->vertices;
    v11 = a1->pagPtr->pagElems;
    if ((a1->flags & 2) != 0)
    {
        if (v11 < v6)
        {
            v12 = v11;
            v13 = ((char*)v6 - (char*)v11 - 1) / sizeof(tPAGElemStruct) + 1;
            do
            {
                if (v12->ff2 >= 0.0)
                {
                    v16 = v10->x;
                    v15 = v10 + 2;
                    v15[-2].x = (float)(v16 + v12->particlePos.x);
                    v15[-2].y = v15[-2].y + v12->particlePos.y;
                    v15[-2].z = v15[-2].z + v12->particlePos.z;
                    v15[-1].x = v15[-1].x + v12->particlePos.x;
                    v15[-1].y = v15[-1].y + v12->particlePos.y;
                    v15[-1].z = v15[-1].z + v12->particlePos.z;
                    v15->x = v15->x + v12->particlePos.x;
                    v15->y = v15->y + v12->particlePos.y;
                    v15->z = v15->z + v12->particlePos.z;
                }
                else
                {
                    v10->x = 0.0;
                    v10->y = 0.0;
                    v10->z = 0.0;
                    v14 = v10 + 1;
                    v14->x = 0.0;
                    v14->y = 0.0;
                    v14->z = 0.0;
                    v15 = v14 + 1;
                    v15->x = 0.0;
                    v15->y = 0.0;
                    v15->z = 0.0;
                }
                v10 = v15 + 1;
                ++v12;
                --v13;
            } while (v13);
        }
    }
    else if (v11 < v6)
    {
        v17 = v11;
        v18 = ((char*)v6 - (char*)v11 - 1) / sizeof(tPAGElemStruct) + 1;
        do
        {
            if (v17->ff2 >= 0.0)
            {
                v21 = v10 + 1;
                v22 = v17->particlePos.x + v21[-1].x;
                v20 = v21 + 1;
                v20[-2].x = (float)v22;
                v20[-2].y = v20[-2].y + v17->particlePos.y;
                v20[-2].z = v20[-2].z + v17->particlePos.z;
                v20[-1].x = v17->particlePos.x + v20[-1].x;
                v20[-1].y = v20[-1].y + v17->particlePos.y;
                v20[-1].z = v20[-1].z + v17->particlePos.z;
                v20->x = v17->particlePos.x + v20->x;
                v20->y = v20->y + v17->particlePos.y;
                v20->z = v20->z + v17->particlePos.z;
            }
            else
            {
                v10->x = 0.0;
                v10->y = 0.0;
                v10->z = 0.0;
                v19 = v10 + 1;
                v19->x = 0.0;
                v19->y = 0.0;
                v19->z = 0.0;
                v20 = v19 + 1;
                v20->x = 0.0;
                v20->y = 0.0;
                v20->z = 0.0;
            }
            v17->particlePos.x = 0.0;
            v17->particlePos.y = 0.0;
            v17->particlePos.z = 0.0;
            v10 = v20 + 1;
            ++v17;
            --v18;
        } while (v18);
    }
    GFX_NeedGeom(4u, 4u, 4u, 0);
    v23 = GFX_gpst_GO;
    v24 = (tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject);
    v25 = (tGEOGRO*)(v24->GROGEOOrSomethingElse);
    v24->GROGEOOrSomethingElse = (tGroGeneric*)(a1->geogro);
    ((tGameObjectVisualObject*)(v23->basic->actorObjectOrVisualObject))->GROMAT = (tGroGeneric*)(a1->geomat);
    ((tGameObjectVisualObject*)(v23->basic->actorObjectOrVisualObject))->objDrawMask &= ~0x800u;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x800u;
    a1->geogro->intf->Render(GFX_gpst_GO);
    ((tGameObjectVisualObject*)(v23->basic->actorObjectOrVisualObject))->objDrawMask |= 0x800u;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent |= 0x800u;
    ((tGameObjectVisualObject*)(v23->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = (tGroGeneric*)v25;
    return 1;
}