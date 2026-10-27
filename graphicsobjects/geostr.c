#include "../memory.h"
#include "../win/wingdi.h"
#include "../loading.h"
#include "../strings.h"
#include "../time.h"
#include "geostr.h"

unsigned int ul_SubMatOrder[] = { 8, 9, 0x0D, 0x10, 0x0A, 0x0B, 0x0C, 0x0E, 0x0F, 0, 1, 2, 3, 4, 5, 6, 7, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };

tGEOSTRStruct* STR_pst_Create()
{
    tGEOSTRStruct* v0;
    tGEOSTRStringData* v1;
    unsigned int v2;

    v0 = (tGEOSTRStruct*)MEM_p_Alloc(sizeof(tGEOSTRStruct));
    memset(v0, 0, sizeof(tGEOSTRStruct));
    GRO_Struct_Init((tGroGeneric*)v0, 10);
    v0->stringNum = 32;
    v1 = (tGEOSTRStringData*)MEM_p_Alloc(32 * sizeof(tGEOSTRStringData));
    v0->stringData = v1;
    memset(v1, 0, 32 * sizeof(tGEOSTRStringData));
    v0->effectsNum = 0;
    v2 = 40 * v0->stringNum;
    v0->maxEffectsNum = 32;
    v0->width = 640;
    v0->height = 480;
    memset(v0->effect, 0, v2);
    v0->fontMaterialID = -1;
    return v0;
}

struct sGroGeneric* STR_p_CreateFromBuffer(struct sGroGeneric* _C, char** a1, struct sWorld* world)
{
    tGEOSTRStruct* v2;
    int v3;
    tGEOSTRLetter* v4;
    tGEOSTRStruct* result;

    v2 = STR_pst_Create();
    v3 = LOA_ReadLong(a1, 0, 1);
    v2->unkNum = v3;
    if (v3)
        v2->letters = (tGEOSTRLetter*)MEM_p_Alloc(sizeof(tGEOSTRLetter) * v3);
    else
        v2->letters = 0;
    v4 = v2->letters;
    if (v4)
    {
        if (v4 < &v4[v2->unkNum])
            memset(v4, 0, ((sizeof(tGEOSTRLetter) * v2->unkNum - 1) & 0xFFFFFFF0) + sizeof(tGEOSTRLetter));
        result = v2;
    }
    else
    {
        v2->unkNum = 0;
        result = v2;
    }
    return (tGroGeneric*)result;
}

void STR_Free(struct sGroGeneric* a1)
{
    tGEOSTRStruct* a1a;
    if (a1 && a1->intf->id == 10 && a1->usageCount <= 0)
    {
        a1a = (tGEOSTRStruct*)a1;
        //nullsub_32();
        if (a1a->unkNum)
            MEM_Free(a1a->letters);
        if (a1a->stringNum)
            MEM_Free(a1a->stringData);
        LOA_DeleteAddress((int)a1a);
        MEM_Free(a1a);
    }
}

int STR_l_HasSomethingToRender(struct sGameObjectVisualObject* a1, struct sGroGeneric** a2)
{
    *a2 = NULL;
    return 1;
}

void STR_Reinit(struct sGroGeneric* a1)
{
    tGEOSTRStruct* a1a = (tGEOSTRStruct*)a1;

    memset(a1a->stringData, 0, sizeof(tGEOSTRStringData) * a1a->stringNum);
    a1a->effectsNum = 0;
}

int STR_l_Effect_Treat(tGEOSTRStruct* a1, tGEOSTREffectStruct* inner1)
{
    tGEOSTRStringData* v3;
    int result;
    int v5;
    tGEOSTRLetter* v6;
    tGEOSTRLetter* v7;
    int v8;
    int v9;
    unsigned int* v10;
    unsigned int v11; 
    unsigned int v12; 
    unsigned int* v13;
    unsigned int v14; 
    int* i;
    double v16;
    BOOL v17; 
    int v18; 
    unsigned int v19;
    int* v20;
    unsigned int v21;
    unsigned int* v22;
    double v23;
    int v24; 
    int v25;
    Vec3* v26;
    Vec3* v27;
    Vec3* v28;
    int* v29; 
    int v30; 
    int v31; 
    int* v32;
    unsigned int v33;
    int* v34;
    double v35;
    Vec3* v36; 
    double v37;
    Vec3* v38; 
    Vec3* v39; 
    long double v40;
    double v41;
    double v42;
    double v43;
    Vec3* v44; 
    Vec3* v45; 
    Vec3* v46; 
    double v47;
    double v48;
    double v49;
    unsigned int v50;
    unsigned int* v51;
    unsigned int v52;
    unsigned int* v53;
    double v54;
    Vec3* v55; 
    double v56;
    double v57;
    double v58;
    double v59;
    double v60;
    double v61;
    double v62;
    Vec3* v63; 
    unsigned int v64;
    double v65;
    double v66;
    double v67;
    double v68;
    double v69;
    Vec3* v70; 
    Vec3* v71; 
    Vec3* v72; 
    Vec3* v73; 
    double v74;
    double v75;
    Vec3* v76; 
    double v77;
    double v78;
    tGEOGRO* v79;
    Vec3* v80;
    Vec3* v81;
    Vec3* v82;
    Vec3* v83;
    double v84;
    Vec3* v85; 
    Vec3* v86; 
    Vec3* v87; 
    Vec3* m;
    double v89;
    unsigned int v90; 
    tGEOGRO* v91;
    int* v92;
    double v93;
    Vec3* v94; 
    Vec3* v95; 
    Vec3* v96; 
    unsigned int v97;
    int* n; 
    double v99;
    double v100;
    Vec3* v101; 
    double v102;
    Vec3* v103; 
    Vec3* v104; 
    double v105;
    double v106;
    double v107;
    Vec3* v108; 
    unsigned int v109;
    Vec3* v110; 
    Vec3* v111; 
    double v112; 
    Vec3* jj; 
    double v114;
    double v115;
    double v116;
    Vec3* v117;
    Vec3* v118;
    Vec3* v119;
    double v120;
    double v121;
    double v122;
    Vec3* v123; 
    int v124; 
    double v125;
    int* v126; 
    Vec3* v127;
    Vec3* v128;
    int v129; 
    double v130;
    Vec3* v131; 
    Vec3* v132; 
    int* v133; 
    int v134; 
    int v135; 
    double v136; 
    Vec3* v137;
    int v138; 
    double v139;
    int v140; 
    int v141; 
    Vec3* v142; 
    tGeoUVElem* v143;
    double v144;
    double v145;
    double v146;
    int v147; 
    Vec3* v148;
    tGeoUVElem* v149;
    double v150;
    double v151;
    double v152;
    int v153; 
    Vec3* v154;
    tGeoUVElem* v155;
    double v156;
    double v157;
    double v158;
    int v159; 
    Vec3* v160;
    tGeoUVElem* v161;
    double v162;
    double v163;
    double v164;
    int v165; 
    Vec3* v166;
    tGeoUVElem* v167;
    double v168;
    double v169;
    double v170;
    int v171;
    int v172;
    Vec3* v173;
    tGeoUVElem* v174;
    double v175;
    double v176;
    double v177;
    int v178; 
    Vec3* v179;
    tGeoUVElem* v180;
    double v181;
    double v182;
    double v183;
    int v184; 
    Vec3* v185;
    tGeoUVElem* v186;
    double v187;
    double v188;
    double v189;
    int v190; 
    Vec3* v191;
    tGeoUVElem* v192;
    double v193;
    double v194;
    double v195;
    int v196; 
    Vec3* v197;
    tGeoUVElem* v198;
    double v199;
    double v200;
    double v201;
    int v202; 
    double v203;
    int v204; 
    Vec3* v205;
    tGeoUVElem* v206;
    double v207;
    int v208; 
    double v209;
    double v210;
    int v211; 
    Vec3* v212;
    tGeoUVElem* v213;
    double v214;
    int v215;
    double v216;
    double v217;
    int v218; 
    Vec3* v219;
    tGeoUVElem* v220;
    double v221;
    int v222; 
    double v223;
    double v224;
    int v225; 
    Vec3* v226; 
    tGeoUVElem* v227;
    double v228;
    int v229; 
    double v230;
    double v231;
    int v232; 
    Vec3* v233;
    tGeoUVElem* v234;
    double v235;
    int v236; 
    double v237;
    double v238;
    double v239;
    tGEOGRO* v240;
    Vec3* v241;
    Vec3* v242;
    Vec3* v243;
    double v244;
    double v245;
    double v246;
    double v247;
    tGeoUVElem* v248;
    double v249;
    Vec3* v250; 
    Vec3* v251; 
    Vec3* v252; 
    double v253; 
    double v254; 
    char v255; 
    int v256; 
    int v257; 
    Vec3* v258;
    double v259;
    double v260;
    double v261;
    double v262;
    double v263;
    double v264;
    double v265;
    double v266;
    double v267;
    double v268;
    int* v269;
    unsigned int v270;
    unsigned int v271;
    unsigned int v272;
    tGeoUVElem* v273; 
    double v274;
    double v275;
    double v276;
    double v277;
    double v278;
    double v279;
    float a1a; 
    float a1b; 
    float v282;
    float v283;
    float v284;
    float v285;
    float l; 
    float v287; 
    float v288; 
    float v289; 
    float v290; 
    float v291; 
    float v292; 
    float v293; 
    float v294; 
    float v295; 
    float j;  
    float k; 
    float v298; 
    float v299; 
    float v300; 
    float v301; 
    float v302;
    float ii; 
    float v304;
    float v305;
    tGEOSTRStringData* v306;
    tGEOSTRLetter* v307;
    float v308;
    float v309;
    float v310;
    float v311;
    float v312;
    int v313;
    int v314;
    int v315;
    int v316;
    int v317;
    float v318;
    int flagsLOByte; 
    float v320;
    float v321;
    int v322;
    int v323;
    float v324;
    float v325;
    float v326;
    float v327;
    int v328;
    int v329;
    int v330;
    int* specularColorPtr;
    float v332;
    float v333;
    float v334;
    float v335;
    float v336;
    float v337;
    int v338; 
    int v339; 
    int v340; 
    int v341; 
    int v342; 
    float v343;
    int v344;
    int v345;
    int v346;
    float v347;
    float v348;
    float v349;
    unsigned int v350;
    Vec3* v351;
    int kk;
    int v353;
    float v354;
    float v355;
    int v356;
    int v357;
    float v358;
    float v359;
    float inner1a;
    float inner1b;
    float inner1c;
    float inner1d;
    float inner1o;
    float inner1e;
    float inner1f;
    float inner1g;
    float inner1h;
    float inner1p;
    float inner1i;
    float inner1j;
    float inner1k;
    float inner1l;
    float inner1m;
    float inner1n;
    float inner1q;
    float inner1r;
    unsigned int o;
    tGEOSTREffectStructType20 *inner1Type20;

    v345 = 0;
    v3 = STR_pst_GetString(a1, inner1->stringid);
    v306 = v3;
    if (!v3)
        return 1;
    if ((v3->flags & 0x400) != 0)
        return 0;
    v344 = STR_l_GetValidIndex(a1, v3, inner1->index);
    v5 = STR_l_GetValidNumber(a1, v306, v344, inner1->number);
    v356 = v306->index;
    v330 = v5;
    v6 = &a1->letters[v344 + v356];
    specularColorPtr = a1->specularColorsPtr;
    v7 = &v6[v5];
    v8 = 4 * specularColorPtr[((16 * (v344 + v356) + 16 * v5) >> 4) - 1] + 4; // talan align?
    v9 = 4 * specularColorPtr[(16 * (v344 + v356)) >> 4];
    v307 = v6;
    if (v8 == 4)
        return 1;
    if ((inner1->flags & 0x100) != 0 && (v306->flags & 0x100) != 0)
    {
        if ((inner1->flags & 0x200) != 0)
        {
            v7 = &a1->letters[v344 + v356];
            v8 = 4 * specularColorPtr[(16 * (v344 + v356)) >> 4];
        }
        v350 = 36;
        v345 = 4 * v306->i2;
        goto LABEL_12;
    }
    v350 = 0;
    if (!v330)
        return 1;
LABEL_12:
    flagsLOByte = inner1->flags & 0xff;
    switch (inner1->flags & 0xff)
    {
    case 1u:
        if (inner1->timeElapsed >= (double)inner1->f3)
            inner1a = (inner1->timeElapsed - inner1->f3) / inner1->f1;
        else
            inner1a = 0.0;
        if ((inner1->flags & 0x10000) != 0)
            inner1a = (float)(1.0 - inner1a);
        v10 = &GFX_gpst_Geo->vertexColors[4 * specularColorPtr[(16 * (v344 + v356)) >> 4] + 1];
        if (v6 < v7)
        {
            v11 = ((unsigned int)((char*)v7 - (char*)v6 - 1) >> 4) + 1;
            do
            {
                v12 = LIGHT_ul_Interpol2Colors(*v10, 0, inner1a);
                *v10 = v12;
                v13 = (v10 + 1);
                *v13++ = v12;
                *v13++ = v12;
                *v13 = v12;
                v10 = (v13 + 1);
                --v11;
            } while (v11);
        }
        v14 = v350;
        for (i = &GFX_gpst_Geo->vertexColors[v345 + 1]; v14; --v14)
        {
            *i = LIGHT_ul_Interpol2Colors(*i, 0, inner1a);
            ++i;
        }
        v16 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v16;
        if (v16 < inner1->f1 + inner1->f3)
            goto LABEL_43;
        v17 = (inner1->flags & 0x10000) == 0;
        goto LABEL_25;
    case 2u:
        v18 = inner1->flags;
        if ((inner1->flags & 0x10000) != 0)
        {
            inner1b = inner1->timeElapsed / inner1->f1;
            if ((v18 & 0x1000000) != 0)
                inner1b = (float)(1.0 - inner1b);
            v19 = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(inner1->color1), *(uint32_t*)&(inner1->color2), inner1b);
        }
        else if ((v18 & 0x1000000) != 0)
        {
            v19 = *(uint32_t *)&(inner1->color1);
        }
        else
        {
            v19 = *(uint32_t*)&(inner1->color2);
        }
        v20 = &GFX_gpst_Geo->vertexColors[v9 + 1];
        if (v6 < v7)
        {
            v21 = ((unsigned int)((char*)v7 - (char*)v6 - 1) >> 4) + 1;
            do
            {
                *v20 = v19;
                v22 = (v20 + 1);
                *v22++ = v19;
                *v22++ = v19;
                *v22 = v19;
                v20 = (v22 + 1);
                --v21;
            } while (v21);
        }
        v23 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v23;
        if (v23 < inner1->f1)
        {
            inner1->flags &= ~0x2000000u;
            return 0;
        }
        v24 = inner1->flags;
        v17 = (inner1->flags & 0x1000000) == 0;
        inner1->timeElapsed = 0.0;
        if (v17)
            v25 = v24 | 0x1000000;
        else
            v25 = v24 & ~0x1000000u;
        inner1->flags = v25;
        inner1->flags = v25 | 0x2000000;
        goto LABEL_43;
    case 3u:
        v26 = GFX_gpst_Geo->vertices;
        v27 = &v26[v8];
        v28 = &v26[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        inner1c = v28->x;
        v283 = v28->y;
        v29 = &GFX_gpst_Geo->vertexColors[4 * specularColorPtr[(16 * (v344 + v356)) >> 4] + 1];
        v332 = v27[-2].x - v28->x;
        v320 = v27[-2].y - v283;
        if (v332 == 0.0)
            goto LABEL_43;
        if (v320 == 0.0)
            goto LABEL_43;
        v333 = (float)(1.0 / v332);
        v321 = (float)(1.0 / v320);
        if (v28 >= v27)
            goto LABEL_43;
        do
        {
            v294 = (v28->x - inner1c) * v333;
            v357 = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(inner1->f1), *(uint32_t*)&(inner1->timeElapsed), v294);
            v30 = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(inner1->color2), *(uint32_t*)&(inner1->color1), v294);
            v295 = (v28->y - v283) * v321;
            *v29 = LIGHT_ul_Interpol2Colors(v357, v30, v295);
            ++v28;
            ++v29;
        } while (v28 < v27);
        return 0;
    case 4u:
        if (inner1->timeElapsed >= (double)inner1->f3)
            inner1d = (inner1->timeElapsed - inner1->f3) / inner1->f1;
        else
            inner1d = 0.0;
        v31 = LIGHT_ul_Interpol2Colors(*(uint32_t*)&(inner1->color1), *(uint32_t*)&(inner1->color2), inner1d);
        v32 = &GFX_gpst_Geo->vertexColors[v9 + 1];
        if (v6 < v7)
        {
            v33 = ((unsigned int)((char*)v7 - (char*)v6 - 1) >> 4) + 1;
            do
            {
                *v32 = v31;
                v34 = v32 + 1;
                *v34++ = v31;
                *v34++ = v31;
                *v34 = v31;
                v32 = v34 + 1;
                --v33;
            } while (v33);
        }
        if (v350) {
            for (o = 0; o < v350; o++) {
                GFX_gpst_Geo->vertexColors[v345 + 1 + o] = v31; // memset32(&GFX_gpst_Geo->vertexColors[v345 + 1], v31, v350);
            }
        }
        v35 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v35;
        if (v35 < inner1->f1 + inner1->f3)
            goto LABEL_43;
        v17 = (*(uint32_t*)&(inner1->color2) & 0xFFFFFF) == 0;
        goto LABEL_25;
    case 5u:
        v36 = GFX_gpst_Geo->vertices;
        v37 = inner1->color2 - inner1->f3;
        v38 = &v36[v8];
        v39 = &v36[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        v308 = (float)((double)(uint16_t)a1->height * inner1->f1 * v37 * v37);
        for (j = (float)(v37 * inner1->timeElapsed); v39 < v38; v39 += 4)
        {
            v282 = (float)((v39[2].x + v39->x) * 0.5 * inner1->color1 + j);
            v40 = fabs(MATH_GetSin(v282)) * v308;
            v41 = v39->y;

            v39[0].y = (float)(v41 - v40);
            v39[1].y = (float)(v39[1].y - v40);
            v39[2].y = (float)(v39[2].y - v40);
            v39[3].y = (float)(v39[3].y - v40);
        }
        v42 = TIM_gf_dt + inner1->f3;
        inner1->f3 = (float)v42;
        if (v42 < inner1->color2)
            goto LABEL_43;
        return 1;
    case 6u:
        v43 = inner1->color2 / inner1->color1;
        v44 = GFX_gpst_Geo->vertices;
        v45 = &v44[v8];
        v46 = &v44[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        v351 = v45;
        for (k = (float)((1.0 - v43) * (1.0 - v43)); v46 < v351; v46 += 4)
        {
            a1a = -inner1->f1;
            inner1o = (float)(RND_RandFloatRange(a1a, inner1->f1) * (double)(uint16_t)a1->width * k);
            a1b = -inner1->timeElapsed;
            v47 = RND_RandFloatRange(a1b, inner1->timeElapsed) * (double)(uint16_t)a1->height * k;
            v46[0].x = inner1o + v46[0].x;
            v46[0].y = (float)(v46[0].y - v47);
            v46[1].x = inner1o + v46[1].x;
            v46[1].y = (float)(v46[1].y - v47);
            v46[2].x = inner1o + v46[2].x;
            v46[2].y = (float)(v46[2].y - v47);
            v46[3].x = inner1o + v46[3].x;
            v46[3].y = (float)(v46[3].y - v47);
        }
        if ((inner1->flags & 0x30000) != 0)
        {
            v48 = inner1->color2 / inner1->color1;
            v49 = v48 + v48;
            v298 = (float)v49;
            if ((inner1->flags & 0x20000) != 0)
            {
                v49 = 2.0 - v49;
                v298 = (float)v49;
            }
            if (v49 > 1.0)
                v298 = 1.0;
            v50 = LIGHT_ul_Interpol2Colors(0, 0xFFFFFFFF, v298);
            v51 = &GFX_gpst_Geo->vertexColors[v9 + 1];
            if (v307 < v7)
            {
                v52 = ((unsigned int)((char*)v7 - (char*)v307 - 1) >> 4) + 1;
                do
                {
                    *v51 = v50;
                    v53 = (v51 + 1);
                    *v53++ = v50;
                    *v53++ = v50;
                    *v53 = v50;
                    v51 = (v53 + 1);
                    --v52;
                } while (v52);
            }
        }
        v54 = TIM_gf_dt + inner1->color2;
        inner1->color2 = (float)v54;
        if (v54 < inner1->color1)
            goto LABEL_43;
        return 1;
    case 7u:
        v322 = 0;
        if (inner1->color1 < (double)inner1->color2)
        {
            result = 0;
            inner1->color1 = TIM_gf_dt + inner1->color1;
            return result;
        }
        v299 = (float)((inner1->f1 - 0.050000001 - inner1->timeElapsed) / (double)v330);
        if (v6 < v7)
        {
            v55 = &GFX_gpst_Geo->vertices[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
            do
            {
                v56 = inner1->color1 - inner1->color2 - (double)v322 * v299;
                if (v56 <= 0.0)
                    break;
                v57 = v56 / inner1->timeElapsed;
                if (v57 >= 1.0)
                    v57 = 1.0;
                v58 = v55[1].y;
                ++v6;
                ++v322;
                v284 = (float)((v58 + v55[0].y) * 0.5);
                v59 = (1.0 - v57) * ((v55[1].y - v55[0].y) * 0.5);
                v60 = v284 - v59;
                v55[3].y = (float)v60;
                v55[0].y = (float)v60;
                v61 = v59 + v284;
                v55[2].y = (float)v61;
                v55[1].y = (float)v61;
                v55 += 4;
            } while (v6 < v7);
        }
        v62 = TIM_gf_dt + inner1->color1;
        inner1->color1 = (float)v62;
        if (v62 >= inner1->color2 + inner1->f1)
            goto LABEL_26;
        goto LABEL_43;
    case 8u:
        v323 = 0;
        v300 = (float)((inner1->f1 - 0.050000001 - inner1->timeElapsed) / (double)v330);
        if (v6 < v7)
        {
            v63 = &GFX_gpst_Geo->vertices[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
            v64 = ((unsigned int)((char*)v7 - (char*)v6 - 1) >> 4) + 1;
            do
            {
                v65 = (inner1->color1 - inner1->color2 - (double)v323 * v300) / inner1->timeElapsed;
                v309 = (float)v65;
                if (v65 < 1.0)
                {
                    if (v309 < 0.0)
                        v309 = 0.0;
                    v66 = (v63[1].y + v63[0].y) * 0.5;
                    v67 = (v63[1].y - v63[0].y) * 0.5 * v309;
                    v63[3].y = (float)(v66 - v67);
                    v63[0].y = (float)(v66 - v67);
                    v68 = v66 + v67;
                    v63[2].y = (float)v68;
                    v63[1].y = (float)v68;
                }
                v63 += 4;
                --v64;
                ++v323;
            } while (v64);
        }
        v69 = TIM_gf_dt + inner1->color1;
        inner1->color1 = (float)v69;
        if (v69 < inner1->color2 + inner1->f1)
            goto LABEL_43;
        return 1;
    case 9u:
    case 10u:
        v70 = GFX_gpst_Geo->vertices;
        v71 = &v70[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        v72 = v71;
        v334 = v71->x;
        v73 = &v70[v8];
        inner1e = v334;
        v74 = v71->y;
        v285 = v71->y;
        if (v71 < v73)
        {
            do
            {
                if (v72->x < (double)inner1e)
                    inner1e = v72->x;
                if (v72->y < (double)v285)
                    v285 = v72->y;
                if (v72->x > (double)v334)
                    v334 = v72->x;
                if (v74 < v72->y)
                    v74 = v72->y;
                ++v72;
            } while (v72 < v73);
        }
        v75 = (inner1->color1 - inner1->color2) / inner1->f1;
        if (v75 < 0.0)
            v75 = 0.0;
        v301 = (float)(v75 * v75);
        if (flagsLOByte == 9)
            v301 = (float)(1.0 - v301);
        v76 = v71;
        inner1f = (float)((v334 + inner1e) * 0.5);
        for (l = (float)((v74 + v285) * 0.5); v76 < v73; v76++)
        {
            v77 = v76->x;
            v76->x = (float)((v77 - inner1f) * v301 + inner1f);
            v76->y = (v76->y - l) * v301 + l;
        }
        v78 = TIM_gf_dt + inner1->color1;
        inner1->color1 = (float)v78;
        if (v78 < inner1->color2 + inner1->f1)
            goto LABEL_43;
        v17 = LOBYTE(inner1->flags) == 9;
        goto LABEL_25;
    case 11u:
    case 12u:
        v79 = GFX_gpst_Geo;
        v80 = GFX_gpst_Geo->vertices;
        v81 = &v80[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        v82 = v81;
        v335 = v81->x;
        v83 = &v80[v8];
        inner1g = v335;
        v84 = v81->y;
        v287 = v81->y;
        if (v81 < v83)
        {
            do
            {
                if (v82->x < (double)inner1g)
                    inner1g = v82->x;
                if (v82->y < (double)v287)
                    v287 = v82->y;
                if (v82->x > (double)v335)
                    v335 = v82->x;
                if (v84 < v82->y)
                    v84 = v82->y;
                ++v82;
            } while (v82 < v83);
        }
        if (inner1->timeElapsed < (double)inner1->color1)
        {
            if (flagsLOByte == 12)
            {
                v85 = v81;
                if (v81 < v83)
                {
                    do
                    {
                        v85->y = -1.0;
                        v85->x = -1.0;
                        ++v85;
                    } while (v85 < v83);
                    v79 = GFX_gpst_Geo;
                }
                v86 = v79->vertices;
                v87 = &v86[v345];
                for (m = &v86[v350 + v345]; v87 < m; ++v87)
                {
                    v87->y = -1.0;
                    v87->x = -1.0;
                }
            }
            result = 0;
            inner1->timeElapsed = TIM_gf_dt + inner1->timeElapsed;
            return result;
        }
        v89 = (inner1->timeElapsed - inner1->color1) / inner1->f1;
        v302 = (float)(v89 * v89);
        if (flagsLOByte == 12)
            v302 = (float)(1.0 - v302);
        v310 = (float)((inner1->color2 - 1.0) * v302 + 1.0);
        inner1h = (float)((v335 + inner1g) * 0.5);
        v288 = (float)((v84 + v287) * 0.5);
        v90 = LIGHT_ul_Interpol2Colors(GFX_gpst_Geo->vertexColors[v9 + 1], 0, v302);
        v91 = GFX_gpst_Geo;
        v92 = &GFX_gpst_Geo->vertexColors[v9 + 1];
        if (v81 < v83)
        {
            do
            {
                v93 = v81->x;
                v81->x = (float)((v93 - inner1h) * v310 + inner1h);
                v81->y = (v81->y - v288) * v310 + v288;
                *v92 = v90;
                ++v81;
                ++v92;
            } while (v81 < v83);
            v91 = GFX_gpst_Geo;
        }
        if (v350)
        {
            v94 = v91->vertices;
            v95 = &v94[v345 + v350];
            v96 = &v94[v345];
            v97 = LIGHT_ul_Interpol2Colors(v91->vertexColors[v345 + 1], 0, v302);
            for (n = &GFX_gpst_Geo->vertexColors[v345 + 1]; v96 < v95; )
            {
                v99 = v96->x;
                v96->x = (float)((v99 - inner1h) * v310 + inner1h);
                v96->y = (v96->y - v288) * v310 + v288;
                *n = v97;
                ++v96;
                ++n;
            }
        }
        v100 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v100;
        if (v100 < inner1->f1 + inner1->color1)
            goto LABEL_43;
        v17 = LOBYTE(inner1->flags) == 11;
    LABEL_25:
        if (v17)
            LABEL_26 :
            v306->flags |= 0x400u; // HIBYTE 0x4
        return 1;
    case 13u:
    case 14u:
        v101 = GFX_gpst_Geo->vertices;
        v102 = inner1->timeElapsed / inner1->f1;
        v103 = &v101[v8];
        v104 = &v101[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        if (flagsLOByte == 14)
            v102 = 1.0 - v102;
        inner1p = (float)(v102 * 1.5707964);
        v311 = MATH_GetCos(inner1p);
        for (ii = MATH_GetSin(inner1p); v104 < v103; v104++)
        {
            v105 = v104->y;
            v106 = (v105 - inner1->color1) * ii + inner1->color1;
            v104->y = (float)v106;
            v104->z = (float)(fabs(v106 - inner1->color1) * v311 + v104->z);
        }
        v107 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v107;
        if (v107 < inner1->f1)
            goto LABEL_43;
        v17 = LOBYTE(inner1->flags) == 14;
        goto LABEL_25;
    case 15u:
        v108 = GFX_gpst_Geo->vertices;
        v109 = 4 * specularColorPtr[(16 * (v344 + v356)) >> 4];
        v110 = &v108[v109];
        v336 = v108[v109].x;
        inner1i = v336;
        v111 = &v108[v8];
        v112 = v108[v109].y;
        v289 = v108[v109].y;
        if (&v108[v109] < v111)
        {
            do
            {
                if (v110->x < (double)inner1i)
                    inner1i = v110->x;
                if (v110->y < (double)v289)
                    v289 = v110->y;
                if (v110->x > (double)v336)
                    v336 = v110->x;
                if (v112 < v110->y)
                    v112 = v110->y;
                ++v110;
            } while (v110 < v111);
        }
        inner1j = (float)((v336 + inner1i) * 0.5);
        v290 = (float)((v112 + v289) * 0.5);
        v304 = MATH_GetSin(inner1->f1);
        v312 = MATH_GetCos(inner1->f1);
        inner1->f1 = TIM_gf_dt * inner1->timeElapsed + inner1->f1;
        for (jj = &GFX_gpst_Geo->vertices[v109]; jj < v111; jj++)
        {
            v114 = jj->x;
            v115 = v114 - inner1j;
            v116 = jj->y - v290;
            v324 = (float)(v312 * v116 + v115 * v304);
            jj->x = (float)(v115 * v312 - v116 * v304 + inner1j);
            jj->y = v324 + v290;
        }
        v117 = GFX_gpst_Geo->vertices;
        v118 = &v117[v345];
        v119 = &v117[v350 + v345];
        if (v118 >= v119)
            goto LABEL_43;
        do
        {
            v120 = v118->x;
            v121 = v120 - inner1j;
            v122 = v118->y - v290;
            v325 = (float)(v312 * v122 + v121 * v304);
            v118->x = (float)(v121 * v312 - v122 * v304 + inner1j);
            v118->y = v325 + v290;
            ++v118;
        } while (v118 < v119);
        return 0;
    case 16u:
        v123 = GFX_gpst_Geo->vertices;
        v326 = v123[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]].y;
        v124 = 0;
        v291 = v326;
        v125 = v123[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]].x;
        inner1k = v123[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]].x;
        if (v330 >= 4)
        {
            v126 = &specularColorPtr[v344 + 2 + v356];
            for (kk = 3; kk < v330; kk += 4)
            {
                v127 = &v123[4 * *(v126 - 2)];
                if (v127->y > (double)v326)
                    v326 = v127->y;
                if (v127[1].y < (double)v291)
                    v291 = v127[1].y;
                if (v125 < v127[2].x)
                    v125 = v127[2].x;
                if (v127->x < (double)inner1k)
                    inner1k = v127->x;
                v128 = &v123[4 * *(v126 - 1)];
                if (v128->y > (double)v326)
                    v326 = v128->y;
                if (v128[1].y < (double)v291)
                    v291 = v128[1].y;
                if (v125 < v128[2].x)
                    v125 = v128[2].x;
                if (v128->x < (double)inner1k)
                    inner1k = v128->x;
                v129 = 4 * *v126;
                v130 = v123[v129].y;
                v131 = &v123[v129];
                if (v130 > v326)
                    v326 = v131->y;
                if (v131[1].y < (double)v291)
                    v291 = v131[1].y;
                if (v125 < v131[2].x)
                    v125 = v131[2].x;
                if (v131->x < (double)inner1k)
                    inner1k = v131->x;
                v132 = &v123[4 * v126[1]];
                if (v132->y > (double)v326)
                    v326 = v132->y;
                if (v132[1].y < (double)v291)
                    v291 = v132[1].y;
                if (v125 < v132[2].x)
                    v125 = v132[2].x;
                if (v132->x < (double)inner1k)
                    inner1k = v132->x;
                v124 += 4;
                v126 += 4;
            }
        }
        if (v124 < v330)
        {
            v133 = &specularColorPtr[v344 + v124 + v356];
            v134 = v330 - v124;
            do
            {
                v135 = 4 * *v133;
                v136 = v123[v135].y;
                v137 = &v123[v135];
                if (v136 > v326)
                    v326 = v137->y;
                if (v137[1].y < (double)v291)
                    v291 = v137[1].y;
                if (v125 < v137[2].x)
                    v125 = v137[2].x;
                if (v137->x < (double)inner1k)
                    inner1k = v137->x;
                ++v133;
                --v134;
            } while (v134);
        }
        v138 = *(int32_t*)&(inner1->color2);
        v327 = v326 - v291;
        v337 = (float)(v125 - inner1k);
        v139 = inner1->timeElapsed / inner1->f1;
        if (v138 == 0 || v138 == 1)
        {
            v202 = 1 - v138;
            v353 = 3 - (v138 != 0);
            v346 = (v138 != 0) + 2;
            if (v138 != 0.0)
                v139 = 1.0 - v139;
            v203 = v139 * v327;
            v328 = 0;
            v292 = (float)(v203 + v291);
            if (v330 >= 4)
            {
                while (1)
                {
                    v204 = a1->specularColorsPtr[v344 + v328 + v306->index];
                    v205 = &GFX_gpst_Geo->vertices[4 * v204];
                    if (v205[1].y > (double)v292)
                        break;
                    if (v205->y < (double)v292)
                    {
                        if (v138 != 0)
                        {
                            v205[3].x = -100.0f;
                            v205[2].x = -100.0f;
                            v205[1].x = -100.0f;
                            v205->x = -100.0f;
                            v205[3].y = -100.0f;
                            v205[2].y = -100.0f;
                            v205[1].y = -100.0f;
                            v205->y = -100.0f;
                        }
                    LABEL_281:
                        v205[7].x = -100.0f;
                        v205[6].x = -100.0f;
                        v205[5].x = -100.0f;
                        v205[4].x = -100.0f;
                        v205[7].y = -100.0f;
                        v205[6].y = -100.0f;
                        v205[5].y = -100.0f;
                        v205[4].y = -100.0f;
                        goto LABEL_286;
                    }
                    v206 = &GFX_gpst_Geo->UVs[4 * v204];
                    v207 = (v292 - v205[v202].y)
                        / (v205[v138].y - v205[v202].y)
                        * (v206[v138].v - v206[v202].v)
                        + v206[v202].v;
                    v206[v353].v = (float)v207;
                    v206[v138].v = (float)v207;
                    v313 = v353;
                    v205[v313].y = (float)v292;
                    v208 = v138;
                    v205[v208].y = (float)v292;
                    v209 = v206[v138].v;
                    v206[7].v = v206[v138].v;
                    v206[6].v = (float)v209;
                    v206[5].v = (float)v209;
                    v206[4].v = (float)v209;
                    v205[v313 + 4].y = (float)v292;
                    v205[v208 + 4].y = (float)v292;
                    if (v138 == 0)
                        v338 = (uint16_t)a1->height;
                    else
                        v338 = 0;
                    v210 = (double)v338;
                    v205[v346 + 4].y = (float)v210;
                    v205[v202 + 4].y = (float)v210;
                    v205[v202 + 4].x = v205[v202 + 4].x + inner1->color1;
                    v205[v346 + 4].x = inner1->color1 + v205[v346 + 4].x;
                LABEL_286:
                    v211 = a1->specularColorsPtr[v344 + 1 + v328 + v306->index];
                    v212 = &GFX_gpst_Geo->vertices[4 * v211];
                    if (v212[1].y > (double)v292)
                    {
                        if (v202)
                        {
                            v212[3].x = -100.0f;
                            v212[2].x = -100.0f;
                            v212[1].x = -100.0f;
                            v212->x = -100.0f;
                            v212[3].y = -100.0f;
                            v212[2].y = -100.0f;
                            v212[1].y = -100.0f;
                            v212->y = -100.0f;
                            v212[7].x = -100.0f;
                            v212[6].x = -100.0f;
                            v212[5].x = -100.0f;
                            v212[4].x = -100.0f;
                            v212[7].y = -100.0f;
                            v212[6].y = -100.0f;
                            v212[5].y = -100.0f;
                            v212[4].y = -100.0f;
                            goto LABEL_297;
                        }
                        goto LABEL_292;
                    }
                    if (v212->y < (double)v292)
                    {
                        if (v138 != 0.0)
                        {
                            v212[3].x = -100.0f;
                            v212[2].x = -100.0f;
                            v212[1].x = -100.0f;
                            v212->x = -100.0f;
                            v212[3].y = -100.0f;
                            v212[2].y = -100.0f;
                            v212[1].y = -100.0f;
                            v212->y = -100.0f;
                        }
                    LABEL_292:
                        v212[7].x = -100.0f;
                        v212[6].x = -100.0f;
                        v212[5].x = -100.0f;
                        v212[4].x = -100.0f;
                        v212[7].y = -100.0f;
                        v212[6].y = -100.0f;
                        v212[5].y = -100.0f;
                        v212[4].y = -100.0f;
                        goto LABEL_297;
                    }
                    v213 = &GFX_gpst_Geo->UVs[4 * v211];
                    v214 = (v292 - v212[v202].y)
                        / (v212[v138].y - v212[v202].y)
                        * (v213[v138].v - v213[v202].v)
                        + v213[v202].v;
                    v213[v353].v = (float)v214;
                    v213[v138].v = (float)v214;
                    v314 = v353;
                    v212[v314].y = v292;
                    v215 = v138;
                    v212[v215].y = v292;
                    v216 = v213[v138].v;
                    v213[7].v = v213[v138].v;
                    v213[6].v = (float)v216;
                    v213[5].v = (float)v216;
                    v213[4].v = (float)v216;
                    v212[v314 + 4].y = v292;
                    v212[v215 + 4].y = v292;
                    if (v138 == 0)
                        v339 = (uint16_t)a1->height;
                    else
                        v339 = 0;
                    v217 = (double)v339;
                    v212[v346 + 4].y = (float)v217;
                    v212[v202 + 4].y = (float)v217;
                    v212[v202 + 4].x = v212[v202 + 4].x + inner1->color1;
                    v212[v346 + 4].x = inner1->color1 + v212[v346 + 4].x;
                LABEL_297:
                    v218 = a1->specularColorsPtr[v344 + 2 + v328 + v306->index];
                    v219 = &GFX_gpst_Geo->vertices[4 * v218];
                    if (v219[1].y > (double)v292)
                    {
                        if (v202)
                        {
                            v219[3].x = -100.0f;
                            v219[2].x = -100.0f;
                            v219[1].x = -100.0f;
                            v219->x = -100.0f;
                            v219[3].y = -100.0f;
                            v219[2].y = -100.0f;
                            v219[1].y = -100.0f;
                            v219->y = -100.0f;
                            v219[7].x = -100.0f;
                            v219[6].x = -100.0f;
                            v219[5].x = -100.0f;
                            v219[4].x = -100.0f;
                            v219[7].y = -100.0f;
                            v219[6].y = -100.0f;
                            v219[5].y = -100.0f;
                            v219[4].y = -100.0f;
                            goto LABEL_308;
                        }
                        goto LABEL_303;
                    }
                    if (v219->y < (double)v292)
                    {
                        if (v138 != 0)
                        {
                            v219[3].x = -100.0f;
                            v219[2].x = -100.0f;
                            v219[1].x = -100.0f;
                            v219->x = -100.0f;
                            v219[3].y = -100.0f;
                            v219[2].y = -100.0f;
                            v219[1].y = -100.0f;
                            v219->y = -100.0f;
                        }
                    LABEL_303:
                        v219[7].x = -100.0f;
                        v219[6].x = -100.0f;
                        v219[5].x = -100.0f;
                        v219[4].x = -100.0f;
                        v219[7].y = -100.0f;
                        v219[6].y = -100.0f;
                        v219[5].y = -100.0f;
                        v219[4].y = -100.0f;
                        goto LABEL_308;
                    }
                    v220 = &GFX_gpst_Geo->UVs[4 * v218];
                    v221 = (v292 - v219[v202].y)
                        / (v219[v138].y - v219[v202].y)
                        * (v220[v138].v - v220[v202].v)
                        + v220[v202].v;
                    v220[v353].v = (float)v221;
                    v220[v138].v = (float)v221;
                    v315 = v353;
                    v219[v315].y = v292;
                    v222 = v138;
                    v219[v222].y = v292;
                    v223 = v220[v138].v;
                    v220[7].v = v220[v138].v;
                    v220[6].v = (float)v223;
                    v220[5].v = (float)v223;
                    v220[4].v = (float)v223;
                    v219[v315 + 4].y = v292;
                    v219[v222 + 4].y = v292;
                    if (v138 == 0)
                        v340 = (uint16_t)a1->height;
                    else
                        v340 = 0;
                    v224 = (double)v340;
                    v219[v346 + 4].y = (float)v224;
                    v219[v202 + 4].y = (float)v224;
                    v219[v202 + 4].x = v219[v202 + 4].x + inner1->color1;
                    v219[v346 + 4].x = inner1->color1 + v219[v346 + 4].x;
                LABEL_308:
                    v225 = a1->specularColorsPtr[v344 + 3 + v328 + v306->index];
                    v226 = &GFX_gpst_Geo->vertices[4 * v225];
                    if (v226[1].y <= (double)v292)
                    {
                        if (v226->y >= (double)v292)
                        {
                            v227 = &GFX_gpst_Geo->UVs[4 * v225];
                            v228 = (v292 - v226[v202].y)
                                / (v226[v138].y - v226[v202].y)
                                * (v227[v138].v - v227[v202].v)
                                + v227[v202].v;
                            v227[v353].v = (float)v228;
                            v227[v138].v = (float)v228;
                            v316 = v353;
                            v226[v316].y = v292;
                            v229 = v138;
                            v226[v229].y = v292;
                            v230 = v227[v138].v;
                            v227[7].v = v227[v138].v;
                            v227[6].v = (float)v230;
                            v227[5].v = (float)v230;
                            v227[4].v = (float)v230;
                            v226[v316 + 4].y = v292;
                            v226[v229 + 4].y = v292;
                            if (v138 == 0)
                                v341 = (uint16_t)a1->height;
                            else
                                v341 = 0;
                            v231 = (double)v341;
                            v226[v346 + 4].y = (float)v231;
                            v226[v202 + 4].y = (float)v231;
                            v226[v202 + 4].x = v226[v202 + 4].x + inner1->color1;
                            v226[v346 + 4].x = inner1->color1 + v226[v346 + 4].x;
                            goto LABEL_319;
                        }
                        if (v138 != 0)
                        {
                            v226[3].x = -100.0f;
                            v226[2].x = -100.0f;
                            v226[1].x = -100.0f;
                            v226->x = -100.0f;
                            v226[3].y = -100.0f;
                            v226[2].y = -100.0f;
                            v226[1].y = -100.0f;
                            v226->y = -100.0f;
                        }
                    LABEL_314:
                        v226[7].x = -100.0f;
                        v226[6].x = -100.0f;
                        v226[5].x = -100.0f;
                        v226[4].x = -100.0f;
                        v226[7].y = -100.0f;
                        v226[6].y = -100.0f;
                        v226[5].y = -100.0f;
                        v226[4].y = -100.0f;
                        goto LABEL_319;
                    }
                    if (!v202)
                        goto LABEL_314;
                    v226[3].x = -100.0f;
                    v226[2].x = -100.0f;
                    v226[1].x = -100.0f;
                    v226->x = -100.0f;
                    v226[3].y = -100.0f;
                    v226[2].y = -100.0f;
                    v226[1].y = -100.0f;
                    v226->y = -100.0f;
                    v226[7].x = -100.0f;
                    v226[6].x = -100.0f;
                    v226[5].x = -100.0f;
                    v226[4].x = -100.0f;
                    v226[7].y = -100.0f;
                    v226[6].y = -100.0f;
                    v226[5].y = -100.0f;
                    v226[4].y = -100.0f;
                LABEL_319:
                    v328 += 4;
                    if (v328 + 3 >= v330)
                        goto LABEL_320;
                }
                if (v202)
                {
                    v205[3].x = -100.0f;
                    v205[2].x = -100.0f;
                    v205[1].x = -100.0f;
                    v205->x = -100.0f;
                    v205[3].y = -100.0f;
                    v205[2].y = -100.0f;
                    v205[1].y = -100.0f;
                    v205->y = -100.0f;
                    v205[7].x = -100.0f;
                    v205[6].x = -100.0f;
                    v205[5].x = -100.0f;
                    v205[4].x = -100.0f;
                    v205[7].y = -100.0f;
                    v205[6].y = -100.0f;
                    v205[5].y = -100.0f;
                    v205[4].y = -100.0f;
                    goto LABEL_286;
                }
                goto LABEL_281;
            }
        LABEL_320:
            if (v328 >= v330)
                goto LABEL_333;
            while (1)
            {
                v232 = a1->specularColorsPtr[v344 + v328 + v306->index];
                v233 = &GFX_gpst_Geo->vertices[4 * v232];
                if (v233[1].y <= (double)v292)
                {
                    if (v233->y >= (double)v292)
                    {
                        v234 = &GFX_gpst_Geo->UVs[4 * v232];
                        v235 = (v292 - v233[v202].y)
                            / (v233[v138].y - v233[v202].y)
                            * (v234[v138].v - v234[v202].v)
                            + v234[v202].v;
                        v234[v353].v = (float)v235;
                        v234[v138].v = (float)v235;
                        v317 = v353;
                        v233[v317].y = v292;
                        v236 = v138;
                        v233[v236].y = v292;
                        v237 = v234[v138].v;
                        v234[7].v = v234[v138].v;
                        v234[6].v = (float)v237;
                        v234[5].v = (float)v237;
                        v234[4].v = (float)v237;
                        v233[v317 + 4].y = v292;
                        v233[v236 + 4].y = v292;
                        if (v138 == 0)
                            v342 = (uint16_t)a1->height;
                        else
                            v342 = 0;
                        v238 = (double)v342;
                        v233[v346 + 4].y = (float)v238;
                        v233[v202 + 4].y = (float)v238;
                        v233[v202 + 4].x = v233[v202 + 4].x + inner1->color1;
                        v233[v346 + 4].x = inner1->color1 + v233[v346 + 4].x;
                        goto LABEL_332;
                    }
                    if (v138 != 0)
                    {
                        v233[3].x = -100.0f;
                        v233[2].x = -100.0f;
                        v233[1].x = -100.0f;
                        v233->x = -100.0f;
                        v233[3].y = -100.0f;
                        v233[2].y = -100.0f;
                        v233[1].y = -100.0f;
                        v233->y = -100.0f;
                        v233[7].x = -100.0f;
                        v233[6].x = -100.0f;
                        v233[5].x = -100.0f;
                        v233[4].x = -100.0f;
                        v233[7].y = -100.0f;
                        v233[6].y = -100.0f;
                        v233[5].y = -100.0f;
                        v233[4].y = -100.0f;
                        goto LABEL_332;
                    }
                }
                else if (v202)
                {
                    v233[3].x = -100.0f;
                    v233[2].x = -100.0f;
                    v233[1].x = -100.0f;
                    v233->x = -100.0f;
                    v233[3].y = -100.0f;
                    v233[2].y = -100.0f;
                    v233[1].y = -100.0f;
                    v233->y = -100.0f;
                }
                v233[7].x = -100.0f;
                v233[6].x = -100.0f;
                v233[5].x = -100.0f;
                v233[4].x = -100.0f;
                v233[7].y = -100.0f;
                v233[6].y = -100.0f;
                v233[5].y = -100.0f;
                v233[4].y = -100.0f;
            LABEL_332:
                if (++v328 >= v330)
                    goto LABEL_333;
            }
        }
        if (v138 == 2)
        {
            v140 = 0;
            inner1l = (float)(v139 * v337 + inner1k);
            if (v330 >= 4)
            {
                do
                {
                    v141 = a1->specularColorsPtr[v344 + v140 + v306->index];
                    v142 = &GFX_gpst_Geo->vertices[4 * v141];
                    if (v142[3].x >= (double)inner1l)
                    {
                        if (v142->x <= (double)inner1l)
                        {
                            v143 = &GFX_gpst_Geo->UVs[4 * v141];
                            v144 = (inner1l - v142->x) / (v142[3].x - v142->x) * (v143[3].u - v143->u) + v143->u;
                            v143[3].u = (float)v144;
                            v143[2].u = (float)v144;
                            v142[3].x = inner1l;
                            v142[2].x = inner1l;
                            v145 = v143[2].u;
                            v143[7].u = v143[2].u;
                            v143[6].u = (float)v145;
                            v143[5].u = (float)v145;
                            v143[4].u = (float)v145;
                            v142[5].x = inner1l;
                            v142[4].x = inner1l;
                            v146 = (double)(uint16_t)a1->width;
                            v142[7].x = (float)v146;
                            v142[6].x = (float)v146;
                            v142[6].y = v142[6].y + inner1->color1;
                            v142[7].y = inner1->color1 + v142[7].y;
                        }
                        else
                        {
                            v142[3].x = -100.0f;
                            v142[2].x = -100.0f;
                            v142[1].x = -100.0f;
                            v142->x = -100.0f;
                            v142[7].x = -100.0f;
                            v142[6].x = -100.0f;
                            v142[5].x = -100.0f;
                            v142[4].x = -100.0f;
                            v142[3].y = -100.0f;
                            v142[2].y = -100.0f;
                            v142[1].y = -100.0f;
                            v142->y = -100.0f;
                            v142[7].y = -100.0f;
                            v142[6].y = -100.0f;
                            v142[5].y = -100.0f;
                            v142[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v142[7].x = -100.0f;
                        v142[6].x = -100.0f;
                        v142[5].x = -100.0f;
                        v142[4].x = -100.0f;
                        v142[7].y = -100.0f;
                        v142[6].y = -100.0f;
                        v142[5].y = -100.0f;
                        v142[4].y = -100.0f;
                    }
                    v147 = a1->specularColorsPtr[v344 + 1 + v140 + v306->index];
                    v148 = &GFX_gpst_Geo->vertices[4 * v147];
                    if (v148[3].x >= (double)inner1l)
                    {
                        if (v148->x <= (double)inner1l)
                        {
                            v149 = &GFX_gpst_Geo->UVs[4 * v147];
                            v150 = (inner1l - v148->x) / (v148[3].x - v148->x) * (v149[3].u - v149->u) + v149->u;
                            v149[3].u = (float)v150;
                            v149[2].u = (float)v150;
                            v148[3].x = inner1l;
                            v148[2].x = inner1l;
                            v151 = v149[2].u;
                            v149[7].u = v149[2].u;
                            v149[6].u = (float)v151;
                            v149[5].u = (float)v151;
                            v149[4].u = (float)v151;
                            v148[5].x = inner1l;
                            v148[4].x = inner1l;
                            v152 = (double)(uint16_t)a1->width;
                            v148[7].x = (float)v152;
                            v148[6].x = (float)v152;
                            v148[6].y = v148[6].y + inner1->color1;
                            v148[7].y = inner1->color1 + v148[7].y;
                        }
                        else
                        {
                            v148[3].x = -100.0f;
                            v148[2].x = -100.0f;
                            v148[1].x = -100.0f;
                            v148->x = -100.0f;
                            v148[7].x = -100.0f;
                            v148[6].x = -100.0f;
                            v148[5].x = -100.0f;
                            v148[4].x = -100.0f;
                            v148[3].y = -100.0f;
                            v148[2].y = -100.0f;
                            v148[1].y = -100.0f;
                            v148->y = -100.0f;
                            v148[7].y = -100.0f;
                            v148[6].y = -100.0f;
                            v148[5].y = -100.0f;
                            v148[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v148[7].x = -100.0f;
                        v148[6].x = -100.0f;
                        v148[5].x = -100.0f;
                        v148[4].x = -100.0f;
                        v148[7].y = -100.0f;
                        v148[6].y = -100.0f;
                        v148[5].y = -100.0f;
                        v148[4].y = -100.0f;
                    }
                    v153 = a1->specularColorsPtr[v344 + 2 + v140 + v306->index];
                    v154 = &GFX_gpst_Geo->vertices[4 * v153];
                    if (v154[3].x >= (double)inner1l)
                    {
                        if (v154->x <= (double)inner1l)
                        {
                            v155 = &GFX_gpst_Geo->UVs[4 * v153];
                            v156 = (inner1l - v154->x) / (v154[3].x - v154->x) * (v155[3].u - v155->u) + v155->u;
                            v155[3].u = (float)v156;
                            v155[2].u = (float)v156;
                            v154[3].x = inner1l;
                            v154[2].x = inner1l;
                            v157 = v155[2].u;
                            v155[7].u = v155[2].u;
                            v155[6].u = (float)v157;
                            v155[5].u = (float)v157;
                            v155[4].u = (float)v157;
                            v154[5].x = inner1l;
                            v154[4].x = inner1l;
                            v158 = (double)(uint16_t)a1->width;
                            v154[7].x = (float)v158;
                            v154[6].x = (float)v158;
                            v154[6].y = v154[6].y + inner1->color1;
                            v154[7].y = inner1->color1 + v154[7].y;
                        }
                        else
                        {
                            v154[3].x = -100.0f;
                            v154[2].x = -100.0f;
                            v154[1].x = -100.0f;
                            v154->x = -100.0f;
                            v154[7].x = -100.0f;
                            v154[6].x = -100.0f;
                            v154[5].x = -100.0f;
                            v154[4].x = -100.0f;
                            v154[3].y = -100.0f;
                            v154[2].y = -100.0f;
                            v154[1].y = -100.0f;
                            v154->y = -100.0f;
                            v154[7].y = -100.0f;
                            v154[6].y = -100.0f;
                            v154[5].y = -100.0f;
                            v154[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v154[7].x = -100.0f;
                        v154[6].x = -100.0f;
                        v154[5].x = -100.0f;
                        v154[4].x = -100.0f;
                        v154[7].y = -100.0f;
                        v154[6].y = -100.0f;
                        v154[5].y = -100.0f;
                        v154[4].y = -100.0f;
                    }
                    v159 = a1->specularColorsPtr[v344 + 3 + v140 + v306->index];
                    v160 = &GFX_gpst_Geo->vertices[4 * v159];
                    if (v160[3].x >= (double)inner1l)
                    {
                        if (v160->x <= (double)inner1l)
                        {
                            v161 = &GFX_gpst_Geo->UVs[4 * v159];
                            v162 = (inner1l - v160->x) / (v160[3].x - v160->x) * (v161[3].u - v161->u) + v161->u;
                            v161[3].u = (float)v162;
                            v161[2].u = (float)v162;
                            v160[3].x = inner1l;
                            v160[2].x = inner1l;
                            v163 = v161[2].u;
                            v161[7].u = v161[2].u;
                            v161[6].u = (float)v163;
                            v161[5].u = (float)v163;
                            v161[4].u = (float)v163;
                            v160[5].x = inner1l;
                            v160[4].x = inner1l;
                            v164 = (double)(uint16_t)a1->width;
                            v160[7].x = (float)v164;
                            v160[6].x = (float)v164;
                            v160[6].y = v160[6].y + inner1->color1;
                            v160[7].y = inner1->color1 + v160[7].y;
                        }
                        else
                        {
                            v160[3].x = -100.0f;
                            v160[2].x = -100.0f;
                            v160[1].x = -100.0f;
                            v160->x = -100.0f;
                            v160[7].x = -100.0f;
                            v160[6].x = -100.0f;
                            v160[5].x = -100.0f;
                            v160[4].x = -100.0f;
                            v160[3].y = -100.0f;
                            v160[2].y = -100.0f;
                            v160[1].y = -100.0f;
                            v160->y = -100.0f;
                            v160[7].y = -100.0f;
                            v160[6].y = -100.0f;
                            v160[5].y = -100.0f;
                            v160[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v160[7].x = -100.0f;
                        v160[6].x = -100.0f;
                        v160[5].x = -100.0f;
                        v160[4].x = -100.0f;
                        v160[7].y = -100.0f;
                        v160[6].y = -100.0f;
                        v160[5].y = -100.0f;
                        v160[4].y = -100.0f;
                    }
                    v140 += 4;
                } while (v140 + 3 < v330);
            }
            for (; v140 < v330; ++v140)
            {
                v165 = a1->specularColorsPtr[v344 + v140 + v306->index];
                v166 = &GFX_gpst_Geo->vertices[4 * v165];
                if (v166[3].x >= (double)inner1l)
                {
                    if (v166->x <= (double)inner1l)
                    {
                        v167 = &GFX_gpst_Geo->UVs[4 * v165];
                        v168 = (inner1l - v166->x) / (v166[3].x - v166->x) * (v167[3].u - v167->u) + v167->u;
                        v167[3].u = (float)v168;
                        v167[2].u = (float)v168;
                        v166[3].x = inner1l;
                        v166[2].x = inner1l;
                        v169 = v167[2].u;
                        v167[7].u = v167[2].u;
                        v167[6].u = (float)v169;
                        v167[5].u = (float)v169;
                        v167[4].u = (float)v169;
                        v166[5].x = inner1l;
                        v166[4].x = inner1l;
                        v170 = (double)(uint16_t)a1->width;
                        v166[7].x = (float)v170;
                        v166[6].x = (float)v170;
                        v166[6].y = v166[6].y + inner1->color1;
                        v166[7].y = inner1->color1 + v166[7].y;
                    }
                    else
                    {
                        v166[3].x = -100.0f;
                        v166[2].x = -100.0f;
                        v166[1].x = -100.0f;
                        v166->x = -100.0f;
                        v166[7].x = -100.0f;
                        v166[6].x = -100.0f;
                        v166[5].x = -100.0f;
                        v166[4].x = -100.0f;
                        v166[3].y = -100.0f;
                        v166[2].y = -100.0f;
                        v166[1].y = -100.0f;
                        v166->y = -100.0f;
                        v166[7].y = -100.0f;
                        v166[6].y = -100.0f;
                        v166[5].y = -100.0f;
                        v166[4].y = -100.0f;
                    }
                }
                else
                {
                    v166[7].x = -100.0f;
                    v166[6].x = -100.0f;
                    v166[5].x = -100.0f;
                    v166[4].x = -100.0f;
                    v166[7].y = -100.0f;
                    v166[6].y = -100.0f;
                    v166[5].y = -100.0f;
                    v166[4].y = -100.0f;
                }
            }
        }
        else if (v138 == 3)
        {
            v171 = 0;
            inner1m = (float)((1.0 - v139) * v337 + inner1k);
            if (v330 >= 4)
            {
                do
                {
                    v172 = a1->specularColorsPtr[v344 + v171 + v306->index];
                    v173 = &GFX_gpst_Geo->vertices[4 * v172];
                    if (v173[3].x >= (double)inner1m)
                    {
                        if (v173->x <= (double)inner1m)
                        {
                            v174 = &GFX_gpst_Geo->UVs[4 * v172];
                            v175 = (inner1m - v173[3].x) / (v173->x - v173[3].x) * (v174->u - v174[3].u) + v174[3].u;
                            v174[1].u = (float)v175;
                            v174->u = (float)v175;
                            v173[1].x = inner1m;
                            v173->x = inner1m;
                            v176 = v174->u;
                            v174[7].u = v174->u;
                            v174[6].u = (float)v176;
                            v174[5].u = (float)v176;
                            v174[4].u = (float)v176;
                            v177 = v173[4].y;
                            v173[5].x = 0.0;
                            v173[4].x = 0.0;
                            v173[7].x = inner1m;
                            v173[6].x = inner1m;
                            v173[4].y = (float)(v177 + inner1->color1);
                            v173[5].y = inner1->color1 + v173[5].y;
                        }
                        else
                        {
                            v173[7].x = -100.0f;
                            v173[6].x = -100.0f;
                            v173[5].x = -100.0f;
                            v173[4].x = -100.0f;
                            v173[7].y = -100.0f;
                            v173[6].y = -100.0f;
                            v173[5].y = -100.0f;
                            v173[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v173[3].x = -100.0f;
                        v173[2].x = -100.0f;
                        v173[1].x = -100.0f;
                        v173->x = -100.0f;
                        v173[7].x = -100.0f;
                        v173[6].x = -100.0f;
                        v173[5].x = -100.0f;
                        v173[4].x = -100.0f;
                        v173[3].y = -100.0f;
                        v173[2].y = -100.0f;
                        v173[1].y = -100.0f;
                        v173->y = -100.0f;
                        v173[7].y = -100.0f;
                        v173[6].y = -100.0f;
                        v173[5].y = -100.0f;
                        v173[4].y = -100.0f;
                    }
                    v178 = a1->specularColorsPtr[v344 + 1 + v171 + v306->index];
                    v179 = &GFX_gpst_Geo->vertices[4 * v178];
                    if (v179[3].x >= (double)inner1m)
                    {
                        if (v179->x <= (double)inner1m)
                        {
                            v180 = &GFX_gpst_Geo->UVs[4 * v178];
                            v181 = (inner1m - v179[3].x) / (v179->x - v179[3].x) * (v180->u - v180[3].u) + v180[3].u;
                            v180[1].u = (float)v181;
                            v180->u = (float)v181;
                            v179[1].x = inner1m;
                            v179->x = inner1m;
                            v182 = v180->u;
                            v180[7].u = v180->u;
                            v180[6].u = (float)v182;
                            v180[5].u = (float)v182;
                            v180[4].u = (float)v182;
                            v183 = v179[4].y;
                            v179[5].x = 0.0;
                            v179[4].x = 0.0;
                            v179[7].x = inner1m;
                            v179[6].x = inner1m;
                            v179[4].y = (float)(v183 + inner1->color1);
                            v179[5].y = inner1->color1 + v179[5].y;
                        }
                        else
                        {
                            v179[7].x = -100.0f;
                            v179[6].x = -100.0f;
                            v179[5].x = -100.0f;
                            v179[4].x = -100.0f;
                            v179[7].y = -100.0f;
                            v179[6].y = -100.0f;
                            v179[5].y = -100.0f;
                            v179[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v179[3].x = -100.0f;
                        v179[2].x = -100.0f;
                        v179[1].x = -100.0f;
                        v179->x = -100.0f;
                        v179[7].x = -100.0f;
                        v179[6].x = -100.0f;
                        v179[5].x = -100.0f;
                        v179[4].x = -100.0f;
                        v179[3].y = -100.0f;
                        v179[2].y = -100.0f;
                        v179[1].y = -100.0f;
                        v179->y = -100.0f;
                        v179[7].y = -100.0f;
                        v179[6].y = -100.0f;
                        v179[5].y = -100.0f;
                        v179[4].y = -100.0f;
                    }
                    v184 = a1->specularColorsPtr[v344 + 2 + v171 + v306->index];
                    v185 = &GFX_gpst_Geo->vertices[4 * v184];
                    if (v185[3].x >= (double)inner1m)
                    {
                        if (v185->x <= (double)inner1m)
                        {
                            v186 = &GFX_gpst_Geo->UVs[4 * v184];
                            v187 = (inner1m - v185[3].x) / (v185->x - v185[3].x) * (v186->u - v186[3].u) + v186[3].u;
                            v186[1].u = (float)v187;
                            v186->u = (float)v187;
                            v185[1].x = inner1m;
                            v185->x = inner1m;
                            v188 = v186->u;
                            v186[7].u = v186->u;
                            v186[6].u = (float)v188;
                            v186[5].u = (float)v188;
                            v186[4].u = (float)v188;
                            v189 = v185[4].y;
                            v185[5].x = 0.0;
                            v185[4].x = 0.0;
                            v185[7].x = inner1m;
                            v185[6].x = inner1m;
                            v185[4].y = (float)(v189 + inner1->color1);
                            v185[5].y = inner1->color1 + v185[5].y;
                        }
                        else
                        {
                            v185[7].x = -100.0f;
                            v185[6].x = -100.0f;
                            v185[5].x = -100.0f;
                            v185[4].x = -100.0f;
                            v185[7].y = -100.0f;
                            v185[6].y = -100.0f;
                            v185[5].y = -100.0f;
                            v185[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v185[3].x = -100.0f;
                        v185[2].x = -100.0f;
                        v185[1].x = -100.0f;
                        v185->x = -100.0f;
                        v185[7].x = -100.0f;
                        v185[6].x = -100.0f;
                        v185[5].x = -100.0f;
                        v185[4].x = -100.0f;
                        v185[3].y = -100.0f;
                        v185[2].y = -100.0f;
                        v185[1].y = -100.0f;
                        v185->y = -100.0f;
                        v185[7].y = -100.0f;
                        v185[6].y = -100.0f;
                        v185[5].y = -100.0f;
                        v185[4].y = -100.0f;
                    }
                    v190 = a1->specularColorsPtr[v344 + 3 + v171 + v306->index];
                    v191 = &GFX_gpst_Geo->vertices[4 * v190];
                    if (v191[3].x >= (double)inner1m)
                    {
                        if (v191->x <= (double)inner1m)
                        {
                            v192 = &GFX_gpst_Geo->UVs[4 * v190];
                            v193 = (inner1m - v191[3].x) / (v191->x - v191[3].x) * (v192->u - v192[3].u) + v192[3].u;
                            v192[1].u = (float)v193;
                            v192->u = (float)v193;
                            v191[1].x = inner1m;
                            v191->x = inner1m;
                            v194 = v192->u;
                            v192[7].u = v192->u;
                            v192[6].u = (float)v194;
                            v192[5].u = (float)v194;
                            v192[4].u = (float)v194;
                            v195 = v191[4].y;
                            v191[5].x = 0.0;
                            v191[4].x = 0.0;
                            v191[7].x = inner1m;
                            v191[6].x = inner1m;
                            v191[4].y = (float)(v195 + inner1->color1);
                            v191[5].y = inner1->color1 + v191[5].y;
                        }
                        else
                        {
                            v191[7].x = -100.0f;
                            v191[6].x = -100.0f;
                            v191[5].x = -100.0f;
                            v191[4].x = -100.0f;
                            v191[7].y = -100.0f;
                            v191[6].y = -100.0f;
                            v191[5].y = -100.0f;
                            v191[4].y = -100.0f;
                        }
                    }
                    else
                    {
                        v191[3].x = -100.0f;
                        v191[2].x = -100.0f;
                        v191[1].x = -100.0f;
                        v191->x = -100.0f;
                        v191[7].x = -100.0f;
                        v191[6].x = -100.0f;
                        v191[5].x = -100.0f;
                        v191[4].x = -100.0f;
                        v191[3].y = -100.0f;
                        v191[2].y = -100.0f;
                        v191[1].y = -100.0f;
                        v191->y = -100.0f;
                        v191[7].y = -100.0f;
                        v191[6].y = -100.0f;
                        v191[5].y = -100.0f;
                        v191[4].y = -100.0f;
                    }
                    v171 += 4;
                } while (v171 + 3 < v330);
            }
            for (; v171 < v330; ++v171)
            {
                v196 = a1->specularColorsPtr[v344 + v171 + v306->index];
                v197 = &GFX_gpst_Geo->vertices[4 * v196];
                if (v197[3].x >= (double)inner1m)
                {
                    if (v197->x <= (double)inner1m)
                    {
                        v198 = &GFX_gpst_Geo->UVs[4 * v196];
                        v199 = (inner1m - v197[3].x) / (v197->x - v197[3].x) * (v198->u - v198[3].u) + v198[3].u;
                        v198[1].u = (float)v199;
                        v198->u = (float)v199;
                        v197[1].x = inner1m;
                        v197->x = inner1m;
                        v200 = v198->u;
                        v198[7].u = v198->u;
                        v198[6].u = (float)v200;
                        v198[5].u = (float)v200;
                        v198[4].u = (float)v200;
                        v201 = v197[4].y;
                        v197[5].x = 0.0;
                        v197[4].x = 0.0;
                        v197[7].x = inner1m;
                        v197[6].x = inner1m;
                        v197[4].y = (float)(v201 + inner1->color1);
                        v197[5].y = inner1->color1 + v197[5].y;
                    }
                    else
                    {
                        v197[7].x = -100.0f;
                        v197[6].x = -100.0f;
                        v197[5].x = -100.0f;
                        v197[4].x = -100.0f;
                        v197[7].y = -100.0f;
                        v197[6].y = -100.0f;
                        v197[5].y = -100.0f;
                        v197[4].y = -100.0f;
                    }
                }
                else
                {
                    v197[3].x = -100.0f;
                    v197[2].x = -100.0f;
                    v197[1].x = -100.0f;
                    v197->x = -100.0f;
                    v197[7].x = -100.0f;
                    v197[6].x = -100.0f;
                    v197[5].x = -100.0f;
                    v197[4].x = -100.0f;
                    v197[3].y = -100.0f;
                    v197[2].y = -100.0f;
                    v197[1].y = -100.0f;
                    v197->y = -100.0f;
                    v197[7].y = -100.0f;
                    v197[6].y = -100.0f;
                    v197[5].y = -100.0f;
                    v197[4].y = -100.0f;
                }
            }
        }
    LABEL_333:
        v239 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v239;
        if (v239 < inner1->f1)
            LABEL_43:
        result = 0;
        else
            result = 1;
        return result;
    case 17u:
        v240 = GFX_gpst_Geo;
        v241 = GFX_gpst_Geo->vertices;
        v242 = &v241[v8];
        v243 = &v241[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        v244 = (double)(uint16_t)a1->height;
        v354 = (float)((1.0 - inner1->timeElapsed) * v244);
        inner1n = (float)((1.0 - inner1->f1) * v244);
        if (v243 >= v242)
            goto LABEL_43;
        while (1)
        {
            if (v243[1].y > (double)inner1n || v243->y < (double)v354)
            {
                v243[3].x = -100.0f;
                v243[2].x = -100.0f;
                v243[1].x = -100.0f;
                v243->x = -100.0f;
                v243[3].y = -100.0f;
                v243[2].y = -100.0f;
                v243[1].y = -100.0f;
                v243->y = -100.0f;
            }
            else if (v243->y > (double)inner1n || v243[1].y < (double)v354)
            {
                if (v243->y <= (double)inner1n)
                    v245 = v243->y;
                else
                    v245 = inner1n;
                if (v243[1].y >= (double)v354)
                    v246 = v243[1].y;
                else
                    v246 = v354;
                v247 = v243[1].y - v243->y;
                v248 = &v240->UVs[v243 - v240->vertices];
                v347 = v248[1].v - v248->v;
                v249 = (v245 - v243->y) / v247 * v347 + v248->v;
                v348 = (float)((v246 - v243->y) / v247 * v347 + v248->v);
                v243[3].y = (float)v245;
                v243->y = (float)v245;
                v248[3].v = (float)v249;
                v248->v = (float)v249;
                v243[2].y = (float)v246;
                v243[1].y = (float)v246;
                v248[2].v = v348;
                v248[1].v = v348;
            }
            v243 += 4;
            if (v243 >= v242)
                break;
            v240 = GFX_gpst_Geo;
        }
        return 0;
    case 18u:
        v250 = GFX_gpst_Geo->vertices;
        v251 = &v250[v8];
        v252 = &v250[4 * specularColorPtr[(16 * (v344 + v356)) >> 4]];
        if (v252 >= v251)
            goto LABEL_43;
        do
        {
            v253 = v252->x;
            v252[0].x = (float)(v253 + inner1->f1);
            v252[3].x = v252[3].x + inner1->f1;
            v252 += 4;
        } while (v252 < v251);
        return 0;
    case 19u:
        inner1Type20 = (tGEOSTREffectStructType20*)inner1;
        v254 = inner1Type20->color2 - TIM_gf_dt;
        inner1Type20->color2 = (float)v254;
        if (v254 >= 0.0)
            goto LABEL_43;
        v255 = inner1Type20->unkb1;
        v256 = inner1Type20->unki1;
        inner1Type20->color2 = inner1->color1;
        v257 = (v256 >> (4 * v255)) & 0xF;
        if (v257)
        {
            inner1Type20->unkb1 = v255 + 1;
        }
        else
        {
            inner1Type20->unkb1 = 1;
            v257 = v256 & 0xF;
        }
        if (!inner1Type20->flagsAndChar)
            inner1Type20->flagsAndChar = v6->flagsAndChar;
        if (v257 == 1)
            inner1Type20->color2 = (float)(inner1Type20->color2 * 0.5);
        v6->flagsAndChar &= 0xffffff00;
        v6->flagsAndChar |= (char)inner1Type20->flagsAndChar + v257 - 1;
        return 0;
        return 0;
    case 20u:
        v329 = 0;
        v305 = inner1->timeElapsed / inner1->f1;
        v318 = (float)((double)(uint16_t)a1->width * inner1->color1);
        if (v330 > 0)
        {
            v358 = v305 + v305;
            do
            {
                v258 = &GFX_gpst_Geo->vertices[4 * a1->specularColorsPtr[v344 + v329 + v306->index]];
                inner1q = (float)((v258[2].x - v258->x) * 0.2);
                v259 = (v258->x + v258[2].x) * 0.5;
                v260 = (v258->x - v259) * v318 * v305;
                v261 = v260 + v260 + v258->x;
                v262 = v258->x + inner1q;
                v293 = (float)v262;
                v263 = v293 + inner1q;
                v355 = (float)((v263 - v259) * v318 * v305 * v305 + v263);
                v264 = v263 + inner1q;
                v349 = (float)((v264 - v259) * v318 * v305 * v305 + v264);
                v265 = v264 + inner1q;
                v343 = (float)((v265 - v259) * v318 * v305 + v265);
                v266 = v265 + inner1q;
                v267 = (v266 - v259) * v318 * v305;
                inner1r = (float)(v267 + v267 + v266);
                v258[9].x = v355;
                v258[8].x = v355;
                v258[1].x = (float)v261;
                v258[7].x = v355;
                v268 = (v262 - v259) * v318 * v305 + v293;
                v258[6].x = v355;
                v258->x = (float)v261;
                v258[13].x = v349;
                v258[5].x = (float)v268;
                v258[12].x = v349;
                v258[4].x = (float)v268;
                v258[11].x = v349;
                v258[3].x = (float)v268;
                v258[10].x = v349;
                v258[2].x = (float)v268;
                v258[17].x = v343;
                v258[16].x = v343;
                v258[15].x = v343;
                v258[14].x = v343;
                v258[19].x = inner1r;
                v258[18].x = inner1r;
                v269 = &GFX_gpst_Geo->vertexColors[4 * a1->specularColorsPtr[v344 + v329 + v306->index] + 1];
                v359 = v305 * v305;
                v270 = LIGHT_ul_Interpol2Colors(*v269, 0, v359);
                v271 = LIGHT_ul_Interpol2Colors(*v269, 0, v305);
                if (v358 < 1.0)
                    v272 = LIGHT_ul_Interpol2Colors(*v269, 0, v358);
                else
                    v272 = 0;
                v269[1] = v272;
                *v269 = v272;
                v269[19] = v272;
                v269[18] = v272;
                v269[5] = v271;
                v269[4] = v271;
                v269[3] = v271;
                v269[2] = v271;
                v269[17] = v271;
                v269[16] = v271;
                v269[15] = v271;
                v269[14] = v271;
                v269[9] = v270;
                v269[8] = v270;
                v269[7] = v270;
                v269[6] = v270;
                v269[13] = v270;
                v269[12] = v270;
                v269[11] = v270;
                v269[10] = v270;
                v273 = &GFX_gpst_Geo->UVs[4 * a1->specularColorsPtr[v344 + v329++ + v306->index]];
                v274 = (v273[2].u - v273->u) * 0.2;
                v275 = v274 + v273->u;
                v273[5].u = (float)v275;
                v273[4].u = (float)v275;
                v273[3].u = (float)v275;
                v273[2].u = (float)v275;
                v276 = v274 + v274 + v273->u;
                v273[9].u = (float)v276;
                v273[8].u = (float)v276;
                v273[7].u = (float)v276;
                v273[6].u = (float)v276;
                v277 = 3.0 * v274 + v273->u;
                v273[13].u = (float)v277;
                v273[12].u = (float)v277;
                v273[11].u = (float)v277;
                v273[10].u = (float)v277;
                v278 = v274 * 4.0 + v273->u;
                v273[17].u = (float)v278;
                v273[16].u = (float)v278;
                v273[15].u = (float)v278;
                v273[14].u = (float)v278;
            } while (v329 < v330);
        }
        v279 = TIM_gf_dt + inner1->timeElapsed;
        inner1->timeElapsed = (float)v279;
        if (v279 >= inner1->f1)
            goto LABEL_26;
        goto LABEL_43;
    default:
        goto LABEL_43;
    }
}

void STR_Effect_TreatAll(tGEOSTRStruct* str)
{
    int i;

    for (i = 0; i < str->effectsNum; ) {
        if ((STR_l_Effect_Treat(str, &str->effect[i]) & 0x1) != 0)
        {
            if (i < (str->effectsNum - 1))
            {
                memcpy(&str->effect[i], &str->effect[str->effectsNum - 1], sizeof(tGEOSTREffectStruct));
                str->effectsNum--;
                continue;
            }
            str->effectsNum--;
        }
        else
        {
            i++;
        }
    }
}

int STR_l_Effect_PreTreat(tGEOSTRStruct* str, tGEOSTREffectStruct* effect)
{
    tGEOSTRStringData* sd;
    int currIdx, num;

    switch (effect->flags & 0xff) {
    case 16:
        sd = STR_pst_GetString(str, effect->stringid);
        if (!sd || (sd->flags & 0x400) != 0)
            return 0;
        
        currIdx = STR_l_GetValidIndex(str, sd, effect->index);
        num = STR_l_GetValidNumber(str, sd, currIdx, effect->number);
        for (; currIdx < num; currIdx++)
        {
            str->specularColorsPtr[currIdx + sd->index] = 1;
        }
        return num;
        break;
    case 20:
        sd = STR_pst_GetString(str, effect->stringid);
        if (!sd || (sd->flags & 0x400) != 0)
            return 0;
             
        currIdx = STR_l_GetValidIndex(str, sd, effect->index);
        num = STR_l_GetValidNumber(str, sd, currIdx, effect->number);                
        for (; currIdx < num; currIdx++)
        {
            str->specularColorsPtr[currIdx + sd->index] = 4;
        }
        return 4 * num;
        break;
    default:
        return 0;
    }   
}

int STR_l_Effect_PreTreatAll(tGEOSTRStruct* a1)
{
    int v1;
    int v2;
    tGEOSTREffectStruct* v3;

    memset(a1->specularColorsPtr, 0, 4 * a1->unkNum);
    v1 = 0;
    v2 = 0;
    if (a1->effectsNum)
    {
        v3 = a1->effect;
        do
        {
            v1 += STR_l_Effect_PreTreat(a1, v3);
            ++v2;
            ++v3;
        } while (v2 < a1->effectsNum);
    }
    return v1;
}

void STR_AddFrame(int a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, __int16 a8, __int16 a9, float a10, int a11, tFontGlypthElem* a12, int a13)
{
    int v13;
    Vec3* v14;
    double v15;
    double v16;
    double v17;
    double v18;
    double v19;
    double v20;
    double v21;
    double v22;
    double v23;
    tGeoUVElem* v24;
    double v25;
    double v26;
    double v27;
    double v28;
    double v29;
    double v30;
    double v31;
    double v32;
    float v33; 
    double v34;
    double v35;
    double v36;
    tGeoTriangleChunks* v37;
    tGeoTriangleChunkElem* v38;
    int v39;
    float v40;
    float v41;
    float a2a;
    float a3a;
    float a4a;
    float a5a;
    int i;

    v13 = 4 * a1;
    v14 = &GFX_gpst_Geo->vertices[4 * a1];
    v15 = (double)a2;
    v14[1].x = (float)v15;
    v14[0].x = (float)v15;
    v16 = (double)a3;
    v14[3].y = (float)v16;
    v14[0].y = (float)v16;
    v17 = (double)a4;
    v14[3].z = a10;
    v14[3].x = (float)v17;
    v14[2].z = a10;
    v14[2].x = (float)v17;
    v14[1].z = a10;
    v18 = (double)a5;
    v14[0].z = a10;
    v14[2].y = (float)v18;
    v14[1].y = (float)v18;
    a4a = (float)v18;
    v14[5].x = (float)v15;
    v14[7].z = a10;
    v14[6].z = a10;
    v14[4].x = (float)v15;
    v14[5].z = a10;
    v14[4].z = a10;
    v14[7].y = (float)v18;
    v14[4].y = (float)v18;
    v14[7].x = (float)v17;
    v14[6].x = (float)v17;
    v19 = (double)a7;
    v14[6].y = (float)v19;
    v14[5].y = (float)v19;
    v14[11].z = a10;
    a3a = (float)v19;
    v14[10].z = a10;
    v14[9].z = a10;
    v14[9].x = (float)v15;
    v14[8].z = a10;
    v20 = v19;
    v14[8].x = (float)v15;
    v14[11].y = (float)v19;
    v14[8].y = (float)v19;
    v14[11].x = (float)v17;
    v14[10].x = (float)v17;
    v21 = (double)a9;
    v14[10].y = (float)v21;
    a5a = (float)v21;
    v14[9].y = (float)v21;
    v14[13].x = (float)v17;
    v14[12].x = (float)v17;
    v14[15].z = a10;
    v14[15].y = (float)v16;
    v14[14].z = a10;
    v14[13].z = a10;
    v14[12].y = (float)v16;
    v14[12].z = a10;
    v22 = (double)a6;
    v14[15].x = (float)v22;
    v14[14].x = (float)v22;
    a2a = (float)v22;
    v14[14].y = (float)v18;
    v14[13].y = (float)v18;
    v14[19].z = a10;
    v14[18].z = a10;
    v14[17].x = (float)v17;
    v14[17].z = a10;
    v14[16].z = a10;
    v14[16].x = (float)v17;
    v14[19].y = (float)v18;
    v14[16].y = (float)v18;
    v14[19].x = (float)v22;
    v14[18].x = (float)v22;
    v14[18].y = (float)v20;
    v14[17].y = (float)v20;
    v14[23].y = a3a;
    v14[20].y = a3a;
    v14[21].x = (float)v17;
    v14[23].x = a2a;
    v14[20].x = (float)v17;
    v14[22].x = a2a;
    v14[22].y = a5a;
    v14[21].y = a5a;
    v14[23].z = a10;
    v14[22].z = a10;
    v14[21].z = a10;
    v14[20].z = a10;
    v14[27].y = (float)v16;
    v14[24].y = (float)v16;
    v14[25].x = a2a;
    v14[24].x = a2a;
    v14[26].y = a4a;
    v23 = (double)a8;
    v14[25].y = a4a;
    v14[27].z = a10;
    v14[26].z = a10;
    v14[27].x = (float)v23;
    v14[25].z = a10;
    v14[26].x = (float)v23;
    v14[24].z = a10;
    v14[29].x = a2a;
    v14[31].x = (float)v23;
    v14[28].x = a2a;
    v14[30].x = (float)v23;
    v14[31].y = a4a;
    v14[28].y = a4a;
    v14[30].y = a3a;
    v14[29].y = a3a;
    v14[31].z = a10;
    v14[30].z = a10;
    v14[29].z = a10;
    v14[28].z = a10;
    v14[35].x = (float)v23;
    v14[34].x = (float)v23;
    v14[33].x = a2a;
    v14[32].x = a2a;
    v14[35].y = a3a;
    v14[32].y = a3a;
    v14[34].y = a5a;
    v14[33].y = a5a;
    v14[35].z = a10;
    v14[34].z = a10;
    v14[33].z = a10;
    v14[32].z = a10;
    for (i = 0; i < 36; i++) {
        GFX_gpst_Geo->vertexColors[4 * a1 + 1 + i] = a11; //    memset32(&GFX_gpst_Geo->vertexColors[4 * a1 + 1], a11, 36u);
    }
    v24 = &GFX_gpst_Geo->UVs[4 * a1];
    v25 = a12->x;
    v26 = (a12->y + v25) * 0.5;
    v27 = (a12->h + a12->w) * 0.5;
    v28 = a12->h;
    v24[1].u = a12->x;
    v24[0].u = (float)v25;
    v24[3].u = (float)v26;
    v24[2].u = (float)v26;
    v24[2].v = (float)v27;
    v24[1].v = (float)v27;
    v24[3].v = (float)v28;
    v24[0].v = (float)v28;
    v24[5].u = (float)v25;
    v24[4].u = (float)v25;
    v24[7].u = (float)v26;
    v24[6].u = (float)v26;
    v24[6].v = (float)v27;
    v24[5].v = (float)v27;
    v24[7].v = (float)v27;
    v24[4].v = (float)v27;
    v29 = a12->w;
    v24[9].u = (float)v25;
    v24[8].u = (float)v25;
    v24[11].u = (float)v26;
    v24[10].u = (float)v26;
    v24[10].v = (float)v29;
    v24[9].v = (float)v29;
    v30 = v26;
    v31 = v27;
    v24[11].v = (float)v27;
    v24[8].v = (float)v27;
    v40 = (float)v30;
    v32 = a12->h;
    v24[13].u = (float)v30;
    v24[12].u = (float)v30;
    v24[15].u = (float)v30;
    v24[14].u = (float)v30;
    v24[14].v = (float)v31;
    v24[13].v = (float)v31;
    v24[15].v = (float)v32;
    v24[12].v = (float)v32;
    v41 = (float)v31;
    v24[17].u = (float)v30;
    v24[16].u = (float)v30;
    v24[19].u = (float)v30;
    v24[18].u = (float)v30;
    v24[18].v = (float)v31;
    v24[17].v = (float)v31;
    v24[19].v = (float)v31;
    v24[16].v = (float)v31;
    v33 = a12->w;
    v24[22].v = v33;
    v24[21].u = (float)v30;
    v24[21].v = v33;
    v24[20].u = (float)v30;
    v24[23].u = (float)v30;
    v24[22].u = (float)v30;
    v24[23].v = v41;
    v24[20].v = v41;
    v34 = a12->y;
    v35 = a12->h;
    v24[25].u = v40;
    v24[24].u = v40;
    v24[27].u = (float)v34;
    v24[26].u = (float)v34;
    v24[26].v = v41;
    v24[25].v = v41;
    v24[27].v = (float)v35;
    v24[24].v = (float)v35;
    v24[29].u = v40;
    v24[31].u = (float)v34;
    v24[28].u = v40;
    v24[30].u = (float)v34;
    v24[30].v = v41;
    v24[29].v = v41;
    v24[31].v = v41;
    v24[28].v = v41;
    v36 = a12->w;
    v24[33].u = v40;
    v24[35].u = (float)v34;
    v24[32].u = v40;
    v24[34].u = (float)v34;
    v24[35].v = v41;
    v24[32].v = v41;
    v24[34].v = (float)v36;
    v24[33].v = (float)v36;
    v37 = &GFX_gpst_Geo->triangleChunks[a13];
    v38 = &v37->triangleChunkElems[v37->triangleNum];
    v37->triangleNum += 18;
    v39 = 9;
    do
    {
        v38->v2 = v13 + 1;
        v38->uv2 = v13 + 1;
        v38->v1 = v13;
        v38->uv1 = v13;
        v38[1].v1 = v13;
        v38[1].uv1 = v13;
        v38->v3 = v13 + 2;
        v38->uv3 = v13 + 2;
        v38[1].v2 = v13 + 2;
        v38[1].v3 = v13 + 3;
        v38[1].uv2 = v13 + 2;
        v38[1].uv3 = v13 + 3;
        v13 += 4;
        v38 += 2;
        --v39;
    } while (v39);
}

void STR_AddSprite(int a1, __int16 a2, __int16 a3, __int16 a4, __int16 a5, float a6, int a7, tFontGlypthElem* a8, int a9)
{
    Vec3* v9;
    double v10;
    double v11;
    double v12;
    double v13;
    int* v14;
    tGeoUVElem* v15;
    double v16;
    double v17;
    double v18;
    double v19;
    tGeoTriangleChunks* v20;
    tGeoTriangleChunkElem* v21;

    v9 = &GFX_gpst_Geo->vertices[4 * a1];
    v10 = (double)a2;
    v9[1].x = (float)v10;
    v9->x = (float)v10;
    v11 = (double)a3;
    v9[3].y = (float)v11;
    v9->y = (float)v11;
    v12 = (double)a4;
    v9[3].z = a6;
    v9[3].x = (float)v12;
    v9[2].z = a6;
    v9[2].x = (float)v12;
    v9[1].z = a6;
    v9->z = a6;
    v13 = (double)a5;
    v9[2].y = (float)v13;
    v9[1].y = (float)v13;
    v14 = &GFX_gpst_Geo->vertexColors[4 * a1 + 1];
    *v14 = a7;
    v14[1] = a7;
    v14[2] = a7;
    v14[3] = a7;
    v15 = &GFX_gpst_Geo->UVs[4 * a1];
    v16 = a8->x;
    v15[1].u = a8->x;
    v15->u = (float)v16;
    v17 = a8->y;
    v15[3].u = a8->y;
    v15[2].u = (float)v17;
    v18 = a8->w;
    v15[2].v = a8->w;
    v15[1].v = (float)v18;
    v19 = a8->h;
    v15[3].v = a8->h;
    v15->v = (float)v19;
    v20 = &GFX_gpst_Geo->triangleChunks[a9];
    v21 = &v20->triangleChunkElems[v20->triangleNum];
    v20->triangleNum += 2;
    v21->v2 = 4 * a1 + 1;
    v21->uv2 = 4 * a1 + 1;
    v21->v1 = 4 * a1;
    v21->v3 = 4 * a1 + 2;
    v21->uv1 = 4 * a1;
    v21->uv3 = 4 * a1 + 2;
    v21[1].v1 = 4 * a1;
    v21[1].v2 = 4 * a1 + 2;
    v21[1].v3 = 4 * a1 + 3;
    v21[1].uv1 = 4 * a1;
    v21[1].uv3 = 4 * a1 + 3;
    v21[1].uv2 = 4 * a1 + 2;
}

void STR_ComputeFrame(tGEOSTRStruct* str, int idx, tGEOSTRStringData* stringData)
{
    Vec3 min, max;     

    *(int32_t *)&(min.x) = -1; // ez azért kell, hogy lásd ezt: ***** lejjebb

    if ((stringData->flags & 0x20) != 0)
    {
        STR_GetRectShort(str, idx, 0, -1, &min, &max);
        stringData->framePosX1 = (int)min.x;
        stringData->framePosY1 = (int)min.y;
        stringData->framePosX2 = (int)max.x;
        stringData->framePosY2 = (int)max.y;
    }

    if ((stringData->flags & 0x100) != 0)
    {
        if ((stringData->flags & 0x3) == 0x1)
        {
            STR_w_XFramePivot += (stringData->framePosX1 + stringData->framePosX2) / -2;
        }
        else if ((stringData->flags & 0x3) == 0x2)
        {
            STR_w_XFramePivot += stringData->framePosX1 - stringData->framePosX2;
        }
 
        if ((stringData->flags & 0xc) == 0x4)
        {
            STR_w_YFramePivot += (stringData->framePosY1 + stringData->framePosY2) / -2;
        }
        else if ((stringData->flags & 0xc) == 0x8)
        {
            STR_w_YFramePivot += stringData->framePosY1 - stringData->framePosY2;
        }
    }

    if ((stringData->flags & 0xf) != 0)
    {
        if ((*(int32_t *)&(min.x)) == -1) // itt ellenõriz rá *****
            STR_GetRectShort(str, idx, 0, -1, &min, &max);

        if ((stringData->flags & 0x3) == 0x1)
        {
            STR_w_XPivot += (int16_t)(unsigned int)(max.x + min.x) / -2;
        }
        else if ((stringData->flags & 0x3) == 0x2)
        {
            STR_w_XPivot -= (int16_t)(max.x - min.x);
        }

        if ((stringData->flags & 0xc) == 0x4)
        {
            STR_w_YPivot += (int16_t)(unsigned int)(max.y + min.y) / -2;
        }
        else if ((stringData->flags & 0xc) == 0x8)
        {
            STR_w_YPivot -= (int16_t)(max.y - min.y);
        }
    }
}

void STR_Render(tGameObjectMain* a1)
{
    tGameObjectMain* v1;
    tGameObjectVisualObject* v2;
    tGEOSTRStruct* str;
    int v5;
    tGEOMAT* v6;
    tGameObjectExtended* v7;
    int v8;
    tGEOSTRStringData* inner2;
    tGEOSTRStringData* v10;
    tGEOSTRLetter* v11;
    tGEOSTRLetter* v12;
    tGEOSTRLetter* v13;
    int v14;
    int v15;
    int v16;
    int v17;
    signed int v18;
    unsigned int* v19;
    __int16 v20;
    tFontData* v21;
    int v22;
    int v23;
    signed int v24;
    int v25;
    int v26;
    tGEOSTRStringData* iinner2;
    tGEOSTRStringData* v28;
    int verticesNum;
    tGEOSTRLetter* inner3;
    tFontData* v31;
    unsigned int v32;
    unsigned int v33;
    tFontGlypthElem* v34;
    __int16 v35;
    __int16 v36;
    __int16 v37;
    int v38;
    tFontData* v39;
    unsigned int v40;
    unsigned int v41;
    unsigned int v42;
    tFontGlypthElem* v43;
    __int16 v44;
    int v45;
    __int16 v46;
    __int16 v47;
    int v48;
    int* v49;
    int v50;
    tGameObjectExtended* v51;
    __int16 v52;
    int a8;
    tFontGlypthElem* a8a;
    __int16 a7;
    int a7a;
    int idx;
    int idxa;
    int v59;
    int v60;
    int v61;
    tGEOSTRStringData* v62;
    tGEOSTRStringData* v63;
    unsigned int v64;
    tGEOSTRLetter* v65;
    int a4;
    tGEOSTRStringData* i;
    tGEOSTRStringData* v68;
    tGEOSTRStruct* v69;
    __int16 a3;
    tUnkGeoStrStruct v71;

    v1 = a1;
    v2 = a1->basic->actorObjectOrVisualObject;
    str = (tGEOSTRStruct*)(v2->GROGEOOrSomethingElse);
    v69 = (tGEOSTRStruct*)(v2->GROGEOOrSomethingElse);
    if (((tGEOSTRStruct*)(v2->GROGEOOrSomethingElse))->renderCnt != gpst_DX_stDD->rasters[4].unk1)
    {
        str->renderCnt = gpst_DX_stDD->rasters[4].unk1;
        // the text is laid out across the width the scenes were laid
        // out for; where that sits in the window is added to each
        // letter as it is placed
        str->width = (unsigned int)D3D_FlatLayerWidth(
            (int)gpst_DX_stDD->ct.cam.width, (int)gpst_DX_stDD->ct.cam.height);
        str->height = (unsigned int)gpst_DX_stDD->ct.cam.height;
        v5 = gpst_DX_stDD->flags;
        if ((v5 & 0x800) == 0)
            gpst_DX_stDD->flags = v5 | 0x200;
        v6 = (tGEOMAT*)(((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT);
        if (v6)
        {
            v7 = a1->extended;
            if (v7 && v7->AIInstance)
            {
                AI_C_Callback = 1;
                AI_ExecCallback(a1, 0);
                AI_C_Callback = 0;
            }
            STR_SetDisplay();
            str->fontMaterialID = -1;
            str->fontDesc = 0;
            ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT = ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            if (v6->intf->id == 4)
            {
                v64 = v6->multiMaterialNumIfType4ThenAmbiantColor;
                if ((int)v64 > 64)
                    v64 = 64;
            }
            else
            {
                v64 = 1;
            }
            memset(&v71, 0, 96u); // FIXME ez milyen méret? Miért csak egy részét nullázza ki?
            str->specularColorsPtr = (int*)gpst_DX_stDD->bigBuffer->data2SpecularColors;
            v8 = STR_l_Effect_PreTreatAll(str);
            inner2 = str->stringData;
            a8 = v8;
            v10 = &inner2[str->stringNum];
            v62 = inner2;
            for (i = v10; inner2 < v10; v62 = inner2)
            {
                if ((inner2->flags & 0x400) == 0)
                {
                    v11 = str->letters;
                    v12 = &v11[inner2->index + inner2->index2];
                    v13 = &v12[inner2->number2];
                    if (v13 > v12)
                    {
                        if ((inner2->flags & 0x100) != 0)
                        {
                            v14 = ((inner2->frame >> 12) & 0x3F) % v64;
                            v71.triangleNum[v14] += 9;
                            a8 += 9;
                            v10 = i;
                        }
                        if (v12 < v13)
                        {
                            v15 = (char*)v12 - (char*)v11;
                            a8 += ((unsigned int)((char*)v13 - (char*)v12 - 1) >> 4) + 1;
                            do
                            {
                                v16 = ((v12->flagsAndChar >> 12) & 0x3F) % v64;
                                v17 = v15 >> 4;
                                ++v12;
                                v15 += 16;
                                v71.triangleNum[v16] += str->specularColorsPtr[v17] + 1;
                            } while (v12 < v13);
                            inner2 = v62;
                            v10 = i;
                        }
                    }
                    v1 = a1;
                }
                ++inner2;
            }
            v18 = 0;
            v19 = ul_SubMatOrder;
            do
            {
                if (v71.triangleNum[*v19])
                {
                    v20 = *(uint16_t*)v19; // FIXME ez totál felesleges, mármint a cast
                    if (*(uint16_t*)v19 != str->fontMaterialID)
                    {
                        str->fontMaterialID = v20;
                        str->fontDesc = STR_pst_GetFontDesc((tGEOMAT *)(((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROMAT), v20);
                    }
                    v21 = str->fontDesc;
                    if (v21)
                    {
                        v22 = *v19;
                        v23 = *v19;
                        v71.triangleChunkID[v23] = v18;
                        v71.materialID[v18++] = v22;
                        v71.fontData[v23] = v21;
                    }
                    else
                    {
                        a8 -= v71.triangleNum[*v19];
                        v71.fontData[*v19] = 0;
                    }
                }
                else
                {
                    v71.fontData[*v19] = 0;
                }
                ++v19;
            } while (v19 <= &ul_SubMatOrder[23]);
            idx = v18;
            if (a8)
            {
                GFX_NeedGeomEx(4 * a8, 4 * a8, v18, 1u, 1);
                v24 = 0;
                if (v18 > 0)
                {
                    v25 = 0;
                    do
                    {
                        v26 = v71.materialID[v24];
                        GFX_NeedGeomExElem(v24, 2 * v71.triangleNum[v26]);
                        GFX_gpst_Geo->triangleChunks[v25].triangleNum = 0;
                        GFX_gpst_Geo->triangleChunks[v25].materialNum = v26;
                        ++v24;
                        ++v25;
                    } while (v24 < idx);
                }
                iinner2 = str->stringData;
                v28 = &iinner2[str->stringNum];
                verticesNum = 0;
                v63 = iinner2;
                v68 = v28;
                idxa = 0;
                if (iinner2 < v28)
                {
                    while ((iinner2->flags & 0x400) != 0)
                    {
                    LABEL_64:
                        v63 = ++iinner2;
                        ++idxa;
                        if (iinner2 >= v28)
                            goto LABEL_65;
                    }
                    STR_w_XPivot = (int)((double)(uint16_t)str->width * iinner2->pivotPoint.x);
                    STR_w_XFramePivot = STR_w_XPivot;
                    STR_w_YPivot = (int)((double)(uint16_t)str->height * iinner2->pivotPoint.y);
                    STR_w_YFramePivot = STR_w_YPivot;
                    inner3 = &str->letters[iinner2->index2 + iinner2->index];
                    v65 = &inner3[iinner2->number2];
                    if (v65 > inner3)
                    {
                        STR_ComputeFrame(str, idxa, iinner2);
                        if ((iinner2->flags & 0x100) == 0)
                        {
                        LABEL_45:
                            if (iinner2->index2)
                            {
                                STR_w_XPivot += str->letters[iinner2->index].x - inner3->x;
                                STR_w_YPivot += str->letters[iinner2->index].y - inner3->y;
                            }
                            for (; inner3 < v65; ++inner3)
                            {
                                v38 = ((inner3->flagsAndChar >> 12) & 0x3F) % v64;
                                v39 = v71.fontData[v38];
                                v60 = v38;
                                str->fontDesc = v39;
                                str->fontMaterialID = v38;
                                if (v39)
                                {
                                    v40 = inner3->flagsAndChar;
                                    v41 = (uint8_t)inner3->flagsAndChar;
                                    
                                    if (v41 < 32)
                                        v42 = 0;
                                    else
                                        v42 = v41 - 32;

                                    v43 = &v39->fge[v42];
                                    a8a = v43;
                                    a7 = inner3->y;
                                    if ((v40 & 0x1000000) != 0)
                                        v44 = (__int16)inner3->width >> 1;
                                    else
                                        v44 = (v40 & 0x4000000) != 0 ? (unsigned __int16)inner3->width : 0;
                                    STR_w_LetterDX = v44;
                                    if ((inner3->flagsAndChar & 0x2000000) != 0)
                                        v45 = (__int16)inner3->height >> 1;
                                    else
                                        v45 = (inner3->flagsAndChar & 0x8000000) != 0 ? (unsigned int)inner3->height : 0;
                                    STR_w_LetterDY = v45;
                                    v46 = str->height + v45 - STR_w_YPivot - a7;
                                    v47 = v46 - (unsigned __int16)inner3->height;
                                    a3 = v46;
                                    v48 = inner3->x + (unsigned __int16)(STR_w_XPivot - v44);
                                    a7a = v71.triangleChunkID[v60];
                                    v52 = inner3->x + STR_w_XPivot - v44;
                                    a4 = inner3->width + v48;
                                    STR_AddSprite(verticesNum, v52, v46, v48 + (unsigned __int16)inner3->width, v47, 0.0, inner3->textColor, a8a, a7a);
                                    v49 = &v69->specularColorsPtr[inner3 - v69->letters];
                                    v50 = *v49;
                                    *v49 = verticesNum++;
                                    if (v50)
                                    {
                                        v61 = v50;
                                        do
                                        {
                                            STR_AddSprite(verticesNum++, v48, a3, a4, v47, 0.0, inner3->textColor, a8a, a7a);
                                            --v61;
                                        } while (v61);
                                    }
                                    iinner2 = v63;
                                    str = v69;
                                }
                            }
                            goto LABEL_63;
                        }
                        v31 = v71.fontData[((iinner2->frame >> 12) & 0x3F) % v64];
                        v59 = ((iinner2->frame >> 12) & 0x3F) % v64;
                        str->fontDesc = v31;
                        if (v31)
                        {
                            v32 = (uint8_t)iinner2->frame;
                            if (v32 < 0x20)
                                v33 = 0;
                            else
                                v33 = v32 - 32;
                            v34 = &v31->fge[v33];
                            v35 = iinner2->framePosX1;
                            iinner2->i2 = verticesNum;
                            v36 = str->height;
                            v37 = STR_w_XFramePivot + iinner2->framePosX2;
                            a4 = v37;
                            STR_AddFrame(
                                verticesNum,
                                STR_w_XFramePivot + v35 - iinner2->frameBorderSize1,
                                v36 - iinner2->framePosY1 - STR_w_YFramePivot + iinner2->frameBorderSize2,
                                STR_w_XFramePivot + v35,
                                v36 - iinner2->framePosY1 - STR_w_YFramePivot,
                                v37,
                                v36 - iinner2->framePosY2 - STR_w_YFramePivot,
                                v37 + iinner2->frameBorderSize1,
                                v36 - iinner2->framePosY2 - STR_w_YFramePivot - iinner2->frameBorderSize2,
                                0.0,
                                iinner2->frameColor,
                                v34,
                                v71.triangleChunkID[v59]);
                            str = v69;
                            verticesNum += 9;
                            goto LABEL_45;
                        }
                    }
                LABEL_63:
                    v28 = v68;
                    goto LABEL_64;
                }
            LABEL_65:
                STR_Effect_TreatAll(str);
                ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = (tGroGeneric*)GFX_gpst_Geo;
                GFX_gpst_Geo->intf->Render(STR_sgpst_GO);
                v1 = a1;
            }
            v51 = v1->extended;
            if (v51)
            {
                if (v51->AIInstance)
                    AI_ExecCallback(v1, 1);
            }
            gpst_DX_stDD->flags = sul_SaveDisplayInfo;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent = sul_SaveDrawMask;
            gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
        }
    }
}

void STR_Init()
{
    int i;
    tStrGlobalStruct* v1;

    for (i = 0; i < 256; ++i)
        STR_sgac_CharTable[i] = i;
    STR_sgac_CharTable[161] = 168;
    STR_sgac_CharTable[224] = 133;
    STR_sgac_CharTable[225] = 160;
    STR_sgac_CharTable[226] = 131;
    STR_sgac_CharTable[228] = 132;
    STR_sgac_CharTable[231] = 135;
    STR_sgac_CharTable[232] = 138;
    STR_sgac_CharTable[233] = 130;
    STR_sgac_CharTable[234] = 136;
    STR_sgac_CharTable[235] = 137;
    STR_sgac_CharTable[236] = 141;
    STR_sgac_CharTable[237] = 161;
    STR_sgac_CharTable[238] = 140;
    STR_sgac_CharTable[239] = 139;
    STR_sgac_CharTable[241] = 164;
    STR_sgac_CharTable[242] = 149;
    STR_sgac_CharTable[243] = 162;
    STR_sgac_CharTable[244] = 147;
    STR_sgac_CharTable[245] = 167;
    STR_sgac_CharTable[246] = 148;
    STR_sgac_CharTable[249] = 151;
    STR_sgac_CharTable[250] = 163;
    STR_sgac_CharTable[251] = 150;
    STR_sgac_CharTable[252] = 129;
    GRO_gast_Interface[10].CreateFromBuffer = STR_p_CreateFromBuffer;
    GRO_gast_Interface[10].Destroy = STR_Free;
    GRO_gast_Interface[10].HasSomethingToRender = STR_l_HasSomethingToRender;
    GRO_gast_Interface[10].Render = STR_Render;
    GRO_gast_Interface[10].Reinit = STR_Reinit;
    STR_sgpst_GO = OBJ_GameObject_Create(0x5000);
    ((tGameObjectVisualObject*)(STR_sgpst_GO->basic->actorObjectOrVisualObject))->objDrawMask &= ~0x10800u;
    STR_sgpst_Font = 0;
    v1 = STR_sgaac_Global;
    do                                            // ezt átnézni
    {
        v1->str[0] = 0;
        ++v1;
    } while (v1 < &(STR_sgaac_Global[23]));
    STR_sgl_MaxNumberOfFont = 30;
    STR_sgl_NumberOfFont = 0;
    STR_sgpst_Font = (tFontData*)MEM_p_Alloc(sizeof(tFontData) * 30);
}
