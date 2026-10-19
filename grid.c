#include "math.h"
#include "gameobject.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "grid.h"

int GRID_gi_TestWall = 0x20;

int ai_Scan[43] = { 0, -1, 1, 0, 0, 1, -1, 0, -1, -1, 1, -1, 1, 1, -1, 1, -1, 0, 0, -1, 1, 0, 0, 1, -1, 0, -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0, 1 };
//int ai_Scan_511[17] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0, 1 };

int GRI_3Dto2D(tWorldGridStruct* a1, Vec3* pos, int* retPosX, int* retPosY)
{
    int v4;
    int result;

    v4 = 1;
    if (pos->x >= (double)a1->startX)
    {
        if ((double)(a1->gridCountX * a1->gridElemSize) + a1->startX >= pos->x)
        {
            *retPosX = (int)(pos->x - a1->startX);
        }
        else
        {
            *retPosX = a1->gridCountX * a1->gridElemSize - 1;
            v4 = 0;
        }
    }
    else
    {
        *retPosX = 0;
        v4 = 0;
    }
    if (pos->y >= (double)a1->startY)
    {
        if ((double)(a1->gridCountY * a1->gridElemSize) + a1->startY >= pos->y)
        {
            *retPosY = (int)(pos->y - a1->startY);
            result = v4;
        }
        else
        {
            *retPosY = a1->gridCountY * a1->gridElemSize - 1;
            result = 0;
        }
    }
    else
    {
        *retPosY = 0;
        result = 0;
    }
    return result;
}

int GRI_ConvertTo2DReal(tWorldGridStruct* a1, int32_t* posX, int32_t* posY)
{
    int v3;
    BOOL v4;
    int result;
    int v6;
    int v7;
    int v8;

    v3 = a1->gridElemSize * a1->s5Limit1MinX;
    v4 = *posX - v3 < 0;
    result = 1;
    *posX -= v3;
    if (v4)
    {
        result = 0;
        *posX = 0;
    }
    v6 = a1->gridElemSize * a1->s5Limit1MinY;
    v4 = *posY - v6 < 0;
    *posY -= v6;
    if (v4)
    {
        result = 0;
        *posY = 0;
    }
    v7 = a1->gridElemSize * a1->s3Size7LimitMaxX;
    if (*posX >= v7)
    {
        *posX = v7 - 1;
        result = 0;
    }
    v8 = a1->gridElemSize * a1->s4Size7LimitMaxY;
    if (*posY >= v8)
    {
        *posY = v8 - 1;
        result = 0;
    }
    return result;
}

void GRI_SetCapa(tWorldGridStruct* a1, Vec3* a2, char a3)
{
    char* v3;
    int retPosY;
    int retPosX;

    if (GRI_3Dto2D(a1, a2, &retPosX, &retPosY) && GRI_ConvertTo2DReal(a1, &retPosX, &retPosY))
    {
        v3 = &a1->capasPtr[retPosY * a1->s3Size7LimitMaxX * a1->gridElemSize + retPosX];
        if ((GRID_gi_TestWall & *v3) == 0)
            *v3 = a3;
    }
}

int GRI_AddCapa(tWorldGridStruct* a1, Vec3* a2, char a3)
{
    char* v4;
    int posX;
    int a3a;
    int a4;

    if (!GRI_3Dto2D(a1, a2, &a3a, &a4))
        return 1;
    posX = a3a;
    a3a = a4;
    if (!GRI_ConvertTo2DReal(a1, &posX, &a3a))
        return 1;
    if ((GRID_gi_Mode & 1) != 0
        || (a1->capasPtr[a3a * a1->s3Size7LimitMaxX * a1->gridElemSize + posX] & GRID_gi_TestWall) == 0)
    {
        v4 = &a1->capasPtr[a3a * a1->s3Size7LimitMaxX * a1->gridElemSize + posX];
        if (a3 > 0 && (a3 & 0xC0) != 0 && (*v4 & 0xC0) == 0xC0)
            return 0;
        *v4 += a3;
    }
    return 1;
}

int GRI_i_GetCapa(tWorldGridStruct* a1, Vec3* a2)
{
    int result;
    int a4;
    int a3;

    if (GRI_3Dto2D(a1, a2, &a3, &a4) && GRI_ConvertTo2DReal(a1, &a3, &a4))
        result = a1->capasPtr[a4 * a1->s3Size7LimitMaxX * a1->gridElemSize + a3];
    else
        result = 0;
    return result;
}

int GRI_i_ComputeDest(struct sWorldGridStruct* a1, Vec3* pos)
{
    int result;
    char* v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int retPosX;
    int retPosY;
    int v10;
    int v11;
    int v12;
    int ai_scanOrder[] = { -1, 0, -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1 };

    if (!GRI_3Dto2D(a1, pos, &retPosX, &retPosY) || !GRI_ConvertTo2DReal(a1, &retPosX, &retPosY))
        return 0;
    v3 = a1->capasPtr;
    v4 = a1->gridElemSize * a1->s3Size7LimitMaxX;
    v10 = GRID_gi_TestWall + 0xC0;
    if ((v3[retPosY * v4 + retPosX] & (GRID_gi_TestWall + 0xC0)) != 0)
    {
        v11 = retPosX;
        v12 = retPosY;
        v5 = 0;
        while (1)
        {
            v6 = v12 + ai_scanOrder[v5 + 1];
            retPosX = v11 + ai_scanOrder[v5];
            v7 = v3[v6 * v4 + retPosX];
            retPosY = v6;
            if ((v7 & v10) == 0)
                break;
            v5 += 2;
            if (v5 >= 16)
                return 0;
        }
        pos->x = (float)((double)retPosX + a1->startX + 0.5);
        pos->y = (float)((double)retPosY + a1->startY + 0.5);
        v12 = a1->gridElemSize * a1->s5Limit1MinX;
        pos->x = (float)((double)v12 + pos->x);
        v12 = a1->gridElemSize * a1->s5Limit1MinY;
        pos->y = (float)((double)v12 + pos->y);
        result = 1;
    }
    else
    {
        pos->x = (float)((double)retPosX + a1->startX + 0.5);
        pos->y = (float)((double)retPosY + a1->startY + 0.5);
        v12 = a1->gridElemSize * a1->s5Limit1MinX;
        result = 1;
        pos->x = (float)((double)v12 + pos->x);
        v12 = a1->gridElemSize * a1->s5Limit1MinY;
        pos->y = (float)((double)v12 + pos->y);
    }
    return result;
}

int GRI_SmoothWay(struct sWorldGridStruct* a1, struct sWorldGridStruct* a2, Vec3* a3, int a4)
{
    int v4;
    int v5;
    int v6;
    int* v7;
    int v8; 
    char v9;
    BOOL v10;
    Vec3* pos;
    int posY;
    int posX;
    int retPosY;
    int retPosX;
    int v17;

    v4 = a4 - 1;
    v17 = a4 - 1;
    if (v17 >= 0)
    {
        pos = &a3[v4];
        while (2)
        {
            if (GRI_3Dto2D(a2, pos, &retPosX, &retPosY))
            {
                v5 = retPosY;
                v6 = retPosX;
                v7 = &ai_Scan[27];
                do
                {
                    v8 = *v7;
                    posX = v6 + *(v7 - 1);
                    posY = v5 + v8;
                    if (!GRI_ConvertTo2DReal(a2, &posX, &posY))
                        return v17 + 1;
                    v9 = a2->capasPtr[posY * a2->s3Size7LimitMaxX * a2->gridElemSize + posX];
                    if ((v9 & GRID_gi_TestWall) != 0 && (a2 == a1 || (v9 & 0xC) == 0))
                        return v17 + 1;
                    if ((v9 & 0xC0) != 0 && (a2 == a1 || (v9 & 0xC) == 0))
                        return v17 + 1;
                    v7 += 2;
                } while ((int)v7 <= (int)&ai_Scan[42]);
                v10 = --v17 < 0;
                --pos;
                v4 = v17;
                if (!v10)
                    continue;
            }
            break;
        }
    }
    return v4 + 1;
}

int GRID_3Dto2D(tWorldGridStruct* a1, Vec3* a2, int32_t* a3, int32_t* a4)
{
    int result;

    result = GRI_3Dto2D(a1, a2, a3, a4);
    if (result)
        result = GRI_ConvertTo2DReal(a1, a3, a4) != 0;
    return result;
}

int GRID_ShortestPath(struct sGameObjectMain* gao, struct sWorldGridStruct* a2, struct sWorldGridStruct* a3, Vec3* a4, Vec3* a5, Vec3* a6, int a7, int a8, float a9, int a10, int a11)
{
    int result;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    int v22;
    int v23;
    int v24;
    float v25;
    float v26;
    float v27;
    char* v28;
    int v29;
    int* v30;
    char v31;
    int v32;
    int v33;
    tGridPtrElem* v34;
    int v35;
    float v36;
    int v37;
    int v38;
    int v39;
    int v40;
    int v41;
    BOOL v42;
    int v43; 
    tGridPtrElem* v44;
    Mtx4x4* v45;
    float v46;
    int v47;
    int v48;
    BOOL v49;
    int v50; 
    int v51; 
    int v52; 
    int v53; 
    int v54; 
    float v55; 
    float v56;
    float v57;
    float v58;
    char v59;
    int v60; 
    char v61;
    tGridPtrElem* v62;
    byte v63;
    float v64;
    float v65;
    int v66;
    int v67;
    int i; 
    int v69;
    int j;
    int v71;
    int v72;
    int v73;
    int v74;
    tGridPtrElem* v75;
    int v76;
    int v77;
    int v78;
    int v79;
    Vec3* v80;
    float v81;
    float v82;
    float v83;
    int v84;
    tGridPtrElem* v85;
    char v86;
    char v87;
    int v88;
    int v89;
    int v90;
    int v91;
    int v92;
    int a4a;
    int a3a;
    float v95; 
    int v96;
    int v97;
    int v98;
    int v99;
    int v100; 
    int v101; 
    float v102;
    float v103;
    int v104; 
    int v105; 
    Vec3 v106; 
    Vec3 a1a; 
    Vec3 a2a;
    Vec3 v109;
    int v95a;
    float v98a; 
    float v99a;

    v89 = 0;
    v88 = 0;
    v100 = 0;
    result = GRID_3Dto2D(a2, a4, &a3a, &a4a);
    if (!result)
        return result;
    OBJ_SightGet(gao, &v106);
    v106.x = v106.x + a4->x;
    v106.y = v106.y + a4->y;
    v106.z = v106.z + a4->z;
    GRID_3Dto2D(a2, &v106, &v92, &v90);
    GRID_3Dto2D(a2, a5, &v92, &v90);
    v12 = (float)a10;
    if (v12 >= a9)
        v12 = a9;
    v13 = a5->x - a4->x;
    v14 = a5->y;
    v15 = a5->z;
    v109.x = a5->x;
    v106.x = v13;
    v16 = a5->y;
    v109.y = v14;
    v17 = v16 - a4->y;
    v109.z = v15;
    v106.y = v17;
    v106.z = a5->z - a4->z;
    v18 = v106.z * v106.z;
    v19 = v106.y * v106.y;
    v20 = v106.x * v106.x;
    if (sqrt(v20 + v19 + v18) > v12)
    {
        v21 = (float)sqrt(v18 + v20 + v19);
        v106.y = 1.0f / v21 * v106.y;
        v106.z = v106.z * (1.0f / v21);
        v106.x = v106.x * (1.0f / v21) * v12;
        v106.y = v106.y * v12;
        v106.z = v106.z * v12;
        v109.x = v106.x + a4->x;
        v109.y = v106.y + a4->y;
        v109.z = v106.z + a4->z;
        GRID_3Dto2D(a2, &v109, &v92, &v90);
    }
    v22 = v92;
    v23 = v90;
    v24 = abs(v90 - a4a) + abs(v92 - a3a);
    if (v92 == a3a && v90 == a4a)
    {
        v101 = a2->gridElemSize * a2->s5Limit1MinX;
        v25 = (float)v101 + (float)a3a + 0.5f;
        a6->x = v25;
        a6->x = v25 + a2->startX;
        v101 = a2->gridElemSize * a2->s5Limit1MinY;
        v26 = (float)v101 + (float)a4a + 0.5f;
        a6->y = v26;
        v27 = v26 + a2->startY;
        a6->z = 0.0f;
        a6->y = v27;
        return 1;
    }
    if (a2 == a3
        && (v24 < a8 || (a3->capasPtr[v90 * a3->s3Size7LimitMaxX * a3->gridElemSize + v92] & GRID_gi_TestWall) != 0))
    {
        v28 = a3->capasPtr;
        v29 = a3->s3Size7LimitMaxX * a3->gridElemSize;
        v86 = v28[v90 * v29 + v92];
        v97 = (int)v28;
        if (((v86 & 0xC0) != 0 && v24 < a8 || (v86 & GRID_gi_TestWall) != 0)
            && ((v86 & GRID_gi_TestWall) == 0 || (v86 & 0xC0) == 0))
        {
            v98 = v92;
            v99 = v90;
            v30 = &ai_Scan[1];
            while (1)
            {
                v22 = v98 + *(v30 - 1);
                v23 = v99 + *v30;
                v87 = v28[v23 * v29 + v22];
                v92 = v22;
                v90 = v23;
                if ((GRID_gi_TestWall & v87) == 0 && (v87 & 0xC0) == 0)
                    break;
                v30 += 2;
                if ((int)v30 >= (int)&ai_Scan[17])
                    break;
                v28 = (char*)v97;
            }
            v28 = (char*)v97;
        }
        v31 = v28[v23 * v29 + v22];
        if (v31
            && ((GRID_gi_TestWall & v31) == 0 || (v31 & 0xC0) == 0)
            && ((GRID_gi_TestWall & v31) != 0 || (v31 & 0xC0) != 0))
        {
            return 0;
        }
    }
    v32 = a4a;
    v33 = a3a;
    a2->ptrs[a3a + a4a * a2->s3Size7LimitMaxX * a2->gridElemSize].flags = 2;
    a2->ptrs[v33 + v32 * a2->s3Size7LimitMaxX * a2->gridElemSize].fEval = 0.0;
    a2->ptrs[v33 + v32 * a2->s3Size7LimitMaxX * a2->gridElemSize].wayX = -1;
    a2->ptrs[v33 + v32 * a2->s3Size7LimitMaxX * a2->gridElemSize].wayY = -1;
    v34 = &a2->ptrs[v33 + v32 * a2->s3Size7LimitMaxX * a2->gridElemSize];
    v97 = 0;
    v35 = 1;
    GRID_gast_Best[0].ptrElems = v34;
    GRID_gast_Best[0].gridShorts.i1 = v33;
    GRID_gast_Best[0].gridShorts.i2 = v32;
    do
    {
        if (v100)
            break;
        v36 = FLT_MAX;
        v37 = v89;
        v38 = v88;
        v39 = -1;
        v40 = 0;
        if (v35 >= 4)
        {
            v95a = 3;
            do
            {
                if (v36 > GRID_gast_Best[v40].ptrElems->fEval)
                {
                    v36 = GRID_gast_Best[v40].ptrElems->fEval;
                    v37 = GRID_gast_Best[v40].gridShorts.i1;
                    v38 = GRID_gast_Best[v40].gridShorts.i2;
                    v39 = v40;
                }
                if (v36 > GRID_gast_Best[v40 + 1].ptrElems->fEval)
                {
                    v36 = GRID_gast_Best[v40 + 1].ptrElems->fEval;
                    v37 = GRID_gast_Best[v40 + 1].gridShorts.i1;
                    v38 = GRID_gast_Best[v40 + 1].gridShorts.i2;
                    v39 = v95a - 2;
                }
                if (v36 > GRID_gast_Best[v40 + 2].ptrElems->fEval)
                {
                    v36 = GRID_gast_Best[v40 + 2].ptrElems->fEval;
                    v37 = GRID_gast_Best[v40 + 2].gridShorts.i1;
                    v38 = GRID_gast_Best[v40 + 2].gridShorts.i2;
                    v39 = v95a - 1;
                }
                if (v36 > GRID_gast_Best[v40 + 3].ptrElems->fEval)
                {
                    v36 = GRID_gast_Best[v40 + 3].ptrElems->fEval;
                    v37 = GRID_gast_Best[v40 + 3].gridShorts.i1;
                    v38 = GRID_gast_Best[v40 + 3].gridShorts.i2;
                    v39 = v95a;
                }
                v40 += 4;
                v95a += 4;
            } while (v95a < v35);
            v88 = v38;
            v89 = v37;
        }
        if (v40 < v35)
        {
            do
            {
                if (v36 > GRID_gast_Best[v40].ptrElems->fEval)
                {
                    v36 = GRID_gast_Best[v40].ptrElems->fEval;
                    v37 = GRID_gast_Best[v40].gridShorts.i1;
                    v38 = GRID_gast_Best[v40].gridShorts.i2;
                    v39 = v40;
                }
                ++v40;
            } while (v40 < v35);
            v88 = v38;
            v89 = v37;
        }
        a2->ptrs[v37 + v38 * a2->s3Size7LimitMaxX * a2->gridElemSize].flags = 1;
        v41 = v97;
        v42 = v97 < 2000;
        GRID_gast_Explored[v97].i1 = v37;
        GRID_gast_Explored[v41].i2 = v38;
        if (!v42)
            break;
        v43 = v41;
        v44 = GRID_gast_Best[--v35].ptrElems;
        GRID_gast_Best[v39].ptrElems = v44;
        GRID_gast_Best[v39].gridShorts = GRID_gast_Best[v35].gridShorts;
        v97 = v43 + 1;
        v96 = v35;
        if (v89 == v92 && v88 == v90)
            v100 = 1;
        v45 = gao->matrices;
        v46 = v45->v2.x;
        v106.x = v46;
        v106.y = v45->v2.y;
        v106.z = v45->v2.z;
        if (v46 >= -0.5)
        {
            if (v106.x <= 0.5)
            {
                if (v106.y >= -0.5)
                    v47 = 8;
                else
                    v47 = 0;
            }
            else
            {
                v47 = 4;
            }
        }
        else
        {
            v47 = 12;
        }
        v101 = 0;
        v91 = v47;
        do
        {
            v48 = ai_Scan[v91];
            v49 = v89 + v48 < 0;
            v50 = v89 + v48;
            v98 = v50;
            if (!v49)
            {
                v51 = a2->gridElemSize;
                if (v50 >= v51 * a2->s3Size7LimitMaxX)
                    goto LABEL_92;
                v105 = abs(v50 - a3a);
                if (v105 <= a10)
                {
                    v52 = ai_Scan[v91 + 1];
                    v49 = v88 + v52 < 0;
                    v53 = v88 + v52;
                    v99 = v53;
                    if (!v49)
                    {
                        if (v53 >= v51 * a2->s4Size7LimitMaxY)
                            goto LABEL_92;
                        v104 = abs(v53 - a4a);
                        if (v104 <= a10)
                        {
                            if (v91 >= 8)
                            {
                                v54 = a3->gridElemSize;
                                if ((a3->capasPtr[ai_Scan[v91 + 8] + v54 * a3->s3Size7LimitMaxX * (v88 + ai_Scan[v91 + 9]) + v89] & (GRID_gi_TestWall | 0xC0)) != 0)
                                {
                                    v35 = v96;
                                    goto LABEL_90;
                                }
                                v35 = v96;
                                if ((a3->capasPtr[ai_Scan[v91 + 10] + v54 * a3->s3Size7LimitMaxX * (v88 + ai_Scan[v91 + 11]) + v89] & (GRID_gi_TestWall | 0xC0)) != 0)
                                    goto LABEL_90;
                            }
                            v95 = (float)(int)(abs(v92 - v50) + abs(v90 - v53));
                            if ((v50 != a3a || v53 != a4a) && (v92 != a3a || v90 != a4a))
                            {
                                OBJ_SightGet(gao, &a2a);
                                v55 = (float)v98;
                                v98a = v55;
                                v56 = (float)v99;
                                a1a.z = 0.0;
                                v99a = v56;
                                v57 = (float)a3a;
                                v102 = v57;
                                v103 = (float)a4a;
                                v109.y = v103;
                                a1a.x = v55 - v57;
                                a1a.y = v56 - v103;
                                MATH_NormalizeVector(&a1a, &a1a);
                                v109.x = v102;
                                v109.y = v103;
                                v58 = a2a.z * a1a.z + a2a.y * a1a.y;
                                a2a.z = 0.0;
                                v95 = (float)fabs(1.0f - (v58 + a2a.x * a1a.x)) * 20.0f + v95;
                                a1a.x = (float)v92;
                                a1a.y = (float)v90;
                                a2a.x = v98a - v102;
                                a2a.y = v99a - v103;
                                MATH_NormalizeVector(&a2a, &a2a);
                                a1a.x = a1a.x - v109.x;
                                a1a.z = 0.0;
                                a1a.y = a1a.y - v109.y;
                                MATH_NormalizeVector(&a1a, &a1a);
                                v95 = (float)fabs(1.0 - (a2a.z * a1a.z + a2a.y * a1a.y + a2a.x * a1a.x)) * 30.0f + v95;
                            }
                            v59 = a3->capasPtr[v53 * a3->s3Size7LimitMaxX * a3->gridElemSize + v50];
                            if ((v59 & GRID_gi_TestWall) != 0 && (v59 & 0xC0) != 0
                                || ((v60 = a2->s3Size7LimitMaxX,
                                    v105 += v104,
                                    v61 = a2->capasPtr[v53 * v60 * a2->gridElemSize + v50],
                                    (v61 & GRID_gi_TestWall) == 0)
                                    || a2 != a3 && (v61 & 0xC) != 0)
                                && ((v61 & 0xC0) == 0 || a2 != a3 && (v61 & 0xC) != 0 || v105 > a8))
                            {
                                v62 = &a2->ptrs[v50 + v53 * a2->s3Size7LimitMaxX * a2->gridElemSize];
                                v63 = v62->flags;
                                if (v63 != 1)
                                {
                                    v64 = v95;
                                    if (v63 == 2)
                                    {
                                        v65 = v64 + a2->ptrs[v89 + v88 * a2->s3Size7LimitMaxX * a2->gridElemSize].fEval;
                                        if (v65 < v62->fEval)
                                        {
                                            v62->fEval = v65;
                                            a2->ptrs[v50 + v53 * a2->s3Size7LimitMaxX * a2->gridElemSize].wayX = v89;
                                            a2->ptrs[v50 + v53 * a2->s3Size7LimitMaxX * a2->gridElemSize].wayY = v88;
                                        }
                                    }
                                    else
                                    {
                                        v62->flags = 2;
                                        v66 = a2->s3Size7LimitMaxX * a2->gridElemSize;
                                        a2->ptrs[v50 + v53 * v66].fEval = v64 + a2->ptrs[v89 + v88 * v66].fEval;
                                        a2->ptrs[v50 + v53 * a2->s3Size7LimitMaxX * a2->gridElemSize].wayX = v89;
                                        a2->ptrs[v50 + v53 * a2->s3Size7LimitMaxX * a2->gridElemSize].wayY = v88;
                                        v35 = v96;
                                        v42 = v96 < 1000;
                                        GRID_gast_Best[v96].ptrElems = &a2->ptrs[v50 + v53 * a2->s3Size7LimitMaxX * a2->gridElemSize];
                                        GRID_gast_Best[v35].gridShorts.i1 = v50;
                                        GRID_gast_Best[v35].gridShorts.i2 = v53;
                                        if (!v42)
                                            goto LABEL_92;
                                        v96 = ++v35;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        LABEL_90:
            ++v101;
            v91 = (v91 + 2) % 16;
        } while (v101 < 8);
    } while (v35);
LABEL_92:
    v67 = v97;
    for (i = 0; i < v67; a2->ptrs[v69].flags = 0)
    {
        v69 = GRID_gast_Explored[i].i1 + a2->s3Size7LimitMaxX * a2->gridElemSize * GRID_gast_Explored[i].i2;
        ++i;
    }
    for (j = 0; j < v35; a2->ptrs[v71].flags = 0)
    {
        v71 = GRID_gast_Best[j].gridShorts.i1 + a2->s3Size7LimitMaxX * a2->gridElemSize * GRID_gast_Best[j].gridShorts.i2;
        ++j;
    }
    if (!v100)
        return 0;
    v72 = v90;
    v96 = 0;
    v73 = v92;
    if (v92 != -1)
    {
        v74 = a2->gridElemSize * a2->s3Size7LimitMaxX;
        v75 = a2->ptrs;
        do
        {
            v76 = v73 + v74 * v72;
            v73 = v75[v76].wayX;
            v72 = v75[v76].wayY;
            ++v96;
        } while (v73 != -1);
    }
    v77 = v90;
    v78 = 0;
    v79 = v92;
    v101 = v92;
    v100 = v90;
    if (v92 != -1)
    {
        v80 = a6;
        do
        {
            if (v96 > a11)
            {
                --v96;
            }
            else
            {
                v105 = a2->gridElemSize * a2->s5Limit1MinX;
                v81 = (float)v105 + (float)v101 + 0.5f;
                v80->x = v81;
                v80->x = v81 + a2->startX;
                v105 = a2->gridElemSize * a2->s5Limit1MinY;
                ++v78;
                v82 = (float)v105 + (float)v100 + 0.5f;
                v80->y = v82;
                v83 = v82 + a2->startY;
                v80->y = v83;
                v80->z = 0.0f;
                v80++;
            }
            v84 = v79 + v77 * a2->s3Size7LimitMaxX * a2->gridElemSize;
            v85 = a2->ptrs;
            v77 = v85[v84].wayY;
            v79 = v85[v84].wayX;
            v101 = v79;
            v100 = v77;
        } while (v79 != -1);
    }
    return v78;
}

int GRI_Compress(tWorldGridStruct* a1, char* a2, int a3, int a4, char* a5, int a6)
{
    int v6;
    int v7;
    char* v8;
    int v9;
    char* v10;
    int v11;
    char* v12;
    int v13;
    byte* v14;
    BOOL v15;
    int v16; 
    byte* v17;
    byte* v18;
    int result;
    int v20;
    int v21;
    int v22;
    int v23;
    char v24;
    int v25;

    v6 = a1->gridElemSize;
    v7 = a4;
    v23 = v6 + a3;
    v22 = v6 + a4;
    v8 = a1->capasPtr;
    if (a2 == v8)
    {
        v9 = a1->s3Size7LimitMaxX;
        v10 = &v8[a4 * v6 * v9 + a3];
        v21 = v6 * (v9 - 1);
    }
    else
    {
        v10 = &a1->notUsedSecondaryCapasPtr[a4 * v6 * a1->gridCountX + a3];
        v21 = v6 * (a1->gridCountX - 1);
    }
    v11 = a6 & *v10;
    v12 = v10 + 1;
    v13 = 1;
    v20 = 0;
    v25 = a3 + 1;
    v14 = a5 + 4;
    v24 = 0;
    while (1)
    {
        v15 = v7 == v22;
        v16 = v25;
        if (v15)
            break;
        while (v16 != v23)
        {
            if ((a6 & *v12) != v11)
            {
                ++v20;
                goto LABEL_7;
            }
            ++v13;
        LABEL_20:
            ++v12;
            ++v16;
        }
        v25 = a3;
        v7 = ++a4;
        v12 += v21;
    }
    v24 = 1;
LABEL_7:
    if (v13 == 1)
    {
        if ((v11 & 0xC0) != 0)
        {
            *v14 = 0x80;
            v17 = v14 + 1;
            *v17 = 1;
            v14 = v17 + 1;
        }
    }
    else if (v13 > 63 || (v11 & 0xFFFFFFC0) != 0)
    {
        if (v13 > 255)
        {
            if (v13 > 0xFFFF)
                goto LABEL_18;
            *v14 = 64;
            *(int16_t*)(v14 + 1) = v13;
            v14 += 3;
        }
        else
        {
            *v14 = 0x80;
            v18 = v14 + 1;
            *v18 = v13;
            v14 = v18 + 1;
        }
    }
    else
    {
        *v14++ = v13 | 0xC0;
    }
    *v14++ = v11;
LABEL_18:
    if (!v24)
    {
        v11 = a6 & *v12;
        v13 = 1;
        goto LABEL_20;
    }
    result = v14 - a5;
    *(int32_t*)a5 = result;
    if (!v11 && !v20)
        result = 0;
    return result;
}

void GRI_CompressReal(tWorldGridStruct* a1, int a2, int a3)
{
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    unsigned int v9;
    int id;
    char** v11;
    int* v12;
    char* v13;
    int v14;

    v13 = (char*)MEM_p_Alloc(a1->gridElemSize * a1->gridElemSize + 4);
    v4 = 0;
    v14 = 0;
    if (a1->s3Size7LimitMaxX > 0)
    {
        while (1)
        {
            if (v4 + a1->s5Limit1MinX < a1->gridCountX)
            {
                v5 = a1->s4Size7LimitMaxY;
                v6 = 0;
                if (v5 > 0)
                    break;
            }
        LABEL_20:
            v14 = ++v4;
            if (v4 >= a1->s3Size7LimitMaxX)
                goto LABEL_21;
        }
        while (1)
        {
            v7 = v6 + a1->s5Limit1MinY;
            if (v7 >= a1->gridCountY)
                goto LABEL_20;
            v8 = a1->s5Limit1MinX + v4;
            if (v8 < a2 || v7 < a3 || v8 >= a2 + a1->s3Size7LimitMaxX || v7 >= a3 + v5)
                v9 = 0;
            else
                v9 = GRI_Compress(a1, a1->capasPtr, v14 * a1->gridElemSize, v6 * a1->gridElemSize, v13, 255);
            id = a1->gridCountX * (v6 + a1->s5Limit1MinY) + v14 + a1->s5Limit1MinX;
            if (!v9)
            {
                if (a1->ptr[id])
                {
                    MEM_Free(a1->ptr[id]);
                    a1->ptr[id] = 0;
                }
                goto LABEL_19;
            }
            v11 = a1->ptr;
            v12 = (int *)v11[id];
            if (!v12)
                goto LABEL_15;
            if (v9 != *v12)
                break;
        LABEL_16:
            memcpy(a1->ptr[id], v13, v9);
        LABEL_19:
            v5 = a1->s4Size7LimitMaxY;
            v4 = v14;
            if (++v6 >= v5)
                goto LABEL_20;
        }
        MEM_Free(v11[id]);
    LABEL_15:
        a1->ptr[id] = (char*)MEM_p_Alloc(v9);
        goto LABEL_16;
    }
LABEL_21:
    MEM_Free(v13);
}

void GRI_UnCompress(tWorldGridStruct* a1, char* a2, int a3, int a4)
{
    char* v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int v13;
    char* v14;
    int v15;
    char* v16;
    char* v17;
    int v18;
    int v19;
    int v20;
    int v21;
    int v22;
    int v23; // for routing fix, it was char
    char* v24;
    char* v25;
    char v26;
    int v27;
    int a2a;
    int a3a;
    int a4a;

    v4 = a1->capasPtr;
    v26 = 0;
    if (a2 != v4)
    {
        v10 = a1->gridCountX;
        v9 = a1->gridElemSize;
        v11 = a3 + a4 * v10;
        v12 = a3 * v9;
        v13 = a4 * v9;
        v14 = &a1->notUsedSecondaryCapasPtr[a4 * v9 * v10 * v9];
        goto LABEL_6;
    }
    v5 = a1->s5Limit1MinX;
    v6 = a1->gridCountX;
    if (v5 + a3 < v6)
    {
        v7 = a4 + a1->s5Limit1MinY;
        if (v7 < a1->gridCountY)
        {
            v8 = v5 + v6 * v7;
            v9 = a1->gridElemSize;
            v10 = a1->s3Size7LimitMaxX;
            v11 = a3 + v8;
            v12 = a3 * v9;
            v13 = a4 * v9;
            v14 = &v4[a4 * v9 * v9 * v10];
        LABEL_6:
            v15 = v9 * (v10 - 1);
            v16 = &v14[v12];
            if (a1->ptr[v11])
            {
                v17 = a1->ptr[v11];
            }
            else
            {
                v18 = a1->ptrInts[v11];
                if (v18 == -1)
                    v17 = 0;
                else
                    v17 = &a1->compressedGrid[v18];
            }
            v19 = v12 * v9;
            a3a = v12 * v9;
            a4a = v13 * v9;
            v27 = v9 + v12 * v9;
            a2a = v13 * v9 + v9;
            if (v17)
                v17 += 4;
            v20 = 0;
            v21 = v19;
            while (1)
            {
                if (a4a == a2a)
                    return;
                if (!v20)
                {
                LABEL_18:
                    if (v17)
                    {
                        v22 = (uint8_t)*v17;
                        v23 = *v17 & 0xC0;
                        if (v23)
                        {
                            switch (v23)
                            {
                            case 0xC0:
                                v24 = v17 + 1;
                                v20 = v22 & 0x3F;
                                v26 = *v24;
                                v17 = v24 + 1;
                                break;
                            case 0x80:
                                v20 = (v20 & ~0xff) | v17[1];
                                v25 = v17 + 1;
                                v20 = (uint8_t)v20;
                                v26 = v25[1];
                                v17 = v25 + 2;
                                break;
                            case 0x40:
                                v20 = *(uint16_t*)(v17 + 1);
                                v26 = v17[3];
                                v17 += 4;
                                break;
                            }
                        }
                        else
                        {
                            v20 = 1;
                            v26 = *v17++;
                        }
                    }
                    else
                    {
                        v20 = a1->gridElemSize * a1->gridElemSize;
                        v26 = 0;
                    }
                }
                while (v21 != v27)
                {
                    --v20;
                    ++v21;
                    *v16++ = v26;
                    if (!v20)
                        goto LABEL_18;
                }
                v21 = a3a;
                v16 += v15;
                ++a4a;
            }
        }
    }
}

void GRI_UnCompressReal(tWorldGridStruct* a1)
{
    int v1;
    int v2;
    int i;

    v1 = 0;
    if (a1->s3Size7LimitMaxX > 0)
    {
        v2 = a1->s4Size7LimitMaxY;
        do
        {
            for (i = 0; i < v2; ++i)
            {
                GRI_UnCompress(a1, a1->capasPtr, v1, i);
                v2 = a1->s4Size7LimitMaxY;
            }
            ++v1;
        } while (v1 < a1->s3Size7LimitMaxX);
    }
}

void GRID_SetRealCenter(tWorldGridStruct* a1, int a2, int a3)
{
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int a1a;

    v4 = a1->iInSetCenter1;
    v5 = a1->iInSetCenter2;
    if (v5 - v4 + 1 > 7)
    {
        v7 = (uint32_t)((double)a1->s3Size7LimitMaxX * 0.5 + 0.5);
        v6 = a2 / a1->gridElemSize - v7;
        if (v6 >= v4)
        {
            if (a2 / a1->gridElemSize >= v5)
                v6 = v5 - v7;
        }
        else
        {
            v6 = a1->iInSetCenter1;
        }
    }
    else
    {
        v6 = a1->iInSetCenter1;
    }
    v8 = a1->iInSetCenter4;
    v9 = a1->iInSetCenter3;
    if (v8 - v9 + 1 > 7)
    {
        v10 = (uint32_t)((double)a1->s4Size7LimitMaxY * 0.5 + 0.5);
        v9 = a3 / a1->gridElemSize - v10;
        if (v9 >= a1->iInSetCenter3)
        {
            if (a3 / a1->gridElemSize >= v8)
                v9 = v8 - v10;
        }
        else
        {
            v9 = a1->iInSetCenter3;
        }
    }
    if (v6 != a1->s5Limit1MinX || v9 != a1->s5Limit1MinY)
    {
        GRI_CompressReal(a1, v6, v9);
        a1->s5Limit1MinX = v6;
        a1->s5Limit1MinY = v9;
        GRI_UnCompressReal(a1);
        a1a = a1->gridElemSize * a1->s5Limit1MinY;
        a1->centerX = (float)((double)(a1->gridElemSize * a1->s5Limit1MinX) + a1->startX);
        a1->centerY = (float)((double)a1a + a1->startY);
    }
}

void GRI_Reinit(struct sWorldGridStruct* a1)
{
    int v1;
    int v2;
    int v3;
    int* v4;
    __int16 v5;
    __int16 v6;
    __int16 v7;
    int v8;
    int v9;
    unsigned int v10;
    int i;
    char** v12;
    __int16 v13;

    if (!a1->ptrs)
    {
        v1 = a1->gridCountX;
        v2 = 0;
        a1->iInSetCenter3 = 1000;
        a1->iInSetCenter1 = 1000;
        a1->iInSetCenter4 = -1;
        a1->iInSetCenter2 = -1;
        if (v1 > 0)
        {
            do
            {
                v3 = 0;
                if (a1->gridCountY > 0)
                {
                    v4 = &a1->ptrInts[v2];
                    do
                    {
                        if (*v4 != -1)
                        {
                            if (v2 < a1->iInSetCenter1)
                                a1->iInSetCenter1 = v2;
                            if (v3 < a1->iInSetCenter3)
                                a1->iInSetCenter3 = v3;
                            if (v2 > a1->iInSetCenter2)
                                a1->iInSetCenter2 = v2;
                            if (v3 > a1->iInSetCenter4)
                                a1->iInSetCenter4 = v3;
                        }
                        ++v3;
                        v4 += v1;
                    } while (v3 < a1->gridCountY);
                }
                ++v2;
            } while (v2 < a1->gridCountX);
        }
        if (a1->iInSetCenter2 == -1 || a1->iInSetCenter4 == -1)
        {
            a1->s3Size7LimitMaxX = 0;
            a1->s4Size7LimitMaxY = 0;
        }
        else
        {
            v5 = (int16_t)(a1->iInSetCenter4 & 0xffff) - (int16_t)(a1->iInSetCenter3 & 0xffff);
            v6 = (int16_t)(a1->iInSetCenter2 & 0xffff) - ((int16_t)(a1->iInSetCenter1 & 0xffff)) + 1;
            a1->s3Size7LimitMaxX = v6;
            v7 = v5 + 1;
            a1->s4Size7LimitMaxY = v7;
            if (v6 > 7)
            {
                a1->s3Size7LimitMaxX = 7;
                a1->iInSetCenter1 = 0;
                a1->iInSetCenter2 = v1;
            }
            if (v7 > 7)
            {
                v8 = a1->gridCountY;
                a1->s4Size7LimitMaxY = 7;
                a1->iInSetCenter3 = 0;
                a1->iInSetCenter4 = v8;
            }
        }
        v9 = a1->gridElemSize * a1->gridElemSize * a1->s3Size7LimitMaxX * a1->s4Size7LimitMaxY;
        v10 = 12 * v9;
        if (12 * v9)
            a1->ptrs = (tGridPtrElem*)MEM_p_Alloc(12 * v9);
        LOA_ul_FileTypeSize_39 += v10;
        memset(a1->ptrs, 0, v10);
    }
    for (i = 0; i < a1->gridCountX * a1->gridCountY; ++i)
    {
        v12 = a1->ptr;
        if (v12[i])
        {
            MEM_Free(v12[i]);
            a1->ptr[i] = 0;
        }
    }
    v13 = a1->iInSetCenter3;
    a1->s5Limit1MinX = a1->iInSetCenter1;
    a1->s5Limit1MinY = v13;
    GRI_UnCompressReal(a1);
}

char* GRID_ul_CallbackCompressedGrid(uint32_t Offset)
{
    char* result;
    char* v2;
    char* v3;
    char* v4;
    int v5;
    char* v6;
    char* v7;
    char v8;
    char v9;
    char* a1;
    int size;

    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    if (!size)
    {
        result = (char*)MEM_p_Alloc(4u);
        *(int32_t*)result = 0;
        return result;
    }
    v2 = (char*)MEM_p_Alloc(size);
    LOA_ul_FileTypeSize_39 += size;
    v3 = &a1[size];
    v4 = v2;
    if (a1 < &a1[size])
    {
        while (1)
        {
            v5 = LOA_ReadLong(&a1, (uint32_t*)v4, 1);
            v6 = a1;
            v7 = &a1[v5 - 4];
            v4 += 4;
            if (a1 < v7)
                break;
        LABEL_13:
            if (v6 >= v3)
                return v2;
        }
        while (1)
        {
            v8 = LOA_ReadChar(&a1, 0, 1);
            *v4++ = v8;
            if ((v8 & 0xC0) == 0xFFFFFFC0)
                break;
            if (v8 == (char)0x80)
            {
                *v4++ = LOA_ReadChar(&a1, 0, 1);
                v9 = LOA_ReadChar(&a1, 0, 1);
                goto LABEL_11;
            }
            if (v8 == 64)
            {
                LOA_ReadShort(&a1, (uint16_t*)v4, 1);
                v4 += 2;
                v9 = LOA_ReadChar(&a1, 0, 1);
                goto LABEL_11;
            }
        LABEL_12:
            v6 = a1;
            if (a1 >= v7)
                goto LABEL_13;
        }
        v9 = LOA_ReadChar(&a1, 0, 1);
    LABEL_11:
        *v4++ = v9;
        goto LABEL_12;
    }
    return v2;
}

tWorldGridStruct* GRID_ul_CallbackWorldGrid(uint32_t Offset)
{
    tWorldGridStruct* v1;
    int v2;
    int* v3;
    int v4;
    char** v5;
    int v6;
    int* v7;
    int i;
    unsigned int v10;
    char* a1;
    int size;

    GRID_gi_Current = 0;
    v1 = (tWorldGridStruct*)MEM_p_Alloc(sizeof(tWorldGridStruct));
    LOA_ul_FileTypeSize_39 += sizeof(tWorldGridStruct);
    memset(v1, 0, sizeof(tWorldGridStruct));
    a1 = BIG_pc_ReadFileTmp(Offset, &size);
    LOA_ReadLong(&a1, 0, 2);
    LOA_ReadLong(&a1, 0, 2);
    v1->compressedGrid = (char*)LOA_ReadLong(&a1, 0, 1);
    LOA_ReadLong(&a1, 0, 2);
    LOA_ReadLong(&a1, 0, 2);
    LOA_ReadLong(&a1, 0, 2);
    v1->startX = LOA_ReadFloat(&a1, 0, 1);
    v1->startY = LOA_ReadFloat(&a1, 0, 1);
    v1->centerX = LOA_ReadFloat(&a1, 0, 1);
    v1->centerY = LOA_ReadFloat(&a1, 0, 1);
    v1->gridCountX = LOA_ReadShort(&a1, 0, 1);
    v1->gridCountY = LOA_ReadShort(&a1, 0, 1);
    LOA_ReadShort(&a1, 0, 2);
    LOA_ReadShort(&a1, 0, 2);
    v1->s5Limit1MinX = LOA_ReadShort(&a1, 0, 1);
    v1->s5Limit1MinY = LOA_ReadShort(&a1, 0, 1);
    v1->gridElemSize = LOA_ReadChar(&a1, 0, 1);
    LOA_ReadLong(&a1, 0, 2);
    LOA_ReadChar(&a1, 0, 2);
    LOA_ReadChar(&a1, 0, 2);
    LOA_ReadChar(&a1, 0, 2);
    v2 = v1->gridCountX;
    v1->s3Size7LimitMaxX = 7;
    v1->s4Size7LimitMaxY = 7;
    v10 = 4 * v1->gridCountY * v2;
    v1->i2 = 0;
    v3 = (int*)MEM_p_Alloc(v10);
    v4 = v1->gridCountY * v1->gridCountX;
    v1->ptrInts = v3;
    LOA_ul_FileTypeSize_39 += 4 * v4;
    v5 = (char**)MEM_p_Alloc(4 * v1->gridCountY * v1->gridCountX);
    v6 = v1->gridCountY * v1->gridCountX;
    v1->ptr = v5;
    LOA_ul_FileTypeSize_39 += 4 * v6;
    memset(v1->ptr, 0, 4 * v1->gridCountY * v1->gridCountX);
    v7 = v1->ptrInts;
    for (i = 0; i < v1->gridCountY * v1->gridCountX; ++v7)
    {
        *v7 = LOA_ReadLong(&a1, 0, 1);
        ++i;
    }
    
    LOA_MakeFileRef((int)v1->compressedGrid, &v1->compressedGrid, GRID_ul_CallbackCompressedGrid, 1);

    size = v1->s3Size7LimitMaxX * v1->gridElemSize;
    size *= v1->s4Size7LimitMaxY * v1->gridElemSize;
    v1->capasPtr = (char*)MEM_p_Alloc(size);
    LOA_ul_FileTypeSize_39 += size;
    v1->ptrs = 0;
    return v1;
}

void GRI_Destroy(tWorldGridStruct* a1)
{
    int i;
    char** v2;

    if (a1)
    {
        for (i = 0; i < a1->gridCountX * a1->gridCountY; ++i)
        {
            v2 = a1->ptr;
            if (v2[i])
                MEM_Free(v2[i]);
        }
        if (a1->compressedGrid)
        {
            LOA_DeleteAddress((int)a1->compressedGrid);
            MEM_Free(a1->compressedGrid);
        }
        if (a1->ptrInts)
            MEM_Free(a1->ptrInts);
        if (a1->ptr)
            MEM_Free(a1->ptr);
        if (a1->capasPtr)
            MEM_Free(a1->capasPtr);
        if (a1->ptrs)
            MEM_Free(a1->ptrs);
        LOA_DeleteAddress((int)a1);
        MEM_Free(a1);
    }
}

BOOL GRI_Surround(tWorldGridStruct* a1, Vec3* a2, Vec3* a3)
{
    int v3;
    int v4;
    int v6;
    int v7;
    int v8;
    float v9;
    float v10;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    float v16;
    Vec3 v17;

    v3 = 0;
    v4 = GRID_gi_TestWall | 0xC0;
    v8 = 0;
    if (fabs(a3->x) >= fabs(a3->y))
    {
        v12 = (float)(a2->y + 1.0);
        v17.x = a2->x;
        v17.y = v12;
        if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
            v3 = 1;
        v16 = (float)(a2->y - 1.0);
        v17.x = a2->x;
        v17.y = v16;
        if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
            v8 = 1;
        if (v3 && v8)
            return 1;
        if (a3->x > 0.0)
        {
            v10 = (float)(a2->x + 1.0);
            v17.y = v12;
            v17.x = v10;
            if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
                ++v3;
            if (v3 && v8)
                return 1;
            v17.x = v10;
            v17.y = v16;
            if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
                ++v8;
            if (v3 && v8)
                return 1;
        }
        if (a3->x < 0.0)
        {
            v14 = (float)(a2->x - 1.0);
            v17.y = v12;
            v17.x = v14;
            if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
                ++v3;
            v6 = v8;
            if (v3 && v8)
                return 1;
            v17.x = v14;
            v17.y = v16;
            v7 = GRI_i_GetCapa(a1, &v17);
            goto LABEL_45;
        }
    }
    else
    {
        v13 = (float)(a2->x - 1.0);
        v17.y = a2->y;
        v17.x = v13;
        if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
            v3 = 1;
        v9 = (float)(a2->x + 1.0);
        v17.y = a2->y;
        v17.x = v9;
        if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
            v8 = 1;
        if (v3 && v8)
            return 1;
        if (a3->y > 0.0)
        {
            v11 = (float)(a2->y + 1.0);
            v17.x = v13;
            v17.y = v11;
            if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
                ++v3;
            if (v3 && v8)
                return 1;
            v17.x = v9;
            v17.y = v11;
            if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
                ++v8;
            if (v3 && v8)
                return 1;
        }
        if (a3->y < 0.0)
        {
            v15 = (float)(a2->y - 1.0);
            v17.x = v13;
            v17.y = v15;
            if ((GRI_i_GetCapa(a1, &v17) & v4) != 0)
                ++v3;
            v6 = v8;
            if (v3 && v8)
                return 1;
            v17.x = v9;
            v17.y = v15;
            v7 = GRI_i_GetCapa(a1, &v17);
        LABEL_45:
            if ((v7 & v4) != 0)
                ++v6;
            return v3 && v6;
        }
    }
    return 0;
}

int GRI_GetTag(tWorldGridStruct* a1, Vec3* a2, int a3, int a4, Vec3* a5)
{
    Vec3* v5;
    Vec3* v6;
    float v7;
    int result;
    float v9;
    double v11;
    float v14; 
    double v16;
    double v19;
    double v20;
    double v22;
    float v25;
    float v26;
    float v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float v37;
    int a1a, a2a;
    float fvar1;

    v5 = a2;
    GRI_3Dto2D(a1, a2, &a2a, &a1a);
    v6 = a5;
    v7 = v5->z;
    v25 = v5->x;
    result = 1;
    v9 = v5->y;
    *a5 = *v5;
    if (a3)
    {
        if (a3 != 1 && a3 == 2)
        {
            v6[1].y = v9;
            v6[1].z = v7;
            v29 = (float)(v25 - 1.0);
            v6[1].x = v29;
            v6[2].y = v9;
            v6[2].z = v7;
            v30 = (float)(v29 + 2.0);
            v6[2].x = v30;
            v6[3].x = v30;
            v6[3].z = v7;
            v36 = (float)(v9 - 1.0);
            v6[3].y = v36;
            v6[4].y = v36;
            v6[4].z = v7;
            v31 = (float)(v30 - 1.0);
            v6[4].x = v31;
            v6[5].y = v36;
            v6[5].z = v7;
            v32 = (float)(v31 - 1.0);
            v6[5].x = v32;
            v6[6].x = v32;
            v6[6].z = v7;
            v37 = (float)(v36 + 2.0);
            v6[6].y = v37;
            v6[7].y = v37;
            v6[7].z = v7;
            v33 = (float)(v32 + 1.0);
            v6[7].x = v33;
            v6[8].y = v37;
            v6[8].z = v7;
            result = 9;
            v34 = (float)(v33 + 1.0);
            v6[8].x = v34;
        }
    }
    else
    {
        v6[1].z = v7;
        v6[1].y = v9;
        v11 = v25;
        fvar1 = (float)fabs(((float)a2a + a1->startX) - a2->x);
        if (fvar1 > 0.5)
        {
            v28 = (float)(v11 - 1.0);
            v14 = v28;
            v6[1].x = v28;
            v22 = v9;
            fvar1 = (float)fabs(((float)a1a + a1->startY) - a2->y);
            if (fvar1 <= 0.5)
                v35 = (float)(v22 - 1.0);
            else
                v35 = (float)(v22 + 1.0);
            v20 = (float)(v28 + 1.0);
        }
        else
        {
            v26 = (float)(v11 + 1.0);
            v14 = v26;
            v6[1].x = v26;
            v16 = v9;
            fvar1 = (float)fabs(((float)a1a + a1->startY) - a2->y);
            if (fvar1 <= 0.5)
                v19 = (float)(v16 - 1.0);
            else
                v19 = (float)(v16 + 1.0);
            v35 = (float)v19;
            v20 = (float)(v26 - 1.0);
        }
        v27 = (float)v20;
        v6[2].x = v14;
        v6[2].y = v35;
        v6[2].z = v7;
        v6[3].x = v27;
        v6[3].y = v35;
        v6[3].z = v7;
        result = 4;
    }
    return result;
}

int GRI_Tag(tWorldGridStruct* a1, Vec3* a2, int a3, int a4)
{
    int v4;
    int v5;
    Vec3* v6;
    Vec3* v7;
    Vec3 v9[20];

    v4 = GRI_GetTag(a1, a2, a3, a4, v9);
    v5 = 0;
    if (v4 > 0)
    {
        v6 = v9;
        do
        {
            if (!GRI_AddCapa(a1, v6, a4))
                break;
            ++v5;
            ++v6;
        } while (v5 < v4);
    }
    if (v5 == v4)
        return 1;
    if (v5 > 0)
    {
        v7 = v9;
        do
        {
            GRI_AddCapa(a1, v7++, -(char)a4);
            --v5;
        } while (v5);
    }
    return 0;
}

int GRI_TagBox(struct sWorldGridStruct* a1, Vec3* pos, Vec3* a3, char a4)
{
    float v4;
    float v5;
    double v6;
    int v7;
    double v8;
    Vec3* v9;
    int v10;
    Vec3* v11;
    Vec3* v12;
    int result;
    float v14;
    float v15;
    int retPosX;
    float v17;
    float v18;
    int retPosY;
    Vec3 a2[200];

    GRI_3Dto2D(a1, pos, &retPosX, &retPosY);
    v14 = (float)((double)retPosX + a1->startX);
    v15 = (float)((double)retPosY + a1->startY);
    GRI_3Dto2D(a1, a3, &retPosY, &retPosX);
    v17 = (float)((double)retPosY + a1->startX);
    v18 = (float)((double)retPosX + a1->startY);
    if (v14 > (double)v17)
    {
        v4 = v14;
        v14 = v17;
        v17 = v4;
    }
    if (v15 > (double)v18)
    {
        v5 = v15;
        v15 = v18;
        v18 = v5;
    }
    v6 = v14;
    v7 = 0;
    if (v14 > (double)v17)
    {
    LABEL_11:
        v10 = 0;
        if (v7 > 0)
        {
            v11 = a2;
            do
            {
                if (!GRI_AddCapa(a1, v11, a4))
                    break;
                ++v10;
                ++v11;
            } while (v10 < v7);
        }
        if (v10 == v7)
        {
            result = 1;
        }
        else
        {
            if (v10 > 0)
            {
                v12 = a2;
                do
                {
                    GRI_AddCapa(a1, v12++, -a4);
                    --v10;
                } while (v10);
            }
            result = 0;
        }
    }
    else
    {
        while (1)
        {
            v8 = v15;
            if (v15 <= (double)v18)
                break;
        LABEL_10:
            v6 = v6 + 1.0;
            if (v6 > v17)
                goto LABEL_11;
        }

        v9 = &a2[v7];
        while (1)
        {
            ++v7;
            v9->x = (float)v6;
            v9->y = (float)v8;
            ++v9;
            if (v7 == 200)
                break;
            v8 = v8 + 1.0;
            if (v8 > v18)
                goto LABEL_10;
        }
        result = 0;
    }
    return result;
}

int GRI_GetTagVec(tWorldGridStruct* a1, Vec3* pos, int a3, int a4, Vec3* a5, Vec3* a6)
{
    Vec3* v6;
    float v7;
    Vec3* v8;
    float v9;
    float v10;
    double v12;
    double v15;
    int result;
    double v18;
    double v21;
    float v22;
    float v23;
    float v24;
    int retPosX, retPosY;
    float fVar1, fVar2;

    v6 = pos;  
    GRI_3Dto2D(a1, pos, &retPosX, &retPosY);
    fVar1 = a1->startX;
    fVar2 = a1->startY;
    v7 = v6->y;
    v8 = a6;
    v9 = v6->z;
    v10 = v6->x;
    *a6 = *v6;
    v24 = a5->x;
    if (fabs(a5->y) > fabs(a5->x))
        v24 = 0.0;
    if (a3)
        return 1;
    if (v24 == 0.0)
    {
        v12 = v7;
        fVar1 = (float)fabs(((float)retPosY + fVar2) - pos->y);
        if (fVar1 <= 0.5f)        
            v15 = v12 - 1.0;
        else
            v15 = v12 + 1.0;
        v23 = (float)v15;
        v8[1].x = v10;
        v8[1].y = v23;
        result = 2;
        v8[1].z = v9;
    }
    else
    {
        v18 = v10;
        fVar1 = (float)fabs(((float)retPosX + fVar1) - pos->x);
        if (fVar1 <= 0.5f)
            v21 = v18 - 1.0;
        else
            v21 = v18 + 1.0;
        v22 = (float)v21;
        v8[1].x = v22;
        v8[1].y = v7;
        result = 2;
        v8[1].z = v9;
    }
    return result;
}

int GRI_TagVec(struct sWorldGridStruct* a1, Vec3* pos, int a3, int a4, Vec3* a5)
{
    int v5;
    int v6;
    Vec3* v7;
    Vec3* v8;
    Vec3 a6[20];

    v5 = GRI_GetTagVec(a1, pos, a3, a4, a5, a6);
    v6 = 0;
    if (v5 > 0)
    {
        v7 = a6;
        do
        {
            if (!GRI_AddCapa(a1, v7, a4))
                break;
            ++v6;
            ++v7;
        } while (v6 < v5);
    }
    if (v6 == v5)
        return 1;
    if (v6 > 0)
    {
        v8 = a6;
        do
        {
            GRI_AddCapa(a1, v8++, -(char)a4);
            --v6;
        } while (v6);
    }
    return 0;
}


int GRI_GetTagDyn(struct sWorldGridStruct* grid, Vec3* pos, float distLimit, Vec3* gridPosRet)

{
    float gridRealStartPosX;
    float gridRealStartPosY;
    float distX;
    float distY;
    int retValX, retValY;

/*
// x 0 3 6 9
// y 1 4 7 10
// z 2 5 8 11
*/

    GRI_3Dto2D(grid, pos, &retValX, &retValY);
    
    gridRealStartPosX = (float)retValX + grid->startX + 0.5f;
    gridRealStartPosY = (float)retValY + grid->startY + 0.5f;
    distX = pos->x - gridRealStartPosX;
    distY = pos->y - gridRealStartPosY;
    
    gridPosRet->x = gridRealStartPosX;
    gridPosRet->y = gridRealStartPosY;
    gridPosRet->z = 0.0f;

    distLimit = 0.5f - distLimit;
    if (distX > distLimit) {
        gridPosRet[1].z = 0.0f; 
        gridPosRet[1].y = gridRealStartPosY;
        gridPosRet[1].x = gridRealStartPosX;

        if (distY > distLimit) {
            gridPosRet[1].x = gridRealStartPosX + 1.0f;
            gridPosRet[2].x = gridRealStartPosX;
            gridPosRet[2].y = gridRealStartPosY + 1.0f;
            gridPosRet[2].z = 0.0f;
            gridPosRet[3].x = gridRealStartPosX + 1.0f;
            gridPosRet[3].y = gridRealStartPosY + 1.0f;
            gridPosRet[3].z = 0.0f;
            return 4;
        }

        gridPosRet[1].x = gridPosRet[1].x + 1.0f;
        if (-distLimit > distY) {
            gridPosRet[2].x = gridRealStartPosX;
            gridPosRet[2].y = gridRealStartPosY - 1.0f;
            gridPosRet[2].z = 0.0f;
            gridPosRet[3].x = gridRealStartPosX + 1.0f;
            gridPosRet[3].y = gridRealStartPosY - 1.0f;
            gridPosRet[3].z = 0.0f;
            return 4;
        }
    }
    else {
        if (-distLimit <= distX) {
            if (distY <= distLimit) {
                if (distY < -distLimit) {
                    gridPosRet[1].x = gridRealStartPosX;
                    gridPosRet[1].y = gridRealStartPosY - 1.0f;
                    gridPosRet[1].z = 0.0f;
                    return 2;
                }
                return 1;
            }
            gridPosRet[1].x = gridRealStartPosX;
            gridPosRet[1].y = gridRealStartPosY + 1.0f;
            gridPosRet[1].z = 0.0f;
        }
        else {
            gridPosRet[1].x = gridRealStartPosX;
            gridPosRet[1].y = gridRealStartPosY;
            gridPosRet[1].z = 0.0f;

            if (distY > distLimit) {
                gridPosRet[1].x = gridRealStartPosX - 1.0f;
                gridPosRet[2].x = gridRealStartPosX;
                gridPosRet[2].y = gridRealStartPosY + 1.0f;
                gridPosRet[2].z = 0.0f;
                gridPosRet[3].x = gridRealStartPosX - 1.0f;
                gridPosRet[3].y = gridRealStartPosY + 1.0f;
                gridPosRet[3].z = 0.0f;
                return 4;
            }
            gridPosRet[1].x = gridRealStartPosX - 1.0f;
            if (distY < -distLimit) {
                gridPosRet[2].x = gridRealStartPosX;
                gridPosRet[2].y = gridRealStartPosY - 1.0f;
                gridPosRet[2].z = 0.0f;
                gridPosRet[3].x = gridRealStartPosX - 1.0f;
                gridPosRet[3].y = gridRealStartPosY - 1.0f;
                gridPosRet[3].z = 0.0f;
                return 4;
            }
        }
    }
    return 2;
}

int GRI_TagDyn(struct sWorldGridStruct* grid, Vec3* a2, float a3, Vec3* pos, char capability)
{
    int num;
    int i, j;

    num = GRI_GetTagDyn(grid, a2, a3, pos);
    
    for (i = 0; i < num; i++)
    {
        if (!GRI_AddCapa(grid, &pos[i], capability))
            break;
    }

    if (i == num)
        return 1;

    for (j = 0; j < i; j++) {
        GRI_AddCapa(grid, &pos[j], -capability);
    }
    
    return 0;
}
