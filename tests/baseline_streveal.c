/* Frozen pre-refactor copy of the string reveal effect, for the
   differential test only. Taken verbatim from the commit that isolated
   it, with one change: the function is not static, so the test can call
   it. Nothing else differs. */

#include "../graphicsobjects/geostr.h"
#include "../graphicsobjects/geo.h"
#include "../time.h"


// 16: the letter-by-letter reveal. Still the decompiler's own output:
// 1,090 lines, seventeen gotos and twelve labels. The names the
// prologue used are re-established from the run so the body is
// untouched while the rest of the file moves on; it is the next pass.
int BASE_STR_l_Effect_Reveal(tStrEffectRun* run)
{
    tGEOSTRStruct* a1 = run->str;
    tGEOSTREffectStruct* inner1 = run->effect;
    tGEOSTRStringData* v306 = run->sd;
    tGEOSTRLetter* v6 = run->letters;
    tGEOSTRLetter* v7 = run->lettersEnd;
    tGEOSTRLetter* v307 = run->letters;
    int* specularColorPtr = run->spec;
    int v8 = run->endVertex;
    int v9 = run->firstVertex;
    int v350 = run->frameVertices;
    int v345 = run->frameFirst;
    int v344 = run->startIdx;
    int v356 = run->sd->index;
    int v330 = run->count;

    int result;
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
    float v291; 
    float v292; 
    int v313;
    int v314;
    int v315;
    int v316;
    int v317;
    float v326;
    float v327;
    int v328;
    float v337;
    int v338; 
    int v339; 
    int v340; 
    int v341; 
    int v342; 
    int v346;
    int kk;
    int v353;
    float inner1k;
    float inner1l;
    float inner1m;

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
            return 0;
        return 1;
}
