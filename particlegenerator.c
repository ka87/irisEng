#include "particlegenerator.h"
#include "modifiers/stpmodifier.h"
#include "time.h"
#include "memory.h"
#include "gameobject.h"

int PAGRenderingEnabled = 1;

void PAG_Update(struct sGameObjectMain* a1, struct sPAGStruct* a2, float dt)
{
    double v4;
    tPAGElemStruct* v5;
    int v6;
    int v7;
    double v8;
    unsigned int v9;
    __int16 v10;
    int v11;
    int v12;
    int i;
    double v14;
    int v15;
    double v19;
    double v20;
    float v23;
    int v24;
    int v25;
    int v26;
    float v27;
    float v28;
    float v31;
    float v32;
    float v33;
    float v34;
    float v35;
    float v36;
    float dta;
    float fvar4;
    float fvar2;

    v4 = dt + a2->timeElapsed;
    v5 = a2->pagElems;
    v6 = a2->triangleNum2;
    v7 = (unsigned int)(60.0 * v4);
    v24 = v7;
    v36 = (float)((double)v7 * 0.016666999);
    a2->timeElapsed = (float)(v4 - v36);
    if (!v6)
        return;
    do
    {
        --v6;
        for (; v5->ff2 < (double)v36; v5->ff = (float)(1.0 / v8))
        {
            if ((v5->particleState & 1) != 0)
            {
                v27 = a2->ffb14;
                dta = a2->ffb13;
                v25 = rand();
                v5->particleState = 0;
                v8 = (double)v25 * 0.000030518509 * (v27 - dta) + dta;
            }
            else
            {
                if ((v5->particleState & 2) != 0)
                {
                    v12 = a2->triangleNum2 - 1;
                    a2->triangleNum2 = v12;
                    if (v6)
                    {
                        memcpy(v5, &a2->pagElems[v12], sizeof(tPAGElemStruct));
                        v7 = v24;
                        goto LABEL_45;
                    }
                    return;
                }
                v28 = a2->ffb16;
                v23 = a2->ffb15;
                v26 = rand();
                v5->particleState = 2;
                v8 = (double)v26 * 0.000030518509 * (v28 - v23) + v23;
            }
            v5->ff2 = (float)v8;
        }
        v5->ff2 = v5->ff2 - v36;
        v5->fff = v36 + v5->fff;
        if ((a2->flags & 0x1000) != 0)
        {
            v9 = v5->f5;
            if ((v9 & 0x8000) == 0)
            {
                v11 = 65537 * v9;
            }
            else
            {
                v10 = v5->f5;
                
                v5->f5 &= 0x0000ffff; //HIWORD(v5->f5) = 0;
                v11 = ((int16_t)(((v9 >> 16) & 0xffff) - (v10 & 0x7FFF)) << 16) | v5->f5;
            }
            v5->f5 = v11;
        }
        if (v7)
        {
            for (i = v7; i; --i)
            {
                v31 = (float)(a2->v1.x * 0.016666999);           // MATH_AddScaleVector inline x2
                v32 = (float)(a2->v1.y * 0.016666999);
                v33 = (float)(a2->v1.z * 0.016666999);
                v34 = (float)(v33 * 0.0083330004);
                v5->particlePos.x = (float)(v31 * 0.0083330004 + v5->particlePos.x);
                v5->particlePos.y = (float)(v5->particlePos.y + v32 * 0.0083330004);
                v5->particlePos.z = v5->particlePos.z + v34;
                v14 = v5->v1.y * 0.016666999;
                v35 = (float)(v5->v1.z * 0.016666999);
                v5->particlePos.x = (float)(v5->v1.x * 0.016666999 + v5->particlePos.x);
                v5->particlePos.y = (float)(v5->particlePos.y + v14);
                v5->particlePos.z = v5->particlePos.z + v35;
                if ((a2->flags & 0x8000u) != 0)
                {
                    v5->v1.x = v31 + v5->v1.x;
                    v5->v1.y = v32 + v5->v1.y;
                    v5->v1.z = v33 + v5->v1.z;
                }
                v15 = a2->flags;
                if ((v15 & 0x400) != 0 && v5->particlePos.z - v5->ffff2 < a2->i3)
                {
                    fvar4 = v5->particlePos.x - a1->matrices->Pos.x;
                    fvar2 = v5->particlePos.y - a1->matrices->Pos.y;
                    fvar2 = fvar4 * fvar4 + fvar2 * fvar2;
                    if (a2->ffa9 != 0.0 && fvar2 <= a2->ffa9)
                        goto LABEL_29;
                    v5->particlePos.z = v5->ffff2 + a2->i3;
                    v19 = -(a2->ff * v5->v1.z);
                }
                else if ((v15 & 0x800) != 0 && v5->ffff2 + v5->particlePos.z > a2->ffa1)
                {
                    v5->particlePos.z = a2->ffa1 - v5->ffff2;
                    v19 = -(a2->ffa2 * v5->v1.z);
                }
                else
                {
                    v5->v1.x = v31 + v5->v1.x;
                    v5->v1.y = v32 + v5->v1.y;
                    v19 = v33 + v5->v1.z;
                }
                v5->v1.z = (float)v19;
            LABEL_29:
                if ((a2->flags & 0x2000) != 0)
                {
                    v20 = a2->f1;
                    v5->v1.x = (float)(v20 * v5->v1.x);
                    v5->v1.y = (float)(v20 * v5->v1.y);
                    v5->v1.z = (float)(v20 * v5->v1.z);
                }
            }
        }
        if (a2->ffa7 != 0.0)
        {
            v5->particlePos.x = MATH_GetSin(a2->ffa7 * v5->fff) * v36 + v5->particlePos.x;
        }
        if (a2->ffa8 != 0.0)
        {
            v5->particlePos.y = MATH_GetSin(v5->fff * a2->ffa8) * v36 + v5->particlePos.y;
        }
        ++v5;
    LABEL_45:
        ;
    } while (v6);
}

void PAG_Generate(tGameObjectMain* a1, tPAGStruct* a2, float a3)
{
    int num; // esi
    tGEOGRO* v4; // eax
    tModifier* v5; // eax
    tStpMdfStruct* v6; // eax
    Mtx4x4* v7; // edi
    tPAGGeneratorElemStruct* v8; // esi
    tPAGGeneratorElemStruct* v9; // edx
    double v10; // st7
    int v11; // ecx
    int v12; // eax
    Mtx4x4* v13; // eax
    tPAGElemStruct* v14; // esi
    double v15; // st7
    double v16; // st7
    int v17; // eax
    double v18; // st7
    double v19; // st7
    double v20; // st7
    double v21; // st7
    int v22; // eax
    double v23; // st7
    double v25; // st7
    int v26; // eax
    double v27; // st7
    double v29; // st7
    double v30; // st7
    int v31; // eax
    double v32; // st7
    double v34; // st7
    double v36; // st7
    int v37; // eax
    double v38; // st7
    int v39; // eax
    double v40; // st7
    double v41; // st7
    Vec3* v42; // ecx
    double v43; // st7
    int v44; // eax
    double v45; // st7
    double v47; // st7
    double v48; // st7
    int v49; // eax
    int v50; // eax
    double v51; // st7
    double v53; // st7
    double v55; // st7
    double v56; // st7
    double v60; // st7
    double v61; // st7
    Vec3* v62; // eax
    double v63; // st7
    double v67; // st7
    BOOL v68; // zf
    int v69; // [esp+Ch] [ebp-18Ch]
    int v70; // [esp+Ch] [ebp-18Ch]
    int v71; // [esp+Ch] [ebp-18Ch]
    int v72; // [esp+Ch] [ebp-18Ch]
    int v73; // [esp+Ch] [ebp-18Ch]
    float v74; // [esp+Ch] [ebp-18Ch]
    float v75; // [esp+Ch] [ebp-18Ch]
    float v76; // [esp+Ch] [ebp-18Ch]
    float v77; // [esp+Ch] [ebp-18Ch]
    float v78; // [esp+Ch] [ebp-18Ch]
    float v79; // [esp+10h] [ebp-188h]
    float v80; // [esp+10h] [ebp-188h]
    float v81; // [esp+10h] [ebp-188h]
    float v82; // [esp+10h] [ebp-188h]
    float v83; // [esp+10h] [ebp-188h]
    float v84; // [esp+10h] [ebp-188h]
    float v85; // [esp+14h] [ebp-184h]
    float v86; // [esp+14h] [ebp-184h]
    float v87; // [esp+14h] [ebp-184h]
    float v89; // [esp+14h] [ebp-184h]
    float v90; // [esp+14h] [ebp-184h]
    float v94; // [esp+18h] [ebp-180h]
    float v98; // [esp+18h] [ebp-180h]
    tPAGGeneratorElemStruct* v101; // [esp+24h] [ebp-174h]
    Mtx4x4* v102; // [esp+28h] [ebp-170h]
    tPAGGeneratorElemStruct* v103; // [esp+2Ch] [ebp-16Ch]
    tPAGGeneratorElemStruct* v104; // [esp+30h] [ebp-168h]
    tGEOGRO* v105; // [esp+34h] [ebp-164h]
    int nnum; // [esp+38h] [ebp-160h]
    float v107; // [esp+3Ch] [ebp-15Ch]
    float v108; // [esp+40h] [ebp-158h]
    float v109; // [esp+44h] [ebp-154h]
    float v110; // [esp+48h] [ebp-150h]
    Vec3* v111 = NULL; // [esp+4Ch] [ebp-14Ch]
    float v112; // [esp+50h] [ebp-148h]
    float v113; // [esp+54h] [ebp-144h]
    float v114; // [esp+58h] [ebp-140h]
    float v115; // [esp+5Ch] [ebp-13Ch]
    int v116; // [esp+60h] [ebp-138h]
    float v117; // [esp+64h] [ebp-134h]
    float v118; // [esp+68h] [ebp-130h]
    float v120; // [esp+6Ch] [ebp-12Ch]
    float v121; // [esp+70h] [ebp-128h]
    float v122; // [esp+74h] [ebp-124h]
    float v123; // [esp+78h] [ebp-120h]
    float v124; // [esp+7Ch] [ebp-11Ch]
    float v125; // [esp+80h] [ebp-118h]
    float v126; // [esp+84h] [ebp-114h]
    float v127; // [esp+88h] [ebp-110h]
    float v128; // [esp+8Ch] [ebp-10Ch]
    float v129; // [esp+90h] [ebp-108h]
    float v130; // [esp+94h] [ebp-104h]
    float v131; // [esp+98h] [ebp-100h]
    float v132; // [esp+9Ch] [ebp-FCh]
    float v133; // [esp+A0h] [ebp-F8h]
    float v134; // [esp+A4h] [ebp-F4h]
    float v135; // [esp+A8h] [ebp-F0h]
    float v136; // [esp+ACh] [ebp-ECh]
    int v137; // [esp+B0h] [ebp-E8h]
    float v138; // [esp+B4h] [ebp-E4h]
    float v139; // [esp+B8h] [ebp-E0h]
    float v140; // [esp+BCh] [ebp-DCh]
    float v141; // [esp+C0h] [ebp-D8h]
    float v142; // [esp+C4h] [ebp-D4h]
    float v143; // [esp+C8h] [ebp-D0h]
    float v144; // [esp+D0h] [ebp-C8h]
    float v145; // [esp+D4h] [ebp-C4h]
    float v146; // [esp+DCh] [ebp-BCh]
    float v147; // [esp+E0h] [ebp-B8h]
    float v148; // [esp+E8h] [ebp-B0h]
    float v149; // [esp+ECh] [ebp-ACh]
    float v150; // [esp+F4h] [ebp-A4h]
    float v151; // [esp+F8h] [ebp-A0h]
    float v152; // [esp+100h] [ebp-98h]
    float v153; // [esp+104h] [ebp-94h]
    float v154; // [esp+10Ch] [ebp-8Ch]
    float v155; // [esp+110h] [ebp-88h]
    float v156; // [esp+118h] [ebp-80h]
    float v157; // [esp+11Ch] [ebp-7Ch]
    float v158; // [esp+124h] [ebp-74h]
    float v159; // [esp+128h] [ebp-70h]
    float v160; // [esp+130h] [ebp-68h]
    float v161; // [esp+134h] [ebp-64h]
    float v162; // [esp+13Ch] [ebp-5Ch]
    float v163; // [esp+140h] [ebp-58h]
    float v164; // [esp+148h] [ebp-50h]
    float v165; // [esp+14Ch] [ebp-4Ch]
    float v166; // [esp+154h] [ebp-44h]
    float v167; // [esp+158h] [ebp-40h]
    float v168; // [esp+160h] [ebp-38h]
    float v169; // [esp+164h] [ebp-34h]
    float v170; // [esp+16Ch] [ebp-2Ch]
    float v171; // [esp+170h] [ebp-28h]
    float v172; // [esp+178h] [ebp-20h]
    float v173; // [esp+17Ch] [ebp-1Ch]
    float v174; // [esp+184h] [ebp-14h]
    float v175; // [esp+188h] [ebp-10h]
    float v176; // [esp+190h] [ebp-8h]
    float v177; // [esp+194h] [ebp-4h]

    num = a2->num;
    nnum = num;
    v4 = (tGEOGRO*)OBJ_p_GetGro(a2->gao1);
    v105 = v4;
    if (v4 && v4->intf->id != 1)
    {
        v4 = 0;
        v105 = 0;
    }
    if (a2->mode == 6)
    {
        if (!v4)
            return;
        v111 = v4->vertices;
        v5 = MDF_pst_GetByType(a2->gao1, 21);       // stp
        if (v5)
        {
            v6 = (tStpMdfStruct*)v5->mdfPrivateDataPtr;
            if (v6->hasStp)
                v111 = v6->verticesStored;
        }
        if (a2->gaoForMatrixRef)
            v7 = OBJ_pst_GetAbsoluteMatrix(a2->gaoForMatrixRef);
        else
            v7 = OBJ_pst_GetAbsoluteMatrix(a2->gao1);
    }
    else
    {
        v7 = NULL; // TODO FIXME: ez a kódban is hibás
    }
    if (num)
    {
        v8 = a2->generators;
        v101 = a2->generators;
        v104 = a2->generators;
        v103 = a2->generators;
        do
        {
            if (v8->generatorFreq == 0.0)
            {
                ++v8;
                v9 = v101 + 1;
                v104 = v8;
                ++v103;
            }
            else
            {
                v10 = a3 + v101->fff22;
                v69 = (unsigned int)(v10 * v8->generatorFreq);
                v101->fff22 = (float)(v10 - (double)v69 / v8->generatorFreq);
                v11 = a2->triangleNum2;
                v12 = a2->triangleNum;
                if (v11 + v69 >= v12)
                    v69 = v12 - v11 - 1;
                if (v69 > 0)
                {
                    if (v103->gao)
                        v13 = OBJ_pst_GetAbsoluteMatrix(v103->gao);
                    else
                        v13 = OBJ_pst_GetAbsoluteMatrix(a1);
                    v102 = v13;
                    if (a2->mode != 6)
                        v7 = v13;
                    v116 = v69;
                    while (2)
                    {
                        v14 = &a2->pagElems[a2->triangleNum2++];
                        v14->ff2 = 0.0;
                        v14->fff = 0.0;
                        if (a2->fd21 != 0.0)
                        {
                            v130 = a2->fd21;
                            v118 = a2->ffb17;
                            v70 = rand();
                            v14->particleState = 1;
                            v14->ff2 = (float)((double)v70 * 0.000030518509 * (v130 - v118) + v118);
                        }
                        if (v14->ff2 == 0.0)
                        {
                            v140 = a2->ffb14;
                            v121 = a2->ffb13;
                            v71 = rand();
                            v14->particleState = 0;
                            v15 = (double)v71 * 0.000030518509 * (v140 - v121) + v121;
                            v14->ff2 = (float)v15;
                            if (v15 == 0.0 && a2->ffb16 != 0.0)
                            {
                                v126 = a2->ffb16;
                                v123 = a2->ffb15;
                                v72 = rand();
                                v14->particleState = 2;
                                v14->ff2 = (float)((double)v72 * 0.000030518509 * (v126 - v123) + v123);
                            }
                        }
                        v14->ff = (float)(1.0 / v14->ff2);
                        v141 = a2->ffb10;
                        v109 = a2->ffb9;
                        v16 = (double)rand() * 0.000030518509 * (v141 - v109) + v109;
                        v14->pagPosAndVertexIndexFloatBase = (float)v16;
                        if ((a2->flags & 4) == 0)
                        {
                            v127 = a2->ffb12;
                            v107 = a2->ffb11;
                            v16 = (double)rand() * 0.000030518509 * (v127 - v107) + v107;
                        }
                        v14->ffff2 = (float)v16;
                        switch (a2->mode)                 // csomó addScaleVector inline
                        {
                        case 0u:
                            v14->particlePos.x = v7->Pos.x;
                            v14->particlePos.y = v7->Pos.y;
                            v14->particlePos.z = v7->Pos.z;
                            goto LABEL_39;
                        case 1u:
                            v17 = rand();
                            v18 = ((double)v17 * 0.000030518509 + (double)v17 * 0.000030518509 - 1.0) * a2->ffb2;
                            v144 = (float)(v7->v1.y * v18);
                            v145 = (float)(v7->v1.z * v18);
                            v14->particlePos.x = (float)(v18 * v7->v1.x + v7->Pos.x);
                            v14->particlePos.y = v7->Pos.y + v144;
                            v14->particlePos.z = v7->Pos.z + v145;
                            v73 = rand();
                            v19 = ((double)v73 * 0.000030518509 + (double)v73 * 0.000030518509 - 1.0) * a2->ffb3;
                            v162 = (float)(v7->v3.y * v19);
                            v163 = (float)(v7->v3.z * v19);
                            v14->particlePos.x = (float)(v19 * v7->v3.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v162;
                            v20 = v14->particlePos.z + v163;
                            goto LABEL_38;
                        case 2u:
                            v22 = rand();
                            v143 = a2->ffb2;
                            v85 = (float)((double)v22 * 0.000030518509 * 12.566371);
                            v79 = (float)((double)rand() * 0.000030518509 * v143);
                            v23 = MATH_GetCos(v85) * v79;
                            v150 = (float)(v7->v1.y * v23);
                            v151 = (float)(v7->v1.z * v23);
                            v14->particlePos.x = (float)(v23 * v7->v1.x + v7->Pos.x);
                            v14->particlePos.y = v7->Pos.y + v150;
                            v14->particlePos.z = v7->Pos.z + v151;
                            v25 = MATH_GetSin(v85) * v79;
                            v170 = (float)(v7->v3.y * v25);
                            v171 = (float)(v7->v3.z * v25);
                            v14->particlePos.x = (float)(v25 * v7->v3.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v170;
                            v20 = v14->particlePos.z + v171;
                            goto LABEL_38;
                        case 3u:
                            v26 = rand();
                            v136 = a2->ffb2;
                            v86 = (float)((double)v26 * 0.000030518509 * 12.566371);
                            v80 = (float)((double)rand() * 0.000030518509 * v136);
                            v27 = MATH_GetCos(v86) * v80;
                            v154 = (float)(v7->v1.y * v27);
                            v155 = (float)(v7->v1.z * v27);
                            v14->particlePos.x = (float)(v27 * v7->v1.x + v7->Pos.x);
                            v14->particlePos.y = v7->Pos.y + v154;
                            v14->particlePos.z = v7->Pos.z + v155;                            
                            v29 = MATH_GetSin(v86) * v80;
                            v166 = (float)(v7->v3.y * v29);
                            v167 = (float)(v7->v3.z * v29);
                            v14->particlePos.x = (float)(v29 * v7->v3.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v166;
                            v14->particlePos.z = v14->particlePos.z + v167;
                            v30 = -((double)rand() * 0.000030518509 * a2->ffb3);
                            v158 = (float)(v7->v2.y * v30);
                            v159 = (float)(v7->v2.z * v30);
                            v14->particlePos.x = (float)(v30 * v7->v2.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v158;
                            v20 = v14->particlePos.z + v159;
                            goto LABEL_38;
                        case 4u:
                            v87 = (float)((double)rand() * 0.000030518509 * 6.2831855);
                            v31 = rand();
                            v124 = a2->ffb2;
                            v74 = (float)((double)v31 * 0.000030518509 * 3.1415927 - 1.5707964);
                            v81 = (float)((double)rand() * 0.000030518509 * v124);
                            v94 = MATH_GetCos(v74);
                            v32 = MATH_GetCos(v87) * v94 * v81;
                            v174 = (float)(v7->v1.y * v32);
                            v175 = (float)(v7->v1.z * v32);
                            v14->particlePos.x = (float)(v32 * v7->v1.x + v7->Pos.x);
                            v14->particlePos.y = v7->Pos.y + v174;
                            v14->particlePos.z = v7->Pos.z + v175;
                            v34 = MATH_GetCos(v74) * MATH_GetSin(v87) * v81;
                            v146 = (float)(v7->v2.y * v34);
                            v147 = (float)(v7->v2.z * v34);
                            v14->particlePos.x = (float)(v34 * v7->v2.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v146;
                            v14->particlePos.z = v14->particlePos.z + v147;
                            v36 = MATH_GetCos(v74) * v81;
                            v148 = (float)(v7->v3.y * v36);
                            v149 = (float)(v7->v3.z * v36);
                            v14->particlePos.x = (float)(v36 * v7->v3.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v148;
                            v20 = v14->particlePos.z + v149;
                        LABEL_38:
                            v14->particlePos.z = (float)v20;
                            goto LABEL_39;
                        case 5u:
                            v37 = rand();
                            v38 = ((double)v37 * 0.000030518509 + (double)v37 * 0.000030518509 - 1.0) * a2->ffb2;
                            v152 = (float)(v7->v1.y * v38);
                            v153 = (float)(v7->v1.z * v38);
                            v14->particlePos.x = (float)(v38 * v7->v1.x + v7->Pos.x);
                            v14->particlePos.y = v7->Pos.y + v152;
                            v14->particlePos.z = v7->Pos.z + v153;
                            v39 = rand();
                            v40 = ((double)v39 * 0.000030518509 + (double)v39 * 0.000030518509 - 1.0) * a2->ffb3;
                            v156 = (float)(v7->v3.y * v40);
                            v157 = (float)(v7->v3.z * v40);
                            v14->particlePos.x = (float)(v40 * v7->v3.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v156;
                            v14->particlePos.z = v14->particlePos.z + v157;
                            v132 = a2->ffb4;
                            v41 = -((double)rand() * 0.000030518509 * v132);
                            v160 = (float)(v7->v2.y * v41);
                            v161 = (float)(v7->v2.z * v41);
                            v14->particlePos.x = (float)(v41 * v7->v2.x + v14->particlePos.x);
                            v14->particlePos.y = v14->particlePos.y + v160;
                            v20 = v14->particlePos.z + v161;
                            goto LABEL_38;
                        case 6u:
                            v137 = v105->vertexNum * RND_JADErand() / 0x7FFF;
                            v42 = &v111[v137];
                            if (v42->z < (double)a2->i3 || v42->z >(double)a2->ffa1)
                            {
                                --a2->triangleNum2;
                            }
                            else
                            {
                                MATH_TransformVertex(&v14->particlePos, v7, v42);
                            LABEL_39:
                                v21 = -a2->ffb1;
                                v164 = (float)(v7->v2.y * v21);
                                v165 = (float)(v7->v2.z * v21);
                                v14->particlePos.x = (float)(v21 * v7->v2.x + v14->particlePos.x);
                                v14->particlePos.y = v14->particlePos.y + v164;
                                v14->particlePos.z = v14->particlePos.z + v165;
                                switch (a2->subMode)
                                {
                                case 0u:
                                    v138 = a2->ffb6;
                                    v117 = a2->ffb5;
                                    v43 = -((double)rand() * 0.000030518509 * (v138 - v117) + v117);
                                    v14->v1.x = (float)(v43 * v102->v2.x);
                                    v14->v1.y = (float)(v102->v2.y * v43);
                                    v14->v1.z = (float)(v102->v2.z * v43);
                                    goto LABEL_89;
                                case 1u:
                                    v134 = a2->ffb7;
                                    v75 = -v134;
                                    v44 = rand();
                                    v142 = a2->ffb6;
                                    v113 = a2->ffb5;
                                    v89 = (float)((double)v44 * 0.000030518509 * (v134 - v75) + v75);
                                    v82 = (float)((double)rand() * 0.000030518509 * (v142 - v113) + v113);
                                    v45 = -(MATH_GetCos(v89) * v82);
                                    v14->v1.x = (float)(v45 * v102->v2.x);
                                    v14->v1.y = (float)(v102->v2.y * v45);
                                    v14->v1.z = (float)(v102->v2.z * v45);
                                    v47 = MATH_GetSin(v89) * v82;
                                    v168 = (float)(v102->v1.y * v47);
                                    v169 = (float)(v102->v1.z * v47);
                                    v14->v1.x = (float)(v47 * v102->v1.x + v14->v1.x);
                                    v14->v1.y = v14->v1.y + v168;
                                    v48 = v14->v1.z + v169;
                                    goto LABEL_88;
                                case 2u:
                                    v128 = a2->ffb7;
                                    v76 = -v128;
                                    v49 = rand();
                                    v129 = a2->ffb8;
                                    v90 = (float)((double)v49 * 0.000030518509 * (v128 - v76) + v76);
                                    v77 = -a2->ffb7;
                                    v50 = rand();
                                    v131 = a2->ffb6;
                                    v122 = a2->ffb5;
                                    v78 = (float)((double)v50 * 0.000030518509 * (v129 - v77) + v77);
                                    v83 = (float)((double)rand() * 0.000030518509 * (v131 - v122) + v122);
                                    v98 = MATH_GetCos(v78);
                                    v51 = -(MATH_GetCos(v90) * v98 * v83);
                                    v14->v1.x = (float)(v51 * v102->v2.x);
                                    v14->v1.y = (float)(v102->v2.y * v51);
                                    v14->v1.z = (float)(v102->v2.z * v51);
                                    v53 = MATH_GetCos(v78) * MATH_GetSin(v90) * v83;
                                    v172 = (float)(v102->v1.y * v53);
                                    v173 = (float)(v102->v1.z * v53);
                                    v14->v1.x = (float)(v53 * v102->v1.x + v14->v1.x);
                                    v14->v1.y = v14->v1.y + v172;
                                    v14->v1.z = v14->v1.z + v173;
                                    v55 = MATH_GetSin(v78) * v83;
                                    v176 = (float)(v102->v3.y * v55);
                                    v177 = (float)(v102->v3.z * v55);
                                    v14->v1.x = (float)(v55 * v102->v3.x + v14->v1.x);
                                    v14->v1.y = v14->v1.y + v176;
                                    v48 = v14->v1.z + v177;
                                LABEL_88:
                                    v14->v1.z = (float)v48;
                                LABEL_89:
                                    if ((a2->flags & 0x1000) != 0)
                                    {
                                        v125 = a2->ffa4;
                                        v112 = a2->ffa3;
                                        v14->f5 = (unsigned int)(unsigned int)(((double)rand() * 0.000030518509 * (v125 - v112)
                                            + v112)
                                            * 162.97466) << 22;
                                        v120 = a2->ffa6;
                                        v114 = a2->ffa5;
                                        v67 = ((double)rand() * 0.000030518509 * (v120 - v114) + v114) * 10430.378;
                                        if (v67 < 0.0)
                                        {
                                            v67 = -v67;
                                            v14->f5 |= 0x8000u;
                                        }
                                        v14->f5 |= (unsigned int)v67 & 0x7FFF;
                                    }
                                    else
                                    {
                                        v14->f5 = 0;
                                    }
                                    break;
                                case 3u:
                                    v14->v1.x = v14->particlePos.x - v7->Pos.x;
                                    v14->v1.y = v14->particlePos.y - v7->Pos.y;
                                    v14->v1.z = v14->particlePos.z - v7->Pos.z;
                                    v56 = sqrt(v14->v1.x * v14->v1.x + v14->v1.y * v14->v1.y + v14->v1.z * v14->v1.z);
                                    v84 = (float)v56;
                                    if (v56 == 0.0)
                                        goto LABEL_87;
                                    v133 = a2->ffb6;
                                    v115 = a2->ffb5;
                                    v60 = (double)rand() * 0.000030518509 * (v133 - v115) + v115;
                                    goto LABEL_86;
                                case 4u:
                                    GEO_UseNormals(v105);
                                    v135 = a2->ffb6;
                                    v108 = a2->ffb5;
                                    v61 = (double)rand() * 0.000030518509;
                                    v62 = &v105->normals[v137];
                                    v63 = -(v61 * (v135 - v108) + v108);
                                    v14->v1.x = (float)(v63 * v62->x);
                                    v14->v1.y = (float)(v62->y * v63);
                                    v14->v1.z = (float)(v62->z * v63);
                                    goto LABEL_89;
                                case 5u:
                                    v14->v1.x = a2->ffa3 - v14->particlePos.x;
                                    v14->v1.y = a2->ffa4 - v14->particlePos.y;
                                    v14->v1.z = a2->ffa5 - v14->particlePos.z;
                                    v56 = sqrt(v14->v1.x * v14->v1.x + v14->v1.y * v14->v1.y + v14->v1.z * v14->v1.z);
                                    v84 = (float)v56;
                                    if (v56 == 0.0)
                                        goto LABEL_87;
                                    v139 = a2->ffb6;
                                    v110 = a2->ffb5;
                                    v60 = (double)rand() * 0.000030518509 * (v139 - v110) + v110;
                                LABEL_86:
                                    v56 = v60 / v84;
                                LABEL_87:
                                    v14->v1.x = (float)(v56 * v14->v1.x);
                                    v14->v1.y = (float)(v56 * v14->v1.y);
                                    v48 = v56 * v14->v1.z;
                                    goto LABEL_88;
                                default:
                                    goto LABEL_89;
                                }
                            }
                            if (--v116)
                                continue;
                            v8 = v104 + 1;
                            ++v104;
                            break;
                        default:
                            goto LABEL_39;
                        }
                        break;
                    }
                }
                else
                {
                    ++v8;
                    v104 = v8;
                }
                v9 = v101 + 1;
                ++v103;
            }
            v68 = nnum == 1;
            v101 = v9;
            --nnum;
        } while (!v68);
    }
}

void PAG_RenderCommon(struct sGameObjectMain* a1, struct sPAGStruct* a2)
{
    int v2;
    BOOL v3;
    tGdi_DisplayData* v4;
    unsigned __int16 v5;
    Mtx4x4* v6;
    int v7;
    Vec3* v8;
    tGameObjectBasic* v9;
    tGameObjectVisualObject* v10;
    tGeoTriangleChunkElem* v11;
    int* v12;
    Vec3* v13;
    tPAGElemStruct* v14;
    int* v15;
    tPAGElemStruct* v16;
    tPAGStruct* v17;
    int v18;
    int v19;
    int v20;
    int v21;
    int v22;
    double v23;
    unsigned int v24;
    float v25;
    float v26;
    unsigned int v27;
    uint32_t* v28;
    int v29;
    tGeoTriangleChunkElem* v30;
    uint16_t v31;
    Vec3* v32; 
    double v33;
    Vec3* v34; 
    int* v35; 
    tPAGElemStruct* v36;
    tPAGElemStruct* v37;
    double v38;
    int v39; 
    int v40; 
    int v41; 
    float scale;
    float v43;
    float v44;
    float v45;
    float v46;
    float a3; 
    float a3a;
    float v49;
    int v50; 
    int v51; 
    Vec3* v52;
    Vec3* v53;
    tPAGElemStruct* v54;
    tGeoSpriteElem* v55;
    tPAGElemStruct* v56;
    tPAGElemStruct* v57;
    float a4a;
    int* a4; 
    int v60; 
    int v61; 
    uint16_t v62;
    Vec3 v63;
    Vec3 src2;
    Vec3 dest;
    Vec3 v66; 
    Vec3 a1a; 

    v2 = a2->flags;
    v3 = (v2 & 0x10000) == 0;
    if ((v2 & 0x40) != 0 && a1->distance)
    {
        v4 = gpst_DX_stDD;
        if ((a2->flags & 0x10000) != 0 && (v2 & 1) != 0 && (gpst_DX_stDD->flags & 4) == 0)
        {
            SOFT_AddCurrentObjectInZList(gpst_DX_stDD, a1);
            return;
        }
        if (a2->renderCnt != gpst_DX_stDD->rasters[4].unk1)
        {
            if (ENG_gb_EngineRunning && (v2 & 0x80u) == 0)
            {
                PAG_Update(a1, a2, TIM_gf_dt);
                PAG_Generate(a1, a2, TIM_gf_dt);
                v4 = gpst_DX_stDD;
            }
            a2->renderCnt = v4->rasters[4].unk1;
            v4 = gpst_DX_stDD;
        }
        if (a2->triangleNum2)
        {
            v60 = v4->drawMaskAllowedStuffCurrent;
            v61 = v4->flags;
            if (v3)
            {
                v4->flags |= 8u;
                v4 = gpst_DX_stDD;
                if ((a2->flags & 1) != 0)
                {
                    gpst_DX_stDD->flags |= 0x200u;
                    v4 = gpst_DX_stDD;
                }
            }
            v5 = v4->matrixStack.currElemNum;
            v4->matrixStack.actMatrixElem = &v4->ct.cam.cameraMtx;
            v62 = v5;
            gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
            gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
            v6 = &gpst_DX_stDD->ct.cam.objectMtx;
            if (a2->ffb10 == a2->ffb9 && a2->ffb12 == a2->ffb11 && (v7 = a2->flags, (v7 & 0x30) == 0))
            {
                MATH_VecMultScalar(&a1a, &gpst_DX_stDD->ct.cam.objectMtx.v1, a2->ffb9);
                v8 = &v6->v2;
                if ((v7 & 4) != 0)
                    MATH_VecMultScalar(&v66, v8, a2->ffb9);
                else
                    MATH_VecMultScalar(&v66, v8, a2->ffb11);
                v63 = a1a;
                src2 = v66;
                v52 = &a1a;
                v53 = &v66;
                v51 = 1;
            }
            else
            {
                v52 = &gpst_DX_stDD->ct.cam.objectMtx.v1;
                v53 = &gpst_DX_stDD->ct.cam.objectMtx.v2;
                v51 = 0;
            }
            v9 = a1->basic;
            if (v9)
            {
                v10 = v9->actorObjectOrVisualObject;
                if (v10)
                {
                    ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROMAT = v10->GROMAT;
                    if (PAGRenderingEnabled)
                    {
                        GFX_NeedGeom(4 * a2->triangleNum2, 4u, 2 * a2->triangleNum2, 1);
                        GFX_gpst_Geo->triangleChunks->materialNum = 0;
                        v11 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
                        v12 = GFX_gpst_Geo->vertexColors;
                        v13 = GFX_gpst_Geo->vertices;
                        GFX_gpst_Geo->UVs->u = 0.0;
                        GFX_gpst_Geo->UVs->v = 0.0;
                        GFX_gpst_Geo->UVs[1].u = 1.0;
                        GFX_gpst_Geo->UVs[1].v = 0.0;
                        GFX_gpst_Geo->UVs[2].u = 1.0;
                        GFX_gpst_Geo->UVs[2].v = 1.0;
                        GFX_gpst_Geo->UVs[3].u = 0.0;
                        GFX_gpst_Geo->UVs[3].v = 1.0;
                        v14 = a2->pagElems;
                        v15 = v12 + 1;
                        v54 = v14;
                        v56 = &v14[a2->triangleNum2];
                        if (v14 < v56)
                        {
                            v16 = v14;
                            while (1)
                            {
                                v17 = a2;
                                v18 = v14->particleState;
                                v50 = a2->color;
                                a3 = v16->pagPosAndVertexIndexFloatBase;
                                v49 = v16->ffff2;
                                if ((v18 & 1) == 0)
                                    break;
                                v19 = a2->flags;
                                v43 = (float)(1.0 - v16->ff2 * v16->ff);
                                if ((v19 & 0x100) != 0)
                                {
                                    v20 = COLOR_ul_Mul(v50, v43);
                                    v19 = a2->flags;
                                    v50 = v20;
                                    if ((v19 & 2) == 0)
                                        v50 = (v20 ^ a2->color) & 0xFFFFFF ^ v20;
                                }
                                if ((v19 & 0x200) != 0)
                                    goto LABEL_42;
                            LABEL_46:
                                if (!v51)
                                {
                                    MATH_VecMultScalar(&v63, v52, a3);
                                    MATH_VecMultScalar(&src2, v53, v49);
                                }
                                v24 = v16->f5;
                                if (v24)
                                {
                                    if (v51)
                                    {
                                        v63.x = v52->x;
                                        v25 = v52->y;
                                        v63.z = v52->z;
                                        v63.y = v25;
                                        src2.x = v53->x;
                                        v26 = v53->z;
                                        src2.y = v53->y;
                                        src2.z = v26;
                                    }
                                    v27 = v24 >> 22;
                                    a4a = MATH_gf_TableSin[v27 + 1];
                                    v44 = MATH_gf_TableSin[(v27 + 256) % 0x3FF + 1];
                                    dest.x = v63.x * v44;
                                    dest.y = v63.y * v44;
                                    dest.z = v63.z * v44;
                                    MATH_AddScaleVector(&dest, &dest, &src2, a4a);
                                    MATH_VecMultScalarWithoutBuffer(&src2, v44);
                                    scale = -a4a;
                                    MATH_AddScaleVector(&src2, &src2, &v63, scale);
                                    v63 = dest;
                                }
                                *v15 = v50;
                                v15[1] = v50;
                                v28 = v15 + 1;
                                v28[1] = v50;
                                ++v28;
                                v28[1] = v50;
                                v29 = (char*)v13 - (char*)GFX_gpst_Geo->vertices;
                                v11->uv1 = 0;
                                v11->uv2 = 1;
                                v11->v1 = v29 / 12;
                                v11->v2 = v29 / 12 + 1;
                                v30 = v11 + 1;
                                v31 = v29 / 12 + 2;
                                v30[-1].v3 = v31;
                                v30[-1].uv3 = 2;
                                v30->v1 = v31;
                                v30->v3 = v29 / 12;
                                v30->uv1 = 2;
                                v30->uv2 = 3;
                                v30->uv3 = 0;
                                v30->v2 = v29 / 12 + 3;
                                v15 = v28 + 2;
                                v11 = v30 + 1;
                                v32 = v13 + 2;
                                v32[-2].x = v16->particlePos.x - v63.x;
                                v32[-2].y = v16->particlePos.y - v63.y;
                                v32[-2].z = v16->particlePos.z - v63.z;
                                v32[-2].x = v32[-2].x - src2.x;
                                v32[-2].y = v32[-2].y - src2.y;
                                v32[-2].z = v32[-2].z - src2.z;
                                v32[-1].x = v16->particlePos.x - v63.x;
                                v32[-1].y = v16->particlePos.y - v63.y;
                                v32[-1].z = v16->particlePos.z - v63.z;
                                v32[-1].x = src2.x + v32[-1].x;
                                v32[-1].y = src2.y + v32[-1].y;
                                v32[-1].z = src2.z + v32[-1].z;
                                ++v32;
                                v32[-1].x = v63.x + v16->particlePos.x;
                                v13 = v32 + 1;
                                v33 = v63.y + v16->particlePos.y;
                                ++v16;
                                ++v54;
                                v13[-2].y = (float)v33;
                                v13[-2].z = v63.z + v16[-1].particlePos.z;
                                v13[-2].x = src2.x + v13[-2].x;
                                v13[-2].y = src2.y + v13[-2].y;
                                v13[-2].z = src2.z + v13[-2].z;
                                v13[-1].x = v63.x + v16[-1].particlePos.x;
                                v13[-1].y = v63.y + v16[-1].particlePos.y;
                                v13[-1].z = v63.z + v16[-1].particlePos.z;
                                v13[-1].x = v13[-1].x - src2.x;
                                v13[-1].y = v13[-1].y - src2.y;
                                v13[-1].z = v13[-1].z - src2.z;
                                if (v54 >= v56)
                                    goto LABEL_71;
                                v14 = v54;
                            }
                            if ((v18 & 2) == 0)
                                goto LABEL_46;
                            v21 = a2->flags;
                            v43 = v16->ff2 * v16->ff;
                            if ((v21 & 8) != 0)
                            {
                                v22 = COLOR_ul_Mul(v50, v43);
                                v21 = a2->flags;
                                v50 = v22;
                                if ((v21 & 2) == 0)
                                    v50 = (v22 ^ a2->color) & 0xFFFFFF ^ v22;
                                v17 = a2;
                            }
                            if ((v21 & 0x10) != 0)
                            {
                            LABEL_42:
                                a3 = v43 * a3;
                                v23 = v43;
                            }
                            else
                            {
                                if ((v21 & 0x20) == 0)
                                    goto LABEL_46;
                                v23 = (1.0 - v43) * v17->fd22 + 1.0;
                                a3 = (float)(a3 * v23);
                            }
                            v49 = (float)(v23 * v49);
                            goto LABEL_46;
                        }
                    }
                    else
                    {
                        GFX_NeedGeomEx(a2->triangleNum2, 4u, 0, 0, 1);
                        GFX_NeedGeomNbElemSprites(1u);
                        GFX_NeedGeomElemNbSprites(0, a2->triangleNum2);
                        GFX_gpst_Geo->spritesPtr->matIdx = 0;
                        GFX_gpst_Geo->spritesPtr->globalSize = 1.0;
                        GFX_gpst_Geo->spritesPtr->globalRatio = 1.0;
                        v34 = GFX_gpst_Geo->vertices;
                        v55 = GFX_gpst_Geo->spritesPtr->spriteElems;
                        v35 = GFX_gpst_Geo->vertexColors;
                        GFX_gpst_Geo->UVs->u = 0.0;
                        a4 = v35 + 1;
                        GFX_gpst_Geo->UVs->v = 0.0;
                        GFX_gpst_Geo->UVs[1].u = 1.0;
                        GFX_gpst_Geo->UVs[1].v = 0.0;
                        GFX_gpst_Geo->UVs[2].u = 1.0;
                        GFX_gpst_Geo->UVs[2].v = 1.0;
                        GFX_gpst_Geo->UVs[3].u = 0.0;
                        GFX_gpst_Geo->UVs[3].v = 1.0;
                        v36 = a2->pagElems;
                        v57 = &v36[a2->triangleNum2];
                        if (v36 < v57)
                        {
                            v37 = v36;
                            do
                            {
                                v38 = v37->pagPosAndVertexIndexFloatBase;
                                a3a = v37->pagPosAndVertexIndexFloatBase;
                                v39 = a2->color;
                                if ((v36->particleState & 1) != 0)
                                {
                                    v40 = a2->flags;
                                    v45 = (float)(1.0 - v37->ff2 * v37->ff);
                                    if ((v40 & 0x100) != 0)
                                    {
                                        v39 = COLOR_ul_Mul(v39, v45);
                                        v38 = a3a;
                                        v40 = a2->flags;
                                        if ((v40 & 2) == 0)
                                            v39 ^= (v39 ^ a2->color) & 0xFFFFFF;
                                    }
                                    if ((v40 & 0x200) != 0)
                                        v38 = v38 * v45;
                                }
                                else if ((v36->particleState & 2) != 0)
                                {
                                    v41 = a2->flags;
                                    v46 = v37->ff2 * v37->ff;
                                    if ((v41 & 8) != 0)
                                    {
                                        v39 = COLOR_ul_Mul(v39, v46);
                                        v38 = a3a;
                                        v41 = a2->flags;
                                        if ((v41 & 2) == 0)
                                            v39 ^= (v39 ^ a2->color) & 0xFFFFFF;
                                    }
                                    if ((v41 & 0x10) != 0)
                                    {
                                        v38 = v38 * v46;
                                    }
                                    else if ((v41 & 0x20) != 0)
                                    {
                                        v38 = v38 * ((1.0 - v46) * a2->fd22 + 1.0);
                                    }
                                }
                                *a4 = v39;
                                v55->size = (float)v38;
                                ++a4;
                                v55->idx = (uint16_t)((uint16_t)v34 - (unsigned int)GFX_gpst_Geo->vertices) / sizeof(Vec3);
                                v34->x = v37->particlePos.x;
                                v34->y = v37->particlePos.y;
                                v34->z = v37->particlePos.z;
                                ++v55;
                                ++v36;
                                ++v34;
                                ++v37;
                            } while (v36 < v57);
                        }
                    }
                LABEL_71:
                    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x8000900u;
                    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
                    gpst_DX_stDD->drawMaskAllowedStuffCurrent = v60;
                    gpst_DX_stDD->flags = v61;
                    gpst_DX_stDD->matrixStack.currElemNum = v62;
                    gpst_DX_stDD->matrixStack.actMatrixElem = &gpst_DX_stDD->matrixStack.elems[v62];
                    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
                    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
                }
            }
        }
    }
}
