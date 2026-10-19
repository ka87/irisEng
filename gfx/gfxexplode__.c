#include "../memory.h"
#include "../time.h"
#include "gfxexplode.h"
#include "../dbg.h"

BOOL explodeRandomFactorsEnabled = TRUE;

void GFX_SetNumberOfSprites(tGfxExplodeData* expl, int numberOfSprites)
{
    int i, j;

    expl->num = (numberOfSprites + 8) & ~7;

    if (expl->particlesTimeElapsed)
        MEM_FreeAlign(expl->particlesTimeElapsed);

    expl->particlesTimeElapsed = (float*)MEM_p_AllocAligned(sizeof(int32_t) * expl->num + 32, 16);
    memset(expl->particlesTimeElapsed, 0, sizeof(int32_t) * expl->num);

    if (expl->vecPtr)
        MEM_FreeAlign(expl->vecPtr);

    expl->vecPtr = (tSpriteC0DEData*)MEM_p_AllocAligned(sizeof(tSpriteC0DEData) * (expl->num + 8), 16);
    memset(expl->vecPtr, 0, sizeof(tSpriteC0DEData) * expl->num);

    if (expl->normalPtr)
        MEM_FreeAlign(expl->normalPtr);

    expl->normalPtr = (tSpriteC0DEData*)MEM_p_AllocAligned(sizeof(tSpriteC0DEData) * (expl->num + 8), 16);
    memset(expl->normalPtr, 0, sizeof(tSpriteC0DEData) * expl->num);

    if (expl->vertrexColors)
        MEM_FreeAlign(expl->vertrexColors - 4);

    expl->vertrexColors = (int*)((char*)MEM_p_AllocAligned(sizeof(uint32_t) * (expl->num + 12), 16) + 16); // +4-gyel eloffseteli, mert belerak vmi random szemetet (darabszámot [v3])

    *(expl->vertrexColors - 1) = expl->num;
    memset(expl->vertrexColors, 0, 4 * expl->num);
    expl->num = (numberOfSprites + 8) & ~7;

    for (i = 0; i < expl->num; i++) {
        ((*(uint32_t*)&expl->particlesTimeElapsed[i])) = 1;
    }
    
    expl->ground = -9.9999998e10f;
    expl->fdf = 0.0f;
    expl->timeElapsed = 0.0f;
    expl->gao = OBJ_GameObject_Create(0x5000);
    ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = (tGroGeneric*)(&expl->geo);
    ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum = -3;
    GRO_Struct_Init((tGroGeneric*)(&expl->geo), 1);

    expl->geo.normals = &expl->normalPtr->v1;
    expl->geo.vertices = &expl->vecPtr->v1;
    expl->geo.vertexColors = expl->vertrexColors - 1;
    expl->geo.spritesPtr = expl->spr;
    expl->geo.vertexNum = expl->num;
    expl->geo.uvNum = 0;
    expl->geo.UVs = NULL;
    expl->geo.triangleChunkCount = 0;
    expl->geo.triangleChunks = NULL;
    expl->geo.bv = NULL;
    expl->geo.mrm = NULL;
    expl->geo.skinning = NULL;
    expl->geo.unkFlags = 0;
    expl->geo.spritesNum = 4;
    expl->geo.collisionPtr = NULL;

    for (i = 4, j = 6; i > 0; i--, j -= 2) {
        expl->spr[i - 1].num = (expl->num / 4);
        expl->spr[i - 1].matIdx = expl->material1ID;
        expl->spr[i - 1].spriteElems = &expl->geoSub[j];
        expl->geoSub[j].idx = 0xC0DE;
        expl->geoSub[j + 1].idx = (i - 1) * (expl->num / 4);
        expl->spr[i - 1].i1 = 0;
        expl->spr[i - 1].i2 = 0;
        expl->spr[i - 1].globalSize = 1.0f;
        expl->spr[i - 1].globalRatio = 1.0f;
    }
   
    expl->spr[0].matIdx = expl->material1ID;
    expl->spr[1].matIdx = expl->material2ID;
    expl->spr[2].matIdx = expl->material3ID;
    expl->spr[3].matIdx = expl->material4ID;
}

void GFX_Explode_Seti(struct sGfxExplodeData* expl, int type, int val)
{    
    int i;   
    float factor;

    switch (type)
    {
    case 13100:
        if (4 * val > 5000)
            val = 500;
        GFX_SetNumberOfSprites(expl, val);
        break;
    case 13101:
        expl->material1ID = val;
        expl->material2ID = val;
        expl->material3ID = val;
        expl->material4ID = val;

        for (i = 3; i >= 0; i--) {
            expl->spr[i].matIdx = expl->material1ID;
        }       
        break;
    case 13102:
        expl->material2ID = val;
        break;
    case 13103:
        expl->phase1Color = val;

        for (i = 0; i < 256; i++) {
            factor = (i * (1.0f / 256.0f));
            expl->interPolatedColors[i] = LIGHT_ul_Interpol2Colors(expl->phase1Color, expl->phase2Color, factor);
        }
        break;
    case 13104:
        expl->phase2Color = val;

        for (i = 0; i < 256; i++) {
            factor = (i * (1.0f / 256.0f));
            expl->interPolatedColors[i] = LIGHT_ul_Interpol2Colors(expl->phase1Color, expl->phase2Color, factor);
            expl->interPolatedColors[i + 256] = LIGHT_ul_Interpol2Colors(expl->phase2Color, expl->phase3Color, factor);
        }       
        break;
    case 13105:
        expl->phase3Color = val;

        for (i = 0; i < 256; i++) {
            factor = (i * (1.0f / 256.0f));
            expl->interPolatedColors[i + 256] = LIGHT_ul_Interpol2Colors(expl->phase2Color, expl->phase3Color, factor);
        }       
        break;
    case 13106:
        expl->piecesToGenerate = val;
        break;
    case 13107:
        expl->GFXDirectly = val;
        break;
    case 13110:
        expl->material1ID = val;
        expl->spr[0].matIdx = val;
        break;
    case 13111:
        expl->material2ID = val;
        expl->spr[1].matIdx = val;
        break;
    case 13112:
        expl->material3ID = val;
        expl->spr[2].matIdx = val;
        break;
    case 13113:
        expl->material4ID = val;
        expl->spr[3].matIdx = val;
        break;
    default:
        return;
    }
}

int GFX_i_Explode_Geti(struct sGfxExplodeData* expl, int type)
{
    int ret;

    switch (type)
    {
    case 13100:
        ret = expl->num;
        break;
    case 13101:
        ret = expl->material4ID;
        break;
    case 13102:
        ret = expl->material2ID;
        break;
    case 13103:
        ret = expl->phase1Color;
        break;
    case 13104:
        ret = expl->phase2Color;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

void GFX_Explode_Setf(struct sGfxExplodeData* expl, int type, float val)
{
    switch (type)
    {
    case 13000:
        expl->growSpeedMin = val;
        break;
    case 13001:
        expl->growSpeedMax = val;
        break;
    case 13002:
        expl->friction.f1 = 1.0f - val;
        break;
    case 13003:
        expl->phase1Time = val;
        expl->ooPhase1Time = 256.0f / val;
        break;
    case 13004:
        expl->phase2Time = val;
        expl->ooPhase2Time = 256.0f / val;
        break;
    case 13005:
        expl->initialSizeMin = val;
        break;
    case 13006:
        expl->initialSizeMax = val;
        break;
    case 13007:
        expl->gravity = val;
        break;
    case 13008:
        expl->generationSpeed = val;
        break;
    case 13009:
        expl->normSpeedMin = val;
        break;
    case 13010:
        expl->normSpeedMax = val;
        break;
    case 13011:
        expl->ground = val;
        break;
    case 13012:
        expl->timeRandomFactor = val;
        break;
    default:
        return;
    }
}

void GFX_Explode_Setv(struct sGfxExplodeData* expl, int type, Vec3* val)
{
    switch (type)
    {
    case 13200:
        expl->initialPos = *val;
        break;
    case 13201:
        expl->speedMin = *val;
        break;
    case 13202:
        expl->speedMax = *val;
        break;
    case 13203:
        expl->friction.v1 = *val;
        expl->friction.v1.x = 1.0f - expl->friction.v1.x;
        expl->friction.v1.y = 1.0f - expl->friction.v1.y;
        expl->friction.v1.z = 1.0f - expl->friction.v1.z;
        break;
    case 13204:
        expl->wind.v1 = *val;
        expl->wind.f1 = 0.0f;
        break;
    case 13205:
        expl->mainPosSpeed = *val;
        break;
    case 13206:
        expl->mainPosFriction = *val;
        expl->mainPosFriction.x = 1.0f - expl->mainPosFriction.x;
        expl->mainPosFriction.y = 1.0f - expl->mainPosFriction.y;
        expl->mainPosFriction.z = 1.0f - expl->mainPosFriction.z;
        break;
    case 13207:
        expl->initialPosVolumeX = *val;
        break;
    case 13208:
        expl->initialPosVolumeY = *val;
        break;
    case 13209:
        expl->initialPosVolumeZ = *val;
        break;
    default:
        return;
    }
}

void GFX_Explode_FastMeca_inner_sub_45F630(tSpriteC0DEData* a1, tSpriteC0DEData* a2, tSpriteC0DEData* a3, tGfxExplodeData* a4, int a5, float a6, float a7, float a8, float a9)
{
    int i;
    double v12;
    double v13;

    for (i = a5; i; )
    {
        a1->v1.x = (a4->wind.v1.x + a2->v1.x) * a6 + a1->v1.x;
        a1->v1.y = (a4->wind.v1.y + a2->v1.y) * a6 + a1->v1.y;
        a1->v1.z = (a4->wind.v1.z + a2->v1.z) * a6 + a1->v1.z;
        a1->f1 = (a4->wind.f1 + a2->f1) * a6 + a1->f1;
        a2->v1.x = a3->v1.x * a2->v1.x;
        a2->v1.y = a3->v1.y * a2->v1.y;
        a2->v1.z = a3->v1.z * a2->v1.z;
        a2->f1 = a2->f1 * a3->f1;
        v12 = a7 - a1->v1.z;
        if (v12 <= 0.0)
            v12 = 0.0;
        ++a2;
        v13 = v12 + a1->v1.z;
        ++a1;
        --i;
        a1[-1].v1.z = (float)v13;
        a2[-1].v1.z = (float)(v12 * a9 + a2[-1].v1.z + a8);
    }
}

void GFX_Explode_FastMeca(tGfxExplodeData* a1, tSpriteC0DEData* a2, tGfxExplodeData* a3, float a4, float a9, float a8, float a7)
{
    GFX_Explode_FastMeca_inner_sub_45F630(a1->vecPtr, a1->normalPtr, a2, a3, a1->num, a4, a7, a8, a9);
}

struct sGfxExplodeData* GFX_Explode_Create()
{
    int i;
    tGfxExplodeData* expl = (tGfxExplodeData*)MEM_p_AllocAligned(sizeof(tGfxExplodeData), 16);

    memset(expl, 0, sizeof(tGfxExplodeData));

    if (explodeRandomFactorsEnabled)
    {
        for (i = (sizeof(fTimeRandomFactors)/sizeof(fTimeRandomFactors[0])) - 1; i >= 0; i--) {
            fTimeRandomFactors[i] = MATH_RandFloat();
        }        
    }
    return expl;
}

void GFX_Explode_Destroy(struct sGfxExplodeData* expl)
{
    if (!expl) return;

    if (expl->particlesTimeElapsed)
        MEM_FreeAlign(expl->particlesTimeElapsed);
    if (expl->vecPtr)
        MEM_FreeAlign(expl->vecPtr);
    if (expl->normalPtr)
        MEM_FreeAlign(expl->normalPtr);
    if (expl->vertrexColors)
        MEM_FreeAlign(expl->vertrexColors - 4);
    
    if (expl->gao)
    {
        if (expl->gao->basic && expl->gao->basic->actorObjectOrVisualObject)
        {
            ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROMAT = NULL;
            ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = NULL;
        }
        OBJ_GameObject_Remove(expl->gao, TRUE);
    }
    expl->particlesTimeElapsed = NULL;
    expl->vecPtr = NULL;
    expl->normalPtr = NULL;
    expl->vertrexColors = NULL;
    expl->gao = NULL;
}

void GFX_SetRenderStateForExplode()
{
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = gpst_DX_stDD->drawMaskAllowedStuff & ~0x10000;
    gpst_DX_stDD->matrixStack.currElemNum = 0;
    gpst_DX_stDD->matrixStack.actMatrixElem = gpst_DX_stDD->matrixStack.elems;
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    gpst_DX_stDD->currTargetGao = GFX_gpst_GO;
    gpst_DX_stDD->extraMaterialToGeo = NULL;
}

void GFX_Explode_Generate(tGfxExplodeData* expl)
{
    int i;
    float dp;
    Vec3 tmp1, tmp2;

    MATH_RandFloat();
    expl->timeElapsed += TIM_gf_dt;

    if (expl->generationSpeed + expl->fdf > expl->timeElapsed) return;
    if (!expl->piecesToGenerate) return;

    for (i = 0; i < expl->num; i++)
    {
        if (*((uint32_t*)&expl->particlesTimeElapsed[i]) == 1)
        {
            expl->vecPtr[i].v1 = expl->initialPos;
            MATH_VecMultScalar(&tmp1, &expl->initialPosVolumeX, (2 * MATH_RandFloat() - 1.0f));
            MATH_VecMultScalar(&tmp2, &expl->initialPosVolumeY, (2 * MATH_RandFloat() - 1.0f));
            MATH_VecAddVecWithoutBuffer(&tmp1, &tmp2);
            MATH_VecMultScalar(&tmp2, &expl->initialPosVolumeZ, (2 * MATH_RandFloat() - 1.0f));
            MATH_VecAddVecWithoutBuffer(&tmp1, &tmp2);
            MATH_VecAddVecWithoutBuffer(&expl->vecPtr[i].v1, &tmp1);
            expl->vecPtr[i].f1 = MATH_RandFloatRange(expl->initialSizeMin, expl->initialSizeMax);
            expl->vertrexColors[i] = expl->phase1Color;
            expl->normalPtr[i].v1.x = MATH_RandFloatRange(expl->speedMin.x, expl->speedMax.x);
            expl->normalPtr[i].v1.y = MATH_RandFloatRange(expl->speedMin.y, expl->speedMax.y);
            expl->normalPtr[i].v1.z = MATH_RandFloatRange(expl->speedMin.z, expl->speedMax.z);
            expl->normalPtr[i].f1 = MATH_RandFloatRange(expl->growSpeedMin, expl->growSpeedMax);
            MATH_SubstractVec(&expl->normalPtr[i].v1, &expl->normalPtr[i].v1, &tmp1);
            MATH_VecAddVecWithoutBuffer(&expl->normalPtr[i].v1, &expl->mainPosSpeed);

            if ((dp = MATH_VecDotProduct(&expl->normalPtr[i].v1)) != 0.0f)
            {
                MATH_VecMultScalarWithoutBuffer(&expl->normalPtr[i].v1, MATH_RandFloatRange(expl->normSpeedMin, expl->normSpeedMax) * MATH_GetInverseSqrt(dp));
            }

            expl->particlesTimeElapsed[i] = 0.0f;
            expl->triangleNum++;
            expl->piecesToGenerate--;
            if ((unsigned int)(4 * expl->triangleNum) > 5000)
                expl->triangleNum = 500;

            if ((expl->generationSpeed + expl->fdf) <= expl->timeElapsed && expl->piecesToGenerate)
                expl->fdf = expl->generationSpeed + expl->fdf;
            else
                i = expl->num;
        }
    }
}

void GFX_Explode_Mecanic(tGfxExplodeData* expl, float deltaTime)
{
    float v3;
    float v4;
    float v5;
    float* v6;
    tSpriteC0DEData* v7;
    int* v8;
    tSpriteC0DEData* v9;
    tSpriteC0DEData* v10;
    float v11;
    int v12;
    int v13;
    float v14;
    double v15;
    float a9;
    float a8;
    float v18;
    int v19;
    tSpriteC0DEData* v20;
    double v21;
    double v22;
    float v23; 
    long double v24;
    float v25;
    float v26;
    tSpriteC0DEData v27;
    float *a1a;

    v24 = (float)(deltaTime * 59.999996);
    v3 = expl->friction.v1.y;
    v27.v1.x = expl->friction.v1.x;
    v4 = expl->friction.v1.z;
    v27.v1.y = v3;
    v5 = expl->friction.f1;
    v19 = 0;
    v27.v1.x = (float)pow(v27.v1.x, v24);
    v27.v1.y = (float)pow(v27.v1.y, v24);
    v27.v1.z = (float)pow(v4, v24);
    v27.f1 = (float)pow(v5, v24);
    v25 = expl->phase1Time;
    v18 = expl->ground;
    v23 = expl->phase2Time + v25;
    v6 = expl->particlesTimeElapsed;
    v7 = expl->vecPtr;
    a8 = deltaTime * expl->gravity;
    v8 = expl->vertrexColors;
    v20 = expl->normalPtr;
    a1a = &v6[expl->num];
    a9 = (float)(2.0 / deltaTime);
    GFX_Explode_FastMeca(expl, &v27, expl, deltaTime, a9, a8, v18);
    if (v6 < a1a)
    {
        v9 = v20;
        v10 = v7;
        do
        {
            v11 = *v6;
            if (*((uint32_t *)v6) != 1)                           // 1 has special meaning
            {
                if ((*(uint32_t*)&v11) <= (*(uint32_t *)&v23))
                {
                    if ((*(uint32_t*)&v11) <= (*(uint32_t *)&v25))
                    {
                        v22 = expl->ooPhase1Time * *v6 + MATH_gd_magicDoubleForRound;
                        v13 = (*(uint32_t *)&v22);
                        if ((*(int32_t *)&v22) > 256)
                            v13 = 256;
                        *v8 = expl->interPolatedColors[v13];
                    }
                    else
                    {
                        //f2iMagic
                        v21 = (*v6 - expl->phase1Time) * expl->ooPhase2Time + MATH_gd_magicDoubleForRound;
                        v12 = *((uint32_t *)&v21);
                        if ((*(int32_t *)&v21) > 255)
                            v12 = 255;
                        *v8 = expl->interPolatedColors[v12 + 256];
                    }
                    v14 = fTimeRandomFactors[(uint8_t)v19++] * expl->timeRandomFactor;
                    *v6 = (v14 + 1.0f) * deltaTime + *v6;
                }
                else
                {
                    *((uint32_t *)v6) = 1;
                    v10->f1 = 0.0f;
                    v9->f1 = 0.0f;
                    *v8 = 0;
                }
            }
            ++v6;
            ++v8;
            ++v10;
            ++v9;
        } while (v6 < a1a);
    }
    v15 = expl->mainPosSpeed.y * deltaTime;                          // addscalevector
    v26 = expl->mainPosSpeed.z * deltaTime;
    expl->initialPos.x = deltaTime * expl->mainPosSpeed.x + expl->initialPos.x;
    expl->initialPos.y = (float)(expl->initialPos.y + v15);
    expl->initialPos.z = expl->initialPos.z + v26;
    expl->mainPosSpeed.x = expl->mainPosFriction.x * expl->mainPosSpeed.x;
    expl->mainPosSpeed.y = expl->mainPosFriction.y * expl->mainPosSpeed.y;
    expl->mainPosSpeed.z = expl->mainPosFriction.z * expl->mainPosSpeed.z;
}

BOOL GFX_i_Explode_Render(struct sGfxExplodeData* a1, struct sGEOMAT* a2)
{
    float* v2;
    float* v3;
    int v4; 
    //unsigned int v6;
    tGameObjectMain* v7; 
    tGameObjectVisualObject** v8;
    tGameObjectVisualObject* v9;
    tGameObjectVisualObject* v10;
    Vec3* currVertices;
//    int* v12;
    int* currColors; 
    ////float v14;
    //float v15;
    //int v16; 
    float* currParticleTime;
    //tGeoTriangleChunks* v18;
    //tGeoTriangleChunkElem* v19;
    tSpriteC0DEData* v20;
    int v21;
  
    tGeoTriangleChunkElem* v32;
    tGeoTriangleChunkElem** v33;
 
    int drawMaskBackup;
    int v37; 
    int* currColor;
    //float* v39;
    float* currParticleEnd;
    tGeoTriangleChunkElem* triangleChunkBackupForElementCounting[4];
    tGeoTriangleChunkElem** v42;
   
    GFX_Explode_Generate(a1);
    v2 = a1->particlesTimeElapsed;
    v3 = &v2[a1->num];
    for (a1->triangleNum = 0; v2 < v3; ++v2)
    {
        if (*((uint32_t*)v2) != 1)
        {
            v4 = a1->triangleNum + 1;
            a1->triangleNum = v4;
            if ((unsigned int)(4 * v4) > 5000)
                a1->triangleNum = 500;
        }
    }
    if (!a1->triangleNum)
        return a1->piecesToGenerate != 0;

    if (a1->GFXDirectly) {
        p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists - 3;
    }

    if (TRUE || a1->GFXDirectly
        && ((char*)zListData->currMeshOffset - (char*)(&zListData->meshElems[0])) / sizeof(struct sZListMeshElem) + 
            2 * (a1->triangleNum + sizeof(struct sZListElem)) <= 10) //check if at least 10 elems can fit into the buffer
    {
        GFX_NeedGeomEx(4 * a1->triangleNum, 4, 4, (a1->triangleNum >> 1) + 8, 1);
        GFX_NeedGeomExElem(1, ((unsigned int)a1->triangleNum >> 1) + 8);
        GFX_NeedGeomExElem(2, ((unsigned int)a1->triangleNum >> 1) + 8);
        GFX_NeedGeomExElem(3, ((unsigned int)a1->triangleNum >> 1) + 8);
        GFX_gpst_Geo->UVs[0].u = 0.0f;
        GFX_gpst_Geo->UVs[0].v = 0.0f;
        GFX_gpst_Geo->UVs[1].u = 1.0f;
        GFX_gpst_Geo->UVs[1].v = 0.0f;
        GFX_gpst_Geo->UVs[2].u = 1.0f;
        GFX_gpst_Geo->UVs[2].v = 1.0f;
        GFX_gpst_Geo->UVs[3].u = 0.0f;
        GFX_gpst_Geo->UVs[3].v = 1.0f;
        GFX_gpst_Geo->triangleChunks[0].materialNum = a1->material1ID;
        GFX_gpst_Geo->triangleChunks[1].materialNum = a1->material2ID;
        GFX_gpst_Geo->triangleChunks[2].materialNum = a1->material3ID;
        GFX_gpst_Geo->triangleChunks[3].materialNum = a1->material4ID;
        currVertices = GFX_gpst_Geo->vertices;
        GFX_gpst_Geo->vertexColors[0] = 4 * a1->triangleNum;
        currColors = GFX_gpst_Geo->vertexColors + 1;
        currParticleTime = a1->particlesTimeElapsed;
        currParticleEnd = &currParticleTime[a1->num];
        currColor = a1->vertrexColors;
        v37 = 0;
        triangleChunkBackupForElementCounting[0] = GFX_gpst_Geo->triangleChunks[0].triangleChunkElems;
        triangleChunkBackupForElementCounting[1] = GFX_gpst_Geo->triangleChunks[1].triangleChunkElems;
        triangleChunkBackupForElementCounting[2] = GFX_gpst_Geo->triangleChunks[2].triangleChunkElems;
        triangleChunkBackupForElementCounting[3] = GFX_gpst_Geo->triangleChunks[3].triangleChunkElems;
        v20 = a1->vecPtr;
        if (currParticleTime < currParticleEnd)
        {
            v21 = 2;
            do
            {
                if (*((uint32_t*)currParticleTime) != 1)
                {
                    currVertices[0].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * v20->f1) + v20->v1.x;
                    currVertices[0].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * v20->f1) + v20->v1.y;
                    currVertices[0].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * v20->f1) + v20->v1.z;

                    currVertices[0].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * v20->f1) + currVertices[0].x;
                    currVertices[0].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * v20->f1) + currVertices[0].y;
                    currVertices[0].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * v20->f1) + currVertices[0].z;

                    currVertices[1].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * v20->f1) + v20->v1.x;
                    currVertices[1].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * v20->f1) + v20->v1.y;
                    currVertices[1].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * v20->f1) + v20->v1.z;

                    currVertices[1].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * -v20->f1) + currVertices[1].x;
                    currVertices[1].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * -v20->f1) + currVertices[1].y;
                    currVertices[1].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * -v20->f1) + currVertices[1].z;

                    currVertices[2].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * -v20->f1) + v20->v1.x;
                    currVertices[2].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * -v20->f1) + v20->v1.y;
                    currVertices[2].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * -v20->f1) + v20->v1.z;

                    currVertices[2].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * -v20->f1) + currVertices[2].x;
                    currVertices[2].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * -v20->f1) + currVertices[2].y;
                    currVertices[2].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * -v20->f1) + currVertices[2].z;

                    currVertices[3].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * -v20->f1) + v20->v1.x;
                    currVertices[3].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * -v20->f1) + v20->v1.y;
                    currVertices[3].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * -v20->f1) + v20->v1.z;

                    currVertices[3].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * v20->f1) + currVertices[3].x;
                    currVertices[3].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * v20->f1) + currVertices[3].y;
                    currVertices[3].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * v20->f1) + currVertices[3].z;

                    currColors[0] = *currColor;
                    currColors[1] = *currColor;
                    currColors[2] = *currColor;
                    currColors[3] = *currColor;
                    currColors += 4;
                    
                    v42 = &triangleChunkBackupForElementCounting[v37 & 3];
                    v32 = *v42;
                    v32->v1 = v21 - 2;
                    v32->v3 = v21;
                    v32->uv1 = 0;
                    v32->uv2 = 1;
                    v32->uv3 = 2;
                    ++v32;
                    v32[-1].v2 = v21 - 1;
                    v32->v1 = v21 - 2;
                    v32->v3 = v21 + 1;
                    v33 = v42;
                    v32->v2 = v21;
                    ++v32;
                    v32[-1].uv1 = 0;
                    v32[-1].uv2 = 2;
                    v32[-1].uv3 = 3;
                    *v33 = v32;
                    ++v37;
                    //currParticleTime = v39;
                    v21 += 4;
                    currVertices += 4;
                }
                ++currColor;
                ++v20;
                ++currParticleTime;
            } while (currParticleTime < currParticleEnd);
            //v19 = v41[3];
        }
        
        GFX_gpst_Geo->triangleChunks[0].triangleNum = triangleChunkBackupForElementCounting[0] - GFX_gpst_Geo->triangleChunks[0].triangleChunkElems; // GFX_NeedGeomExElem and GFX_NeedGeom modifies these
        GFX_gpst_Geo->triangleChunks[1].triangleNum = triangleChunkBackupForElementCounting[1] - GFX_gpst_Geo->triangleChunks[1].triangleChunkElems; // --
        GFX_gpst_Geo->triangleChunks[2].triangleNum = triangleChunkBackupForElementCounting[2] - GFX_gpst_Geo->triangleChunks[2].triangleChunkElems; // --
        GFX_gpst_Geo->triangleChunks[3].triangleNum = triangleChunkBackupForElementCounting[3] - GFX_gpst_Geo->triangleChunks[3].triangleChunkElems; // --
        
        drawMaskBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x8000900;
        
        if (GFX_gpst_Current->aiGameObject->distance)
            GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup;
    }
    else
    {
        PRO_QPC_RecordTimeDecRef(7);
        v7 = a1->gao;
        if (v7)
        {
            v8 = (tGameObjectVisualObject**)&v7->basic->actorObjectOrVisualObject;
            if (v8)
            {
                v9 = *v8;
                if (v9)
                {
                    v9->GROMAT = (tGroGeneric*)a2;
                    v10 = ((tGameObjectVisualObject*)(a1->gao->basic->actorObjectOrVisualObject));
                    v10->objDrawMask &= ~0x8000940u;
                    GRO_Render(a1->gao);
                }
            }
        }
        QueryPerformanceCounter(&PerfCounterStruct[7]);
    }
    if (ENG_gb_EngineRunning)
        GFX_Explode_Mecanic(a1, TIM_gf_dt);
    return 1;
}