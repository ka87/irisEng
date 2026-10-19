#include "../memory.h"
#include "../loading.h"
#include "../gameobject.h"
#include "../graphicsobjects/staticlod.h"
#include "morphmodifier.h"
struct sMorphMdfStruct* GEO_pst_ModifierMorphing_Get(struct sGameObjectMain* a1)
{
    tModifier* v1;
    tMorphMdfStruct* result;

    v1 = MDF_pst_GetByType(a1, 4); // morph
    if (!v1 || (result = (tMorphMdfStruct*)v1->mdfPrivateDataPtr) == 0)
        result = 0;
    return result;
}

void GEO_ModifierMorphing_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3a)
{
    tMorphMdfStruct* v3;
    tMorphMdfStruct* v4;
    tGameObjectBasic* v5;
    tGEOGRO* v6;
    int v7;
    unsigned int v8;
    tMorphMdfInnerStruct* v9;
    int v10;
    int v11;
    unsigned int v12;
    int v13;
    unsigned int v14;
    tMorphMdfInner2Struct* v15;
    int v16;
    tMorphMdfInner2Struct* v17;
    unsigned int v18;
    int v19;
    int a2a;
    int a2b;
    struct sMorphMdfStruct* a3 = (struct sMorphMdfStruct*)a3a;

    v3 = a3;
    v4 = (tMorphMdfStruct*)MEM_p_Alloc(sizeof(tMorphMdfStruct));
    a2->mdfPrivateDataPtr = v4;
    if (a3)
    {
        memcpy(v4, a3, sizeof(tMorphMdfStruct));
        v7 = v4->num;
        if (v7)
        {
            v8 = sizeof(tMorphMdfInnerStruct) * v7;
            v9 = (tMorphMdfInnerStruct*)MEM_p_Alloc(sizeof(tMorphMdfInnerStruct) * v7);
            v4->ptr = v9;
            memcpy(v9, a3->ptr, v8);
            a2a = 0;
            if (v4->num > 0)
            {
                v10 = 0;
                do
                {
                    v11 = v4->ptr[v10].num;
                    v19 = v11;
                    if (v11)
                    {
                        v12 = 4 * v11;
                        v4->ptr[v10].ptr1 = (int*)MEM_p_Alloc(4 * v11);
                        memcpy(v4->ptr[v10].ptr1, a3->ptr[v10].ptr1, v12);
                        v4->ptr[v10].ptr2 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v19);
                        memcpy(v4->ptr[v10].ptr2, a3->ptr[v10].ptr2, 4 * ((unsigned int)(sizeof(Vec3) * v19) >> 2));
                    }
                    ++v10;
                    ++a2a;
                } while (a2a < v4->num);
                v3 = a3;
            }
        }
        v13 = v4->num2;
        if (v13)
        {
            v14 = sizeof(tMorphMdfInner2Struct) * v13;
            v15 = (tMorphMdfInner2Struct*)MEM_p_Alloc(sizeof(tMorphMdfInner2Struct) * v13);
            v4->ptr2 = v15;
            memcpy(v15, v3->ptr2, v14);
            v16 = 0;
            for (a2b = 0; a2b < v4->num2; ++a2b)
            {
                v17 = v4->ptr2;
                v18 = 4 * v17[v16].num;
                if (v18)
                {
                    v4->ptr2[v16].ptr = (int*)MEM_p_Alloc(4 * v17[v16].num);
                    memcpy(v4->ptr2[v16].ptr, a3->ptr2[v16].ptr, v18);
                }
                ++v16;
            }
        }
    }
    else
    {
        memset(v4, 0, sizeof(tMorphMdfStruct));
        v5 = a1->basic;
        if (v5->actorObjectOrVisualObject)
        {
            v6 = (tGEOGRO*)(((tGameObjectVisualObject*)(v5->actorObjectOrVisualObject))->GROGEOOrSomethingElse);
            if (v6)
            {
                if (v6->intf->id == 1)
                    v4->i2 = v6->vertexNum;
            }
        }
    }
}

void GEO_ModifierMorphing_Destroy(struct sModifier* a1)
{
    tMorphMdfStruct* v1;
    int v2;
    int v3;
    int v4;
    tMorphMdfInnerStruct* v5;
    int v6;
    int v7;
    int v8;
    tMorphMdfInner2Struct* v9;
    int v10;
    tMorphMdfInner2Struct* v11;

    v1 = (tMorphMdfStruct*)a1->mdfPrivateDataPtr;
    v2 = v1->num;
    if (v2)
    {
        v3 = 0;
        if (v2 > 0)
        {
            v4 = 0;
            do
            {
                v5 = &v1->ptr[v4];
                if (v5->num)
                {
                    MEM_Free(v5->ptr1);
                    MEM_Free(v1->ptr[v4].ptr2);
                }
                ++v3;
                ++v4;
            } while (v3 < v1->num);
        }
        MEM_Free(v1->ptr);
    }
    v6 = v1->num2;
    if (v6)
    {
        v7 = 0;
        if (v6 > 0)
        {
            v8 = 0;
            do
            {
                v9 = v1->ptr2;
                v10 = v9[v8].num;
                v11 = &v9[v8];
                if (v10)
                    MEM_Free(v11->ptr);
                ++v7;
                ++v8;
            } while (v7 < v1->num2);
        }
        MEM_Free(v1->ptr2);
    }
    MEM_Free(v1);
}

void GEO_ModifierMorphing_UnApply(struct sModifier* a1, struct sGEOGRO* geogro) {
}

void GEO_ModifierMorphing_Apply(struct sModifier* a1, struct sGEOGRO* geogro)
{
    tMorphMdfStruct* v2;
    tGEOGRO* v3;
    Vec3* v4;
    int v5;
    tMorphMdfInner2Struct* v6;
    int v7;
    int* v8;
    tMorphMdfStruct* v9;
    int v10;
    int i;
    double v12;
    double v13;
    tMorphMdfInnerStruct* v14;
    int v15;
    int v16;
    Vec3* v17;
    Vec3* v18;
    tMorphMdfInnerStruct* v19;
    Vec3* v20;
    int v21;
    int v22;
    Vec3* v23;
    double v24;
    Vec3* v25;
    Vec3* v26;
    int v27;
    Vec3* v28;
    double v29;
    BOOL v30;
    int v31;
    float a3a;
    float a3;
    tMorphMdfStruct* v34;
    Vec3* v35;
    tMorphMdfInner2Struct* v36;
    int v37;
    Vec3 a1a;
    float j;
    float v40;
    tGEOSTATICLOD* statlod;
    
    v2 = (tMorphMdfStruct*)a1->mdfPrivateDataPtr;
    v34 = v2;
    if (v2->num2)
    {
        if (v2->i2)
        {
            v3 = geogro;
            v4 = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
            v35 = v4;
            if (geogro)
            {
                if (geogro->intf->id == 8)            // staticlod
                {
                    statlod = (tGEOSTATICLOD*)geogro;
                    if (statlod->distance[0] >= statlod->currDistance && statlod->lodNum > 1u)
                        return;
                    v3 = statlod->geo[0];
                }
                if (v3->intf->id == 1)
                {
                    QueryPerformanceCounter(&PerfCounterStruct[52]);
                    v5 = gpst_DX_stDD->flags;
                    if ((v5 & 0x400) == 0)
                    {
                        gpst_DX_stDD->flags = v5 | 0x400;
                        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
                        memcpy(v4, v3->vertices, sizeof(Vec3) * v3->vertexNum);
                    }
                    v6 = v2->ptr2;
                    v36 = v6;
                    v37 = 0;
                    if (v2->num2 > 0)
                    {
                        while (1)
                        {
                            v7 = v6->num;
                            if (v6->num)
                            {
                                if (v6->Factor != 0.0 && v6->Prog != 0.0)
                                    break;
                            }
                        LABEL_48:
                            ++v6;
                            v30 = ++v37 < v34->num2;
                            v36 = v6;
                            if (!v30)
                                goto LABEL_49;
                        }
                        v8 = v6->ptr;
                        v9 = v34;
                        v10 = (unsigned int)v6->Prog;
                        v31 = v10;
                        for (i = v10; i < v7; ++i)
                        {
                            if ((unsigned int)v8[i] < (unsigned int)v34->num)
                                break;
                        }
                        if (i < v7)
                        {
                            v31 = --v10;
                            if (v10 >= 0)
                            {
                                do
                                {
                                    if ((unsigned int)v8[v10] < (unsigned int)v34->num)
                                        break;
                                    --v10;
                                } while (v10 >= 0);
                                v31 = v10;
                            }
                            v12 = (v6->Prog - (double)v31 - 1.0) / (double)(i - v10);
                            j = (float)v12;
                            if (v12 <= 0.0)
                                goto LABEL_38;
                        }
                        else
                        {
                            i = v7 - 1;
                            for (j = 1.0; i > 0; --i)
                            {
                                if ((unsigned int)v8[i] < (unsigned int)v34->num)
                                    break;
                            }
                            if ((unsigned int)v8[i] >= (unsigned int)v34->num)
                                goto LABEL_48;
                            v9 = v34;
                        }
                        v13 = j * v6->Factor;
                        v14 = &v9->ptr[v8[i]];
                        v15 = 0;
                        v16 = v14->num;
                        if (v13 == 1.0)
                        {
                            if (v16 > 0)
                            {
                                v17 = v14->ptr2;
                                do
                                    MATH_VecAddVecWithoutBuffer(&v35[v14->ptr1[v15++]], v17++);
                                while (v15 < v14->num);
                            LABEL_37:
                                v10 = v31;
                                v6 = v36;
                                goto LABEL_38;
                            }
                        }
                        else if (v16 > 0)
                        {
                            v18 = v14->ptr2;
                            do
                            {
                                a3a = (float)v13;
                                MATH_VecMultScalar(&a1a, v18, a3a);
                                MATH_VecAddVecWithoutBuffer(&v35[v14->ptr1[v15++]], &a1a);
                                ++v18;
                            } while (v15 < v14->num);
                            goto LABEL_37;
                        }
                    LABEL_38:
                        v40 = (float)(1.0 - j);
                        if (v10 >= 0 && v40 > 0.0)
                        {
                            a3 = v40 * v6->Factor;
                            v19 = &v34->ptr[v8[v10]];
                            v20 = v19->ptr2;
                            v21 = 0;
                            v22 = v19->num;
                            if (a3 == 1.0)
                            {
                                if (v22 > 0)
                                {
                                    v23 = v20;
                                    do
                                    {
                                        v24 = v23->x + v35[v19->ptr1[v21]].x;
                                        v25 = &v35[v19->ptr1[v21++]];
                                        v25->x = (float)v24;
                                        v25->y = v23->y + v25->y;
                                        v25->z = v25->z + v23->z;
                                        ++v23;
                                    } while (v21 < v19->num);
                                }
                            }
                            else if (v22 > 0)
                            {
                                v26 = v20;
                                do
                                {
                                    v27 = v19->ptr1[v21];
                                    a1a.x = a3 * v26->x;
                                    v28 = &v35[v27];
                                    v29 = v26->y * a3;
                                    a1a.y = (float)v29;
                                    a1a.z = a3 * v26->z;
                                    v28->x = a1a.x + v28->x;
                                    v28->y = a1a.y + v28->y;
                                    v28->z = a1a.z + v28->z;
                                    ++v21;
                                    ++v26;
                                } while (v21 < v19->num);
                            }
                        }
                        goto LABEL_48;
                    }
                LABEL_49:
                    PRO_QPC_RecordTimeDecRef(52);
                }
            }
        }
    }
}

int GEO_ul_ModifierMorphing_Load(struct sModifier* a1, char* a2)
{
    tMorphMdfStruct* v2;
    tMorphMdfInnerStruct* v3;
    tMorphMdfInner2Struct* v4;
    tMorphMdfInnerStruct* v5;
    int v6;
    int* v7;
    BOOL v8;
    Vec3* v9;
    int v10;
    tMorphMdfInner2Struct* v11;
    int* v12;
    int v13;
    int i;
    int j;
    char* v17;

    v2 = (tMorphMdfStruct*)a1->mdfPrivateDataPtr;
    v17 = a2;
    LOA_ReadLong(&v17, 0, 1);
    v2->i1 = LOA_ReadLong(&v17, 0, 1);
    v2->i2 = LOA_ReadLong(&v17, 0, 1);
    v2->num = LOA_ReadLong(&v17, 0, 1);
    v2->num2 = LOA_ReadLong(&v17, 0, 1);
    if (sizeof(tMorphMdfInnerStruct) * v2->num)
        v3 = (tMorphMdfInnerStruct*)MEM_p_Alloc(sizeof(tMorphMdfInnerStruct) * v2->num);
    else
        v3 = 0;
    v2->ptr = v3;
    if (sizeof(tMorphMdfInner2Struct) * v2->num2)
        v4 = (tMorphMdfInner2Struct*)MEM_p_Alloc(sizeof(tMorphMdfInner2Struct) * v2->num2);
    else
        v4 = 0;
    v5 = v2->ptr;
    v2->ptr2 = v4;
    for (i = 0; i < v2->num; ++i)
    {
        v6 = 0;
        v5->num = LOA_ReadLong(&v17, 0, 1);
        LOA_ReadString(&v17, 0, 64, 2);
        if (v5->num)
        {
            v7 = (int*)MEM_p_Alloc(4 * v5->num);
            v8 = v5->num <= 0;
            v5->ptr1 = v7;
            if (!v8)
            {
                do
                {
                    *v7 = LOA_ReadLong(&v17, 0, 1);
                    ++v6;
                    ++v7;
                } while (v6 < v5->num);
            }
            v9 = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * v5->num);
            v10 = 0;
            v8 = v5->num <= 0;
            v5->ptr2 = v9;
            if (!v8)
            {
                do
                {
                    LOA_ReadVector(&v17, v9, 1);
                    ++v10;
                    ++v9;
                } while (v10 < v5->num);
            }
        }
        else
        {
            v5->ptr1 = 0;
            v5->ptr2 = 0;
        }
        ++v5;
    }
    v11 = v2->ptr2;
    for (j = 0; j < v2->num2; ++j)
    {
        v11->num = LOA_ReadLong(&v17, 0, 1);
        v11->Factor = LOA_ReadFloat(&v17, 0, 1);
        v11->Prog = LOA_ReadFloat(&v17, 0, 1);
        LOA_ReadString(&v17, 0, 64, 2);
        if (v11->num)
        {
            v12 = (int*)MEM_p_Alloc(4 * v11->num);
            v13 = 0;
            v8 = v11->num <= 0;
            v11->ptr = v12;
            if (!v8)
            {
                do
                {
                    *v12 = LOA_ReadLong(&v17, 0, 1);
                    ++v13;
                    ++v12;
                } while (v13 < v11->num);
            }
        }
        ++v11;
    }
    return v17 - a2;
}