#include "../memory.h"
#include "../loading.h"
#include "../time.h"
# include "explodemodifier.h"

void GAO_ModifierExplode_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tGroGeneric* v2;
    tGEOGRO* v3;
    tExplodeMdfStruct* v4;
    int v5;
    tGEOGRO* v6;
    Vec3* v7;
    Vec3* v8;
    int v9;
    tGeoTriangleChunks* v10;
    int* v11;
    tGeoTriangleChunkElem* v12;
    unsigned int v13;
    Vec3* v14;
    Vec3* v15;
    Vec3* v16;
    Vec3* v17;
    float v18;
    float v19;
    float v20;
    long double v21;
    Vec3* v22;
    BOOL v23;
    int v24; 
    int i; 
    tExplodeMdfStruct* v26;
    tGEOGRO* v27;
    int v28;
    tGeoTriangleChunks* v29;
    int v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;

    a2->modifierFlags |= 0x1000000u;
    v2 = OBJ_p_GetGro(a1);
    if (v2 && v2->intf->id == 1)
    {
        v3 = (tGEOGRO*)v2;
        v27 = (tGEOGRO*)v2;
        v4 = (tExplodeMdfStruct*)MEM_p_Alloc(sizeof(tExplodeMdfStruct));
        v26 = v4;
        v5 = GEO_l_GetNumberOfTriangles(v3);
        v4->trinagleNum = v5;
        v5 *= 3;
        v4->vertexNum = v5;
        v24 = v3->triangleChunkCount;
        v4->triangleChunkNum = v24;
        v4->f1 = 0.0;
        v4->f3 = -0.5;
        v4->f2 = 1.0;
        v6 = GEO_pst_Create(v5, 0, v24, 0);
        v4->geogroNew = v6;
        v6->uvNum = v3->uvNum;
        v4->geogroNew->UVs = v3->UVs;
        v28 = 0;
        v7 = v4->geogroNew->vertices;
        GEO_UseNormals(v4->geogroNew);
        v8 = v4->geogroNew->normals;
        v9 = 0;
        v30 = 0;
        if (v4->triangleChunkNum > 0)
        {
            for (i = 0; ; v9 = i * 32)
            {
                v10 = &(v4->geogroNew->triangleChunks[v9]);
                v10->triangleNum = v3->triangleChunks[v9].triangleNum;
                v29 = v10;
                v10->materialNum = v3->triangleChunks[v9].materialNum;
                v10->MRMUVIndicesNum = 0;
                GEO_AllocElementContent(v10);
                memcpy(v10->triangleChunkElems, v3->triangleChunks[i].triangleChunkElems, 4 * ((unsigned int)(sizeof(tGeoTriangleChunkElem) * v10->triangleNum) >> 2));
                v11 = &v10->triangleNum;
                v12 = v10->triangleChunkElems;
                v13 = 0;
                if (*v11 > 0)
                {
                    do
                    {
                        v14 = &v27->vertices[v12->v1];
                        v7->x = v14->x;
                        v7->y = v14->y;
                        v7->z = v14->z;
                        v15 = &v27->vertices[v12->v2];
                        v7[1].x = v15->x;
                        v7[1].y = v15->y;
                        v7[1].z = v15->z;
                        v16 = v7 + 1;
                        v17 = &v27->vertices[v12->v3];
                        v16[1].x = v17->x;
                        v16[1].y = v17->y;
                        ++v16;
                        v16->z = v17->z;
                        v7 = v16 + 1;
                        v35 = v7[-3].z + v7[-2].z;
                        v31 = v7[-3].x + v7[-2].x + v7[-1].x;
                        v33 = v7[-3].y + v7[-2].y + v7[-1].y;
                        v36 = v35 + v7[-1].z;
                        if (fabs(v31) != 0 || fabs(v7[-3].y + v7[-2].y + v7[-1].y) || fabs(v36) != 0)
                        {
                            v21 = 1.0 / sqrt(v36 * v36 + v33 * v33 + v31 * v31);
                            v32 = (float)(v31 * v21);
                            v18 = v32;
                            v34 = (float)(v33 * v21);
                            v19 = v34;
                            v37 = (float)(v36 * v21);
                            v20 = v37;
                        }
                        else
                        {
                            v18 = 0.0;
                            v19 = 0.0;
                            v20 = 0.0;
                        }
                        v8->y = v19;
                        v8->x = v18;
                        v8->z = v20;
                        v8[1].y = v19;
                        v22 = v8 + 1;
                        v22->x = v18;
                        v22->z = v20;
                        ++v22;
                        v22->y = v19;
                        v22->x = v18;
                        v22->z = v20;
                        v8 = v22 + 1;
                        v12->v1 = v28;
                        v12->v2 = v28 + 1;
                        v12->v3 = v28 + 2;
                        v28 += 3;
                        ++v13;
                        ++v12;
                    } while (v13 < v29->triangleNum);
                }
                v23 = v30 + 1 < v26->triangleChunkNum;
                ++i;
                v3 = v27;
                ++v30;
                v4 = v26;
                if (!v23)
                    break;
            }
        }
        v4->gaogeogro = v3;
        a2->mdfPrivateDataPtr = v4;
    }
    else
    {
        a2->mdfPrivateDataPtr = 0;
    }
}

void GAO_ModifierExplode_Apply(struct sModifier* a1, void *unk1)
{
    tExplodeMdfStruct* v1;
    tGameObjectMain* v2;
    Vec3* v3;
    Vec3* v4;
    Vec3* v5;
    double v6;
    double v7;
    double v8;
    double v9;
    float v10;
    float i;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float a1a;

    v1 = (tExplodeMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1)
    {
        v2 = gpst_DX_stDD->currTargetGao;
        v1->gaogeogro = (tGEOGRO*)(((tGameObjectVisualObject*)(v2->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
        ((tGEOGRO*)((tGameObjectVisualObject*)(v2->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) = v1->geogroNew;
        v3 = v1->geogroNew->vertices;
        v4 = &v3[v1->geogroNew->vertexNum];
        GEO_UseNormals(v1->geogroNew);
        v5 = v1->geogroNew->normals;
        a1a = v1->f1;
        v10 = a1a * a1a * v1->f3;
        v6 = TIM_gf_dt + a1a;
        for (i = (float)(v6 * v6 * v1->f3); v3 < v4; v3[-1].z = (float)(v9 + v3[-1].z))
        {
            ++v3;
            v7 = a1a * v5->x;
            ++v5;
            v18 = a1a * v5[-1].z;
            v20 = (float)(v7 + v3[-1].x);
            v19 = (float)(v6 * v5[-1].z);
            v12 = (float)(v6 * v5[-1].x + v3[-1].x);
            v17 = v19 + v3[-1].z + i;
            v13 = v12 - v20;
            v15 = (float)(v3[-1].y + v5[-1].y * v6 - (v3[-1].y + v5[-1].y * a1a));
            v8 = v1->f2;
            v14 = (float)(v13 * v8);
            v16 = (float)(v15 * v8);
            v9 = (v17 - (v18 + v3[-1].z + v10)) * v8;
            v3[-1].x = v14 + v3[-1].x;
            v3[-1].y = v16 + v3[-1].y;
        }
        v1->f1 = (float)v6;
    }
}

void GAO_ModifierExplode_Unapply(struct sModifier* a1, void *unk1)
{
    tExplodeMdfStruct* v1;

    v1 = (tExplodeMdfStruct*)a1->mdfPrivateDataPtr;
    if (v1)
        ((tGEOGRO*)((tGameObjectVisualObject*)(gpst_DX_stDD->currTargetGao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse) = v1->gaogeogro;
}

int GAO_ModifierExplode_Load(struct sModifier* mdf, char* bufferPtr)
{
    char* v2;

    v2 = bufferPtr;
    mdf->modifierFlags |= 0x1000000u;
    bufferPtr = v2;
    LOA_ReadLong(&bufferPtr, 0, 2);
    return bufferPtr - v2;
}

void GAO_ModifierExplode_Reinit(struct sModifier* a1)
{
    tExplodeMdfStruct* v1;
    tGEOGRO* v2;
    int v3;
    Vec3* v4;
    tGeoTriangleChunks* v5;
    int v6;
    tGeoTriangleChunkElem* v7;
    unsigned int v8;
    Vec3* v9;
    int a1a;

    v1 = (tExplodeMdfStruct*)a1->mdfPrivateDataPtr;
    v2 = v1->geogroNew;
    v3 = 0;
    v1->f1 = 0.0;
    v4 = v2->vertices;
    for (a1a = 0; a1a < v1->triangleChunkNum; ++a1a)
    {
        v5 = v1->gaogeogro->triangleChunks;
        v6 = v5[v3].triangleNum;
        v7 = v5[v3].triangleChunkElems;
        v8 = 0;
        if (v6 > 0)
        {
            do
            {
                *v4 = v1->gaogeogro->vertices[v7->v1];
                v4[1] = v1->gaogeogro->vertices[v7->v2];
                v9 = v4 + 2;
                *v9 = v1->gaogeogro->vertices[v7->v3];
                v4 = v9 + 1;
                ++v8;
                ++v7;
            } while (v8 < v1->gaogeogro->triangleChunks[v3].triangleNum);
        }
        ++v3;
    }
}

void GAO_ModifierExplode_Destroy(struct sModifier* a1)
{
}