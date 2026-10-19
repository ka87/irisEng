#include "memory.h"
#include "gameobject.h"
#include "graphicsobjects/geo.h"
#include "collision.h" // there is no ok3.h yet

int Face_Plane(Vec3* a1, Vec3* a2, Vec3* a3)
{
    int v3; // edx

    v3 = a1->x > (double)a3->x;
    if (a1->x < (double)a2->x)
        v3 |= 2u;
    if (a1->y > (double)a3->y)
        v3 |= 4u;
    if (a1->y < (double)a2->y)
        v3 |= 8u;
    if (a1->z > (double)a3->z)
        v3 |= 0x10u;
    if (a1->z < (double)a2->z)
        v3 |= 0x20u;
    return v3;
}

int bevel_2D(Vec3* a1, Vec3* a2, Vec3* a3)
{
    double v3; // st7
    int v4; // ecx
    double v5; // st7
    double v6; // st6
    double v7; // st5
    double v8; // st7
    float v10; // [esp+0h] [ebp-24h]
    float v11; // [esp+4h] [ebp-20h]
    float v12; // [esp+8h] [ebp-1Ch]
    float v13; // [esp+Ch] [ebp-18h]
    float v14; // [esp+Ch] [ebp-18h]
    float v15; // [esp+10h] [ebp-14h]
    float v16; // [esp+14h] [ebp-10h]
    float v17; // [esp+18h] [ebp-Ch]
    float v18; // [esp+1Ch] [ebp-8h]
    float v19; // [esp+30h] [ebp+Ch]

    v16 = a3->z - a2->z;
    v13 = (float)((a3->x - a2->x) * 0.5);
    v14 = v13 + a2->x;
    v15 = (float)((a3->y - a2->y) * 0.5 + a2->y);
    v3 = v16 * 0.5 + a2->z;
    v10 = a1->x - v14;
    v11 = a1->y - v15;
    v4 = 0;
    v12 = (float)(a1->z - v3);
    v17 = a3->x - v14;
    v18 = a3->y - v15;
    v5 = a3->z - v3;
    v19 = v18 + v17;
    if (v11 + v10 > v19)
        v4 = 1;
    if (v10 - v11 > v19)
        v4 |= 2u;
    if (v11 - v10 > v19)
        v4 |= 4u;
    v6 = -v10;
    if (v6 - v11 > v19)
        v4 |= 8u;
    v7 = v5 + v17;
    if (v12 + v10 > v7)
        v4 |= 0x10u;
    if (v10 - v12 > v7)
        v4 |= 0x20u;
    if (v12 - v10 > v7)
        v4 |= 0x40u;
    if (v6 - v12 > v7)
        v4 |= 0x80u;
    v8 = v5 + v18;
    if (v12 + v11 > v8)
        v4 |= 0x100u;
    if (v11 - v12 > v8)
        v4 |= 0x200u;
    if (v12 - v11 > v8)
        v4 |= 0x400u;
    if (-v11 - v12 > v8)
        v4 |= 0x800u;
    return v4;
}

int bevel_3D(Vec3* a1, Vec3* a2, Vec3* a3)
{
    double v3; // st7
    int v4; // ecx
    double v5; // st7
    double v6; // st6
    double v7; // st5
    float v9; // [esp+0h] [ebp-18h]
    float v10; // [esp+4h] [ebp-14h]
    float v11; // [esp+8h] [ebp-10h]
    float v12; // [esp+Ch] [ebp-Ch]
    float v13; // [esp+Ch] [ebp-Ch]
    float v14; // [esp+10h] [ebp-8h]
    float v15; // [esp+14h] [ebp-4h]

    v15 = a3->z - a2->z;
    v12 = (float)((a3->x - a2->x) * 0.5);
    v13 = v12 + a2->x;
    v14 = (float)((a3->y - a2->y) * 0.5 + a2->y);
    v3 = v15 * 0.5 + a2->z;
    v9 = a1->x - v13;
    v10 = a1->y - v14;
    v4 = 0;
    v11 = (float)(a1->z - v3);
    v5 = a3->z - v3 + a3->y - v14 + a3->x - v13;
    if (v11 + v10 + v9 > v5)
        v4 = 1;
    v6 = v10 + v9;
    if (v6 - v11 > v5)
        v4 |= 2u;
    v7 = v9 - v10;
    if (v11 + v7 > v5)
        v4 |= 4u;
    if (v7 - v11 > v5)
        v4 |= 8u;
    if (v11 - v9 + v10 > v5)
        v4 |= 0x10u;
    if (v10 - v9 - v11 > v5)
        v4 |= 0x20u;
    if (v11 - v6 > v5)
        v4 |= 0x40u;
    if (-v9 - v10 - v11 > v5)
        v4 |= 0x80u;
    return v4;
}

int check_point(Vec3* a1, Vec3* a2, float a3, int a4, Vec3* a5, Vec3* a6)
{
    Vec3 a1a; // [esp+0h] [ebp-Ch] BYREF

    a1a.x = (a2->x - a1->x) * a3 + a1->x;
    a1a.y = (a2->y - a1->y) * a3 + a1->y;
    a1a.z = (a2->z - a1->z) * a3 + a1->z;
    return a4 & Face_Plane(&a1a, a5, a6);
}

BOOL check_line(Vec3* a1, Vec3* a2, char a3, Vec3* a4, Vec3* a6)
{
    BOOL result; // eax
    float a3a; // [esp+0h] [ebp-20h]
    float a3b; // [esp+0h] [ebp-20h]
    float a3c; // [esp+0h] [ebp-20h]
    float a3d; // [esp+0h] [ebp-20h]
    float a3e; // [esp+0h] [ebp-20h]
    float a3f; // [esp+0h] [ebp-20h]

    result = 1;
    if ((a3 & 1) == 0 || (a3a = (a1->x - a6->x) / (a1->x - a2->x), check_point(a1, a2, a3a, 62, a4, a6)))
    {
        if ((a3 & 2) == 0 || (a3b = (a1->x - a4->x) / (a1->x - a2->x), check_point(a1, a2, a3b, 61, a4, a6)))
        {
            if ((a3 & 4) == 0 || (a3c = (a1->y - a6->y) / (a1->y - a2->y), check_point(a1, a2, a3c, 59, a4, a6)))
            {
                if ((a3 & 8) == 0 || (a3d = (a1->y - a4->y) / (a1->y - a2->y), check_point(a1, a2, a3d, 55, a4, a6)))
                {
                    if ((a3 & 0x10) == 0 || (a3e = (a1->z - a6->z) / (a1->z - a2->z), check_point(a1, a2, a3e, 47, a4, a6)))
                    {
                        if ((a3 & 0x20) == 0 || (a3f = (a1->z - a4->z) / (a1->z - a2->z), check_point(a1, a2, a3f, 31, a4, a6)))
                            result = 0;
                    }
                }
            }
        }
    }
    return result;
}

unsigned int COL_OK3_GetNumberOfFacesInBox(tCobOK3AABBoxElem* a1)
{
    int v1; // edx
    unsigned int result; // eax
    tCobOK3AABBoxElemChunkElem* v3; // ecx

    v1 = a1->num;
    result = 0;
    if (v1 > 0)
    {
        v3 = a1->chunkElems;
        do
        {
            result += v3->num;
            v3++;
            --v1;
        } while (v1);
    }
    return result;
}

void COL_pst_OK3_GetTriangle(void* aa, unsigned __int16 a2, unsigned __int16 a3, Vec3** a4, Vec3** a5, Vec3** a6, int type)
{
    if (type)
    {
        tCobMeshObjectMesh* a1 = (tCobMeshObjectMesh*)aa;
        *a4 = &a1->vertices[a1->meshChunkElems[a2].triangleElems[a3].v1];
        *a5 = &a1->vertices[a1->meshChunkElems[a2].triangleElems[a3].v2];
        *a6 = &a1->vertices[a1->meshChunkElems[a2].triangleElems[a3].v3];
    }
    else
    {
        tGEOGRO* a1 = (tGEOGRO*)aa;
        *a4 = &a1->vertices[a1->triangleChunks[a2].triangleChunkElems[a3].v1];
        *a5 = &a1->vertices[a1->triangleChunks[a2].triangleChunkElems[a3].v2];
        *a6 = &a1->vertices[a1->triangleChunks[a2].triangleChunkElems[a3].v3];
    }
}

void COL_OK3_OptimizeBoxBV(tCobOK3AABBoxElem* a1, void* a2, int type)
{
    int v4; // ecx
    double v5; // st7
    unsigned int v6; // eax
    tCobOK3AABBoxElemChunkElem* v7; // esi
    int v8; // edi
    Vec3* a4; // [esp+4h] [ebp-24h] BYREF
    Vec3* a5; // [esp+8h] [ebp-20h] BYREF
    Vec3* a6; // [esp+Ch] [ebp-1Ch] BYREF
    float v12; // [esp+10h] [ebp-18h]
    float v13; // [esp+14h] [ebp-14h]
    float v14; // [esp+18h] [ebp-10h]
    float v15; // [esp+1Ch] [ebp-Ch]
    float v16; // [esp+20h] [ebp-8h]
    int v17; // [esp+2Ch] [ebp+4h]

    if (a1 && a2)
    {
        v4 = a1->num;
        v5 = 1000000.0;
        v6 = 0;
        v15 = 1000000.0;
        v16 = 1000000.0;
        v12 = -1000000.0;
        v13 = -1000000.0;
        v14 = -1000000.0;
        v17 = 0;
        if (v4 > 0)
        {
            do
            {
                v7 = &a1->chunkElems[v6];
                v8 = 0;
                if (v7->num)
                {
                    do
                    {
                        COL_pst_OK3_GetTriangle(a2, v7->triangleChunkID, v7->triangleChunkElemIDs[v8], &a4, &a5, &a6, type);
                        if (a4->x > (double)v12)
                            v12 = a4->x;
                        if (a4->x < (double)v15)
                            v15 = a4->x;
                        if (a4->y > (double)v13)
                            v13 = a4->y;
                        if (a4->y < (double)v16)
                            v16 = a4->y;
                        if (a4->z > (double)v14)
                            v14 = a4->z;
                        if (v5 > a4->z)
                            v5 = a4->z;
                        if (a5->x > (double)v12)
                            v12 = a5->x;
                        if (a5->x < (double)v15)
                            v15 = a5->x;
                        if (a5->y > (double)v13)
                            v13 = a5->y;
                        if (a5->y < (double)v16)
                            v16 = a5->y;
                        if (a5->z > (double)v14)
                            v14 = a5->z;
                        if (v5 > a5->z)
                            v5 = a5->z;
                        if (a6->x > (double)v12)
                            v12 = a6->x;
                        if (a6->x < (double)v15)
                            v15 = a6->x;
                        if (a6->y > (double)v13)
                            v13 = a6->y;
                        if (a6->y < (double)v16)
                            v16 = a6->y;
                        if (a6->z > (double)v14)
                            v14 = a6->z;
                        if (v5 > a6->z)
                            v5 = a6->z;
                        ++v8;
                    } while (v8 < v7->num);
                    v6 = v17;
                }
                v17 = ++v6;
            } while (v6 < a1->num);
        }
        if (a1->vec1.x < (double)v15)
            a1->vec1.x = v15;
        if (a1->vec2.x > (double)v12)
            a1->vec2.x = v12;
        if (a1->vec1.y < (double)v16)
            a1->vec1.y = v16;
        if (a1->vec2.y > (double)v13)
            a1->vec2.y = v13;
        if (v5 > a1->vec1.z)
            a1->vec1.z = (float)v5;
        if (a1->vec2.z > (double)v14)
            a1->vec2.z = v14;
    }
}

BOOL COL_OK3_TriangleTouchBox(Vec3* a1, Vec3* a2, Vec3* a3, Vec3* a4, Vec3* a5)
{
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    char v11;
    int v12;
    int v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    int v34;
    int v35;
    int v36;
    int v37;
    int v38;
    Vec3 v39;
    Vec3 v40;
    Vec3 a5a;
    Vec3 v42;
    Vec3 dest;
    Vec3 src2;

    v37 = Face_Plane(a1, a4, a5);
    if (!v37)
        return 1;
    v34 = Face_Plane(a2, a4, a5);
    if (!v34)
        return 1;
    v6 = Face_Plane(a3, a4, a5);
    if (!v6)
        return 1;
    if ((v34 & v6 & v37) != 0)
        return 0;
    v7 = (bevel_2D(a1, a4, a5) << 8) | v37;
    v35 = (bevel_2D(a2, a4, a5) << 8) | v34;
    v8 = (bevel_2D(a3, a4, a5) << 8) | v6;
    if ((v35 & v8 & v7) != 0)
        return 0;
    v9 = (bevel_3D(a1, a4, a5) << 24) | v7;
    v38 = v9;
    v36 = (bevel_3D(a2, a4, a5) << 24) | v35;
    v10 = bevel_3D(a3, a4, a5);
    v11 = v36;
    v12 = (v10 << 24) | v8;
    v13 = v36 & v12;
    if ((v36 & v12 & v9) != 0)
        return 0;
    if ((v36 & v9) == 0)
    {
        if (check_line(a1, a2, v9 | v36, a4, a5))
            return 1;
        v9 = v38;
        v13 = v36 & v12;
        v11 = v36;
    }
    if ((v12 & v9) != 0)
        goto LABEL_16;
    if (check_line(a1, a3, v9 | v12, a4, a5))
        return 1;
    v13 = v36 & v12;
    v11 = v36;
LABEL_16:
    if (!v13 && check_line(a2, a3, v11 | v12, a4, a5))
        return 1;
    MATH_SubstractVec(&dest, a2, a1);
    MATH_SubstractVec(&src2, a3, a1);
    MATH_CrossProduct(&a5a, &dest, &src2);
    v14 = a4->y;
    v15 = a4->z;
    v39.x = a4->x;
    v16 = a5->x;
    v39.y = v14;
    v17 = a5->y;
    dest.x = v16;
    dest.y = v17;
    v39.z = v15;
    dest.z = a5->z;
    MATH_SubstractVec(&src2, a1, &v39);
    v42.x = dest.x - v39.x;
    v42.y = dest.y - v39.y;
    v42.z = dest.z - v39.z;
    v30 = (a5a.z * src2.z + a5a.y * src2.y + a5a.x * src2.x) / (a5a.z * v42.z + a5a.x * v42.x + a5a.y * v42.y);
    MATH_VecMultScalar(&v40, &v42, v30);
    v40.x = v40.x + v39.x;
    v40.y = v40.y + v39.y;
    v40.z = v40.z + v39.z;
    if (INT_PointInTriangle(&v40, a1, a2, a3, &a5a, 0))
        return 1;
    v18 = a5->y;
    v19 = a4->z;
    v39.x = a4->x;
    v20 = a5->x;
    v39.y = v18;
    v21 = a4->y;
    dest.x = v20;
    dest.y = v21;
    v39.z = v19;
    dest.z = a5->z;
    MATH_SubstractVec(&src2, a1, &v39);
    v42.x = dest.x - v39.x;
    v42.y = dest.y - v39.y;
    v42.z = dest.z - v39.z;
    v31 = (a5a.z * src2.z + a5a.y * src2.y + a5a.x * src2.x) / (a5a.z * v42.z + a5a.x * v42.x + a5a.y * v42.y);
    MATH_VecMultScalar(&v40, &v42, v31);
    v40.x = v40.x + v39.x;
    v40.y = v40.y + v39.y;
    v40.z = v40.z + v39.z;
    if (INT_PointInTriangle(&v40, a1, a2, a3, &a5a, 0))
        return 1;
    v22 = a5->y;
    v23 = a4->z;
    v39.x = a5->x;
    v24 = a4->x;
    v39.y = v22;
    v25 = a4->y;
    dest.x = v24;
    dest.y = v25;
    v39.z = v23;
    dest.z = a5->z;
    MATH_SubstractVec(&src2, a1, &v39);
    v42.x = dest.x - v39.x;
    v42.y = dest.y - v39.y;
    v42.z = dest.z - v39.z;
    v32 = (a5a.z * src2.z + a5a.y * src2.y + a5a.x * src2.x) / (a5a.z * v42.z + a5a.x * v42.x + a5a.y * v42.y);
    MATH_VecMultScalar(&v40, &v42, v32);
    v40.x = v40.x + v39.x;
    v40.y = v40.y + v39.y;
    v40.z = v40.z + v39.z;
    if (INT_PointInTriangle(&v40, a1, a2, a3, &a5a, 0))
        return 1;
    v26 = a4->y;
    v27 = a4->z;
    v39.x = a5->x;
    v28 = a4->x;
    v39.y = v26;
    v29 = a5->y;
    dest.x = v28;
    dest.y = v29;
    v39.z = v27;
    dest.z = a5->z;
    MATH_SubstractVec(&src2, a1, &v39);
    v42.x = dest.x - v39.x;
    v42.y = dest.y - v39.y;
    v42.z = dest.z - v39.z;
    v33 = (a5a.z * src2.z + a5a.y * src2.y + a5a.x * src2.x) / (a5a.z * v42.z + a5a.x * v42.x + a5a.y * v42.y);
    MATH_VecMultScalar(&v40, &v42, v33);
    v40.x = v40.x + v39.x;
    v40.y = v40.y + v39.y;
    v40.z = v40.z + v39.z;
    return INT_PointInTriangle(&v40, a1, a2, a3, &a5a, 0) != 0;
}

void COL_OK3_Optimize(tCobOK3* a1, void* a2, int type)
{
    int v4; // eax
    tCobOK3AABBoxElem* v5; // ebx
    unsigned int v6; // edx
    tCobOK3AABBoxElemChunkElem* v7; // edi
    int v8; // eax
    int v9; // eax
    unsigned int v10; // edx
    unsigned int v11; // esi
    int v12; // edi
    int v13; // [esp+10h] [ebp-4h]
    int a1a; // [esp+18h] [ebp+4h]

    v4 = 0;
    v13 = 0;
    if (a1->num <= 0)
        goto LABEL_14;
    a1a = 0;
    do
    {
        v5 = &a1->aaBoxes[v13];
        v6 = 0;
        if (v5->num <= 0)
            goto LABEL_10;
        while (1)
        {
            v7 = &v5->chunkElems[v6];
            if (!v7->num)
                break;
        LABEL_7:
            if (++v6 >= v5->num)
                goto LABEL_10;
        }
        v8 = v5->num;
        if (v8 != 1)
        {
            memcpy(v7, &v7[1], 8 * (v8 - v6) - 8);
            --v5->num;
            --v6;
            goto LABEL_7;
        }
        MEM_Free(v5->chunkElems);
        v5->chunkElems = 0;
        v5->num = 0;
    LABEL_10:
        v9 = a1a;
        if (v5->chunkElems)
        {
            v10 = v13;
        }
        else
        {
            memcpy(&(a1->aaBoxes[v13]), &(a1->aaBoxes[v13 + 1]), 32 * (a1->num - v13 - 1));
            v10 = v13 - 1;
            --a1->num;
            v9 = a1a - 32;
        }
        v4 = v9 + 32;
        v13 = v10 + 1;
        a1a = v4;
    } while (v10 + 1 < a1->num);
LABEL_14:
    v11 = 0;
    if (a1->num > 0)
    {
        v12 = 0;
        do
        {
            COL_OK3_OptimizeBoxBV(&a1->aaBoxes[v12], a2, type);
            ++v11;
            ++v12;
        } while (v11 < a1->num);
    }
}

int COL_OK3_SubdivideBox(void* aa, unsigned int a2, int* a3, int type)
{
    tCobOK3* v4; // ebx
    BOOL v5; // cc
    tCobOK3AABBoxElem* v6; // ecx
    unsigned int v7; // esi
    double v8; // st7
    int v9; // edx
    double v10; // st7
    int v11; // eax
    int v12; // eax
    int v13; // eax
    int v14; // edx
    int v15; // eax
    int v16; // edi
    int v17; // edx
    tCobOK3AABBoxElem* v18; // ebx
    int v19; // ecx
    float v20; // edx
    BOOL v21; // zf
    int v22; // eax
    double v23; // st7
    float v24; // eax
    float v25; // edx
    int v26; // eax
    float v27; // edx
    float v28; // edx
    double v29; // st7
    float v30; // eax
    float v31; // edx
    float v32; // eax
    float v33; // edx
    float v34; // eax
    tCobOK3AABBoxElem* v35; // ebp
    float v36; // ecx
    float v37; // edx
    float v38; // eax
    int v39; // eax
    tCobOK3AABBoxElemChunkElem* v40; // eax
    unsigned int v41; // edx
    int v42; // eax
    int v43; // esi
    tCobOK3AABBoxElemChunkElem* v44; // edi
    tCobOK3AABBoxElemChunkElem* v45; // eax
    tCobOK3AABBoxElemChunkElem* v46; // eax
    int v47; // edx
    unsigned int v48; // esi
    __int16* v49; // eax
    int v50; // ecx
    tCobOK3* v51; // edi
    char* v52; // eax
    unsigned int v53; // ebx
    char* v54; // ebp
    unsigned int i; // esi
    tCobOK3AABBoxElemChunkElem* v56; // ecx
    tCobOK3* v57; // ebx
    unsigned int v58; // esi
    int v59; // eax
    float v60; // [esp+0h] [ebp-4E70h]
    float v61; // [esp+0h] [ebp-4E70h]
    float v62; // [esp+4h] [ebp-4E6Ch]
    float v63; // [esp+4h] [ebp-4E6Ch]
    float v64; // [esp+8h] [ebp-4E68h]
    float v65; // [esp+8h] [ebp-4E68h]
    Vec3 v66; // [esp+Ch] [ebp-4E64h] BYREF
    Vec3 v67; // [esp+18h] [ebp-4E58h] BYREF
    int v68; // [esp+24h] [ebp-4E4Ch]
    int v69 = 0; // [esp+28h] [ebp-4E48h]
    tCobOK3* v70; // [esp+2Ch] [ebp-4E44h]
    int v71; // [esp+30h] [ebp-4E40h]
    int v72; // [esp+34h] [ebp-4E3Ch]
    int v73; // [esp+38h] [ebp-4E38h]
    unsigned int v74; // [esp+3Ch] [ebp-4E34h]
    int v75; // [esp+40h] [ebp-4E30h]
    Vec3* a6; // [esp+44h] [ebp-4E2Ch] BYREF
    Vec3* a5; // [esp+48h] [ebp-4E28h] BYREF
    Vec3* a4; // [esp+4Ch] [ebp-4E24h] BYREF
    __int16 v79[10000]; // [esp+50h] [ebp-4E20h] BYREF

    if (!aa)
        return 0;
    v4 = type ? ((tCobMeshObjectMesh *)aa)->ok3 : ((tGEOGRO*)aa)->collisionPtr;
    v5 = a2 <= v4->num;
    v70 = v4;
    if (!v5)
        return 0;
    v6 = v4->aaBoxes;
    v7 = a2;
    v8 = v6[v7].vec2.x - v6[v7].vec1.x;
    v74 = 32 * a2;
    v60 = (float)v8;
    v62 = v6[v7].vec2.y - v6[v7].vec1.y;
    v64 = v6[v7].vec2.z - v6[v7].vec1.z;
    if (a3)
    {
        v9 = *a3;
    }
    else
    {
        v9 = 0;
        v10 = v60 * 1.5;
        v71 = 0;
        if (v64 > v10 || v62 * 1.5 < v64)
        {
            v9 = v64 > 2.0;
            v71 = v9;
        }
        if (v62 > v10 || v64 * 1.5 < v62)
        {
            if (v62 <= 1.0)
                v11 = 0;
            else
                v11 = 2;
            v9 |= v11;
            v71 = v9;
        }
        if (v62 * 1.5 < v60 || v64 * 1.5 < v60)
        {
            if (v60 <= 1.0)
                v12 = 0;
            else
                v12 = 4;
            v9 |= v12;
            v71 = v9;
        }
        if (v9)
            goto LABEL_33;
        if (v62 <= 1.0)
            v13 = 0;
        else
            v13 = 2;
        v14 = v13 | (v64 > 2.0);
        if (v60 <= 1.0)
            v15 = 0;
        else
            v15 = 4;
        v9 = v15 | v14;
    }
    v71 = v9;
LABEL_33:
    switch (v9)
    {
    case 0:
        return 0;
    case 1:
    case 2:
    case 4:
        v16 = 2;
        v69 = 2;
        goto LABEL_38;
    case 3:
    case 5:
    case 6:
        v16 = 4;
        v69 = 4;
        goto LABEL_38;
    case 7:
        v16 = 8;
        v69 = 8;
        goto LABEL_38;
    default:
        v16 = v69;
    LABEL_38:
        v17 = v16 + v4->num;
        v4->num = v17;
        if (v17 % 0x32 <= v16)
        {
            v70->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Realloc(v6, sizeof(tCobOK3AABBoxElem) * (v17 + 50));
            ++COL_OK3_Realloc;
        }
        v18 = &v70->aaBoxes[v7];
        v61 = (float)(v60 * 0.5);
        v68 = 0;
        v63 = (float)(v62 * 0.5);
        v65 = (float)(v64 * 0.5);
        if (v16 <= 0)
            goto LABEL_111;
        break;
    }
    do
    {
        switch (v71)
        {
        case 7:
            v19 = v68;
            if ((v68 & 4) != 0)
                v67.z = v18->vec1.z;
            else
                v67.z = v65 + v18->vec1.z;
            if ((v68 & 4) != 0)
                v66.z = v18->vec2.z - v65;
            else
                v66.z = v18->vec2.z;
            if ((v68 & 2) != 0)
                v67.y = v18->vec1.y;
            else
                v67.y = v63 + v18->vec1.y;
            if ((v68 & 2) != 0)
            {
                v66.y = v18->vec2.y - v63;
            LABEL_98:
                v22 = v19 & 1;
                v21 = v22 == 0;
                goto LABEL_99;
            }
            goto LABEL_70;
        case 6:
            v20 = v18->vec2.z;
            v67.z = v18->vec1.z;
            v19 = v68;
            v66.z = v20;
            if ((v68 & 1) != 0)
                v67.y = v18->vec1.y;
            else
                v67.y = v63 + v18->vec1.y;
            if ((v68 & 1) != 0)
            {
                v22 = v68 & 2;
                v21 = v22 == 0;
                v66.y = v18->vec2.y - v63;
            }
            else
            {
                v66.y = v18->vec2.y;
                v22 = v68 & 2;
                v21 = v22 == 0;
            }
        LABEL_99:
            if (v21)
                v67.x = v61 + v18->vec1.x;
            else
                v67.x = v18->vec1.x;
            if (v22)
            {
                v66.x = v18->vec2.x - v61;
                goto LABEL_102;
            }
            goto LABEL_89;
        case 5:
            v19 = v68;
            if ((v68 & 2) != 0)
                v67.z = v18->vec1.z;
            else
                v67.z = v65 + v18->vec1.z;
            if ((v68 & 2) != 0)
            {
                v23 = v18->vec2.z - v65;
                v24 = v18->vec2.y;
                v67.y = v18->vec1.y;
                v66.y = v24;
                v66.z = (float)v23;
                goto LABEL_98;
            }
            v25 = v18->vec1.y;
            v66.z = v18->vec2.z;
            v67.y = v25;
        LABEL_70:
            v66.y = v18->vec2.y;
            goto LABEL_98;
        case 3:
            v19 = v68;
            if ((v68 & 1) != 0)
                v67.z = v18->vec1.z;
            else
                v67.z = v65 + v18->vec1.z;
            if ((v68 & 1) != 0)
                v66.z = v18->vec2.z - v65;
            else
                v66.z = v18->vec2.z;
            v26 = v68 & 2;
            if ((v68 & 2) != 0)
            {
                v67.y = v18->vec1.y;
            LABEL_94:
                if (!v26)
                    goto LABEL_88;
                v29 = v18->vec2.y - v63;
                v30 = v18->vec2.x;
                v67.x = v18->vec1.x;
                v66.x = v30;
                v66.y = (float)v29;
                goto LABEL_102;
            }
            goto LABEL_93;
        case 1:
            v19 = v68;
            if ((v68 & 1) != 0)
                v67.z = v18->vec1.z;
            else
                v67.z = v65 + v18->vec1.z;
            if ((v68 & 1) != 0)
                v66.z = v18->vec2.z - v65;
            else
                v66.z = v18->vec2.z;
            v67.y = v18->vec1.y;
        LABEL_88:
            v27 = v18->vec1.x;
            v66.y = v18->vec2.y;
            v67.x = v27;
        LABEL_89:
            v66.x = v18->vec2.x;
            goto LABEL_102;
        case 2:
            v28 = v18->vec2.z;
            v67.z = v18->vec1.z;
            v19 = v68;
            v26 = v68 & 1;
            v66.z = v28;
            if ((v68 & 1) != 0)
            {
                v67.y = v18->vec1.y;
                goto LABEL_94;
            }
        LABEL_93:
            v67.y = v63 + v18->vec1.y;
            goto LABEL_94;
        case 4:
            v31 = v18->vec2.z;
            v32 = v18->vec1.y;
            v67.z = v18->vec1.z;
            v66.y = v18->vec2.y;
            v19 = v68;
            v66.z = v31;
            v67.y = v32;
            goto LABEL_98;
        }
        v19 = v68;
    LABEL_102:
        v33 = v67.y;
        v34 = v67.z;
        v35 = &v70->aaBoxes[v19 + v70->num - v69];
        v35->vec1.x = v67.x;
        v36 = v66.x;
        v35->vec1.y = v33;
        v37 = v66.y;
        v35->vec1.z = v34;
        v38 = v66.z;
        v35->vec2.x = v36;
        v35->vec2.y = v37;
        v35->vec2.z = v38;
        v39 = v18->num;
        v35->num = v39;
        v40 = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(8 * v39);
        v41 = 8 * v35->num;
        v35->chunkElems = v40;
        memset(v40, 0, v41);
        v42 = v18->num;
        v75 = 0;
        if (v42 > 0)
        {
            do
            {
                v43 = v75;
                v44 = &v35->chunkElems[v75];
                v44->triangleChunkID = v18->chunkElems[v75].triangleChunkID;
                v44->num = 0;
                v45 = &v18->chunkElems[v43];
                v21 = v45->num == 0;
                v72 = 0;
                v73 = 0;
                if (!v21)
                {
                    do
                    {
                        COL_pst_OK3_GetTriangle(aa, v44->triangleChunkID, v45->triangleChunkElemIDs[v73], &a4, &a5, &a6, type);
                        if (COL_OK3_TriangleTouchBox(a4, a5, a6, &v67, &v66))
                            v79[(uint16_t)v72++] = v18->chunkElems[v43].triangleChunkElemIDs[v73];
                        v46 = v18->chunkElems;
                        v47 = v46[v43].num;
                        v45 = &v46[v43];
                        ++v73;
                    } while (v73 < v47);
                    if ((int16_t)v72)
                    {
                        v48 = 2 * (uint16_t)v72;
                        v44->num = v72;
                        v49 = (int16_t*)MEM_p_Alloc(v48);
                        v44->triangleChunkElemIDs = v49;
                        memcpy(v49, v79, v48);
                    }
                }
                v50 = v18->num;
                ++v75;
            } while (v75 < v50);
        }
        ++v68;
    } while (v68 < v69);
LABEL_111:
    v51 = v70;
    v52 = (char*)MEM_p_Alloc(32 * (v70->num - v69 - a2));
    v53 = v74;
    v54 = v52;
    for (i = 0; i < v51->aaBoxes[a2].num; ++i)
    {
        v56 = v51->aaBoxes[a2].chunkElems;
        if (v56[i].triangleChunkElemIDs)
            MEM_Free(v56[i].triangleChunkElemIDs);
    }
    v57 = v70;
    v58 = v74;
    MEM_Free(v70->aaBoxes[a2].chunkElems);
    v59 = v69;
    memcpy(v54, &v57->aaBoxes[a2 + 1], 32 * (v57->num - v69 - a2));
    memcpy(&v57->aaBoxes[a2], &v57->aaBoxes[v57->num - v59], 4 * ((unsigned int)(32 * v59) >> 2));
    memcpy(&v57->aaBoxes[v59 + a2], v54, 32 * (v57->num - v59 - a2));
    MEM_Free(v54);
    v57->aaBoxes[v57->num - 1].num = 0;
    v57->aaBoxes[v57->num - 1].chunkElems = 0;

    return 1;
}

void COL_OK3_Build(void* aa, int type, int a3)
{
    tGameObjectExtended* v4;
    tGameObjectCollMain* v5;
    tColMap* v6;
    tColCollisionObject** v7;
    tColCollisionObject* v8;
    tCobMeshObjectMesh* v9;
    tCobOK3* v10;
    tCobOK3AABBoxElem* v11;
    int v12;
    tCobOK3AABBoxElem* v13;
    unsigned int v14;
    tCobOK3AABBoxElemChunkElem* v15;
    BOOL v16;
    tColCollisionObject** v17;
    Vec3* v18;
    int v19;
    Vec3* v20;
    tCobOK3* v21;
    tCobOK3AABBoxElem* v22;
    tCobMeshChunkElem* v23;
    tCobOK3AABBoxElemChunkElem* v24;
    int v25;
    unsigned int v26;
    int v27;
    tColCollisionObject* v28;
    tCobOK3* v29;
    int v30;
    tCobOK3AABBoxElem *v31;
    int v32;
    tCobOK3AABBoxElem *v33;
    unsigned int v34;
    tCobOK3AABBoxElemChunkElem* v35;
    tCobOK3* v36;
    Vec3* v37;
    int v38;
    Vec3* v39;
    tCobOK3* v40;
    unsigned int v41;
    tGeoTriangleChunks* v42;
    tCobOK3AABBoxElemChunkElem* j;
    unsigned int k;
    unsigned int v49;
    unsigned int v50a;
    Vec3 v50;
    float v51;
    float v53;
    Vec3 v54;
    float v55;
    float v56;
    Vec3 v57; 
    float v58;
    float v61;
    float v63;
    int v65;
    unsigned int i;
    unsigned int v45;
    int v46;

    if (aa)
    {
        if (type)
        {
            tGameObjectMain *a1 = (tGameObjectMain*)aa;
            if ((a1->flags & 0x100) != 0)
            {
                v4 = a1->extended;
                if (v4)
                {
                    v5 = v4->coll;
                    if (v5)
                    {
                        v6 = v5->colMap;
                        if (v6)
                        {
                            v7 = v6->collisionObjectPtr;
                            if (v7)
                            {
                                v8 = *v7;
                                if (v8->objectType == 5)
                                {
                                    v8->flags &= ~8u;
                                    v9 = (tCobMeshObjectMesh*)(*a1->extended->coll->colMap->collisionObjectPtr)->objectPtrByType;
                                    if (a3)
                                    {
                                        v10 = v9->ok3;
                                        if (v10)
                                        {
                                            v50a = 0;
                                            if (v10->num > 0)
                                            {
                                                v65 = 0;
                                                do
                                                {
                                                    v11 = v9->ok3->aaBoxes;
                                                    v12 = v11[v65].num;
                                                    v13 = &v11[v65];
                                                    v14 = 0;
                                                    if (v12 > 0)
                                                    {
                                                        do
                                                        {
                                                            v15 = v13->chunkElems;
                                                            if (v15[v14].triangleChunkElemIDs)
                                                                MEM_Free(v15[v14].triangleChunkElemIDs);
                                                            ++v14;
                                                        } while (v14 < v13->num);
                                                    }
                                                    MEM_Free(v13->chunkElems);
                                                    v16 = ++v50a < v9->ok3->num;
                                                    ++v65;
                                                } while (v16);
                                            }
                                            MEM_Free(v9->ok3->aaBoxes);
                                            MEM_Free(v9->ok3->faceTagInts);
                                            MEM_Free(v9->ok3);
                                            v9->ok3 = 0;
                                        }
                                    }
                                    if (v9->ok3)
                                    {
                                        v17 = a1->extended->coll->colMap->collisionObjectPtr;
                                        (*v17)->flags |= 8u;
                                    }
                                    else if (v9->faceCount >= 75u)
                                    {
                                        v18 = v9->vertices;
                                        v53 = v18->y;
                                        v57.x = v18->x;
                                        v51 = v18->x;
                                        v57.z = v18->z;
                                        v55 = v57.z;
                                        v57.y = v53;
                                        if (v9->vertexCount > 0)
                                        {
                                            v19 = v9->vertexCount;
                                            v20 = v9->vertices;
                                            do
                                            {
                                                if (v20->x < (double)v51)
                                                    v51 = v20->x;
                                                if (v20->y < (double)v53)
                                                    v53 = v20->y;
                                                if (v20->z < (double)v55)
                                                    v55 = v20->z;
                                                if (v20->x > (double)v57.x)
                                                    v57.x = v20->x;
                                                if (v20->y > (double)v57.y)
                                                    v57.y = v20->y;
                                                if (v20->z > (double)v57.z)
                                                    v57.z = v20->z;
                                                ++v20;
                                                --v19;
                                            } while (v19);
                                        }
                                        if (v57.x - v51 >= 2.0 || (v61 = v57.y - v53, v61 >= 2.0) || (v63 = v57.z - v55, v63 >= 2.0))
                                        {
                                            v21 = (tCobOK3*)MEM_p_Alloc(0xCu);
                                            v49 = (((unsigned int)v9->faceCount >> 3) & 0x1FFFFFFC) + 4;
                                            v9->ok3 = v21;
                                            v9->ok3->faceTagInts = (int*)MEM_p_Alloc(v49);
                                            memset(v9->ok3->faceTagInts, 0, 4 * ((unsigned int)v9->faceCount >> 5) + 4);
                                            v9->ok3->num = 1;
                                            v9->ok3->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(0x640u);
                                            v22 = v9->ok3->aaBoxes;
                                            v22->vec1.x = v51;
                                            v22->vec1.y = v53;
                                            v22->vec1.z = v55;
                                            v9->ok3->aaBoxes->vec2 = v57;
                                            v9->ok3->aaBoxes->num = v9->meshChunkElemNum;
                                            v9->ok3->aaBoxes->chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(8 * v9->meshChunkElemNum);
                                            v23 = v9->meshChunkElems;
                                            v24 = v9->ok3->aaBoxes->chunkElems;
                                            for (i = 0; i < v9->meshChunkElemNum; ++i)
                                            {

                                                v24->triangleChunkID = i;
                                                v24->num = v23->elemNum;
                                                if (v23->elemNum)
                                                {
                                                    v24->triangleChunkElemIDs = (__int16*)MEM_p_Alloc(2 * v23->elemNum);
                                                    v25 = 0;
                                                    if (v23->elemNum)
                                                    {
                                                        do
                                                        {
                                                            v24->triangleChunkElemIDs[v25] = v25;
                                                            ++v25;
                                                        } while (v25 < v23->elemNum);
                                                    }
                                                }
                                                else
                                                {
                                                    v24->triangleChunkElemIDs = 0;
                                                }
                                                ++v23;
                                                ++v24;
                                            }
                                            COL_OK3_SubdivideBox(v9, 0, 0, 1);
                                            v26 = 0;
                                            if (v9->ok3->num > 0)
                                            {
                                                v27 = 0;
                                                do
                                                {
                                                    if (COL_OK3_GetNumberOfFacesInBox(&v9->ok3->aaBoxes[v27]) > 75)
                                                    {
                                                        if (COL_OK3_SubdivideBox(v9, v26, 0, 1))
                                                        {
                                                            --v26;
                                                            --v27;
                                                        }
                                                    }
                                                    ++v26;
                                                    ++v27;
                                                } while (v26 < v9->ok3->num);
                                            }
                                            v28 = *a1->extended->coll->colMap->collisionObjectPtr;
                                            v28->flags |= 8u;
                                            COL_OK3_Optimize(v9->ok3, v9, type);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
        tGEOGRO* a1 = (tGEOGRO*)aa;
        if (a3)
        {
            v29 = a1->collisionPtr;
            if (v29)
            {
                v61 = 0;
                if (v29->num > 0)
                {
                    v30 = 0;
                    do
                    {
                        v31 = a1->collisionPtr->aaBoxes;
                        v32 = v31[v30].num;
                        v33 = &v31[v30];
                        v34 = 0;
                        if (v32 > 0)
                        {
                            do
                            {
                                v35 = v33->chunkElems;
                                if (v35[v34].triangleChunkElemIDs)
                                    MEM_Free(v35[v34].triangleChunkElemIDs);
                                ++v34;
                            } while (v34 < v33->num);
                        }
                        MEM_Free(v33->chunkElems);
                        ++v30;
                        ++v61;
                    } while (v61 < a1->collisionPtr->num);
                }
                MEM_Free(a1->collisionPtr->aaBoxes);
                v36 = a1->collisionPtr;
                if (v36->faceTagInts)
                    MEM_Free((void*)v36->faceTagInts);
                MEM_Free(a1->collisionPtr);
                a1->collisionPtr = 0;
            }
        }
        if (!a1->collisionPtr && a1->vertexNum >= 100)
        {
            v37 = a1->vertices;
            v50 = *v37;
            v54.x = v37->x;
            v54.y = v50.y;
            v54.z = v50.z;
            v38 = a1->vertexNum;
            v39 = a1->vertices;
            do
            {
                if (v39->x < (double)v50.x)
                    v50.x = v39->x;
                if (v39->y < (double)v50.y)
                    v50.y = v39->y;
                if (v39->z < (double)v50.z)
                    v50.z = v39->z;
                if (v39->x > (double)v54.x)
                    v54.x = v39->x;
                if (v39->y > (double)v54.y)
                    v54.y = v39->y;
                if (v39->z > (double)v54.z)
                    v54.z = v39->z;
                ++v39;
                --v38;
            } while (v38);
            if (v54.x - v50.x >= 2.0 || (v56 = v54.y - v50.y, v56 >= 2.0) || (v58 = v54.z - v50.z, v58 >= 2.0))
            {
                v40 = (tCobOK3*)MEM_p_Alloc(0xCu);
                a1->collisionPtr = v40;
                v41 = 0;
                v40->faceTagInts = 0;
                a1->collisionPtr->num = 1;
                a1->collisionPtr->aaBoxes = (tCobOK3AABBoxElem*)MEM_p_Alloc(0x640u);
                a1->collisionPtr->aaBoxes->vec1 = v50;
                a1->collisionPtr->aaBoxes->vec2 = v54;
                a1->collisionPtr->aaBoxes->num = a1->triangleChunkCount;
                a1->collisionPtr->aaBoxes->chunkElems = (tCobOK3AABBoxElemChunkElem*)MEM_p_Alloc(8 * a1->triangleChunkCount);
                v42 = a1->triangleChunks;
                for (j = a1->collisionPtr->aaBoxes->chunkElems; v41 < a1->triangleChunkCount; ++v41)
                {
                    j->triangleChunkID = v41;
                    j->num = v42->triangleNum;
                    if (v42->triangleNum)
                    {
                        j->triangleChunkElemIDs = (__int16*)MEM_p_Alloc(2 * v42->triangleNum);
                        for (k = 0; k < v42->triangleNum; ++k)
                            j->triangleChunkElemIDs[k] = k;
                    }
                    else
                    {
                        j->triangleChunkElemIDs = 0;
                    }
                    ++v42;
                    ++j;
                }
                COL_OK3_SubdivideBox((tCobMeshObjectMesh*)a1, 0, 0, 0);
                v45 = 0;
                if (a1->collisionPtr->num > 0)
                {
                    v46 = 0;
                    do
                    {
                        if (COL_OK3_GetNumberOfFacesInBox((tCobOK3AABBoxElem*)&a1->collisionPtr->aaBoxes[v46]) > 0x64
                            && COL_OK3_SubdivideBox(a1, v45, 0, 0))
                        {
                            --v45;
                            --v46;
                        }
                        ++v45;
                        ++v46;
                    } while (v45 < a1->collisionPtr->num);
                }
                COL_OK3_Optimize((tCobOK3*)a1->collisionPtr, (tCobMeshObjectMesh*)a1, 0);
            }
        }
    }
  }
}