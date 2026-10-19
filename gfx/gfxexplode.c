#include "../memory.h"
#include "../time.h"
#include "gfxexplode.h"

BOOL explodeRandomFactorsEnabled = TRUE;

void GFX_SetNumberOfSprites(tGfxExplodeData* expl, int a2)
{
    int i, j;

    expl->num = (a2 + 8) & ~7;

    if (expl->particlesTimeElapsed)
        MEM_FreeAlign(expl->particlesTimeElapsed);

    expl->particlesTimeElapsed = (float*)MEM_p_AllocAligned(4 * (expl->num + 8), 16);
    memset(expl->particlesTimeElapsed, 0, 4 * expl->num);

    if (expl->positionVecs)
        MEM_FreeAlign(expl->positionVecs);

    expl->positionVecs = (tSpriteC0DEData*)MEM_p_AllocAligned(sizeof(tSpriteC0DEData) * (expl->num + 8), 16);
    memset(expl->positionVecs, 0, sizeof(tSpriteC0DEData) * expl->num);

    if (expl->speedVecs)
        MEM_FreeAlign(expl->speedVecs);

    expl->speedVecs = (tSpriteC0DEData*)MEM_p_AllocAligned(sizeof(tSpriteC0DEData) * (expl->num + 8), 16);;
    memset(expl->speedVecs, 0, sizeof(tSpriteC0DEData) * expl->num);

    if (expl->vertexColors)
        MEM_FreeAlign(expl->vertexColors - 4);

    expl->vertexColors = (int*)((char*)MEM_p_AllocAligned(4 * (expl->num + 12), 16) + 16); // offsets it with +4, and puts color counts there later;

    *(expl->vertexColors - 1) = expl->num;
    memset(expl->vertexColors, 0, 4 * expl->num);

    for (i = 0; i < expl->num; i++) {
        *(uint32_t*)(&expl->particlesTimeElapsed[i]) = 1;
    }

    expl->ground = -9.9999998e10f;
    expl->lastGenerationTimeStamp = 0.0;
    expl->timeElapsed = 0.0;
    expl->gao = OBJ_GameObject_Create(0x5000);
    ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = (tGroGeneric*)(&expl->geo);
    ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->boneDisplayOrder_zListBufferNum = -3;
    GRO_Struct_Init((tGroGeneric*)(&expl->geo), 1);
    expl->geo.normals = &expl->speedVecs->v1;
    expl->geo.vertices = &expl->positionVecs->v1;
    expl->geo.vertexColors = expl->vertexColors - 1; // !!! puts a pointer there
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
        expl->spr[i - 1].spriteElems = &expl->spriteElems[j];
        expl->spriteElems[j].idx = 0xc0de;
        expl->spriteElems[j + 1].idx = (i - 1) * (expl->num / 4);
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

void GFX_Explode_FastMeca_CalculateSpeedAndPosition(tSpriteC0DEData* positionVecs, tSpriteC0DEData* speedVecs, tSpriteC0DEData* frictionPow, tGfxExplodeData* expl, int num, float deltaTime, float ground, float gravVal, float invDeltaTime)
{
    int i;
    float distFromGround;

    for (i = 0; i < num; i++)
    {
        positionVecs[i].v1.x = (expl->wind.v1.x + speedVecs[i].v1.x) * deltaTime + positionVecs[i].v1.x;
        positionVecs[i].v1.y = (expl->wind.v1.y + speedVecs[i].v1.y) * deltaTime + positionVecs[i].v1.y;
        positionVecs[i].v1.z = (expl->wind.v1.z + speedVecs[i].v1.z) * deltaTime + positionVecs[i].v1.z;
        positionVecs[i].f1 = (expl->wind.f1 + speedVecs[i].f1) * deltaTime + positionVecs[i].f1;
        speedVecs[i].v1.x = frictionPow->v1.x * speedVecs[i].v1.x;
        speedVecs[i].v1.y = frictionPow->v1.y * speedVecs[i].v1.y;
        speedVecs[i].v1.z = frictionPow->v1.z * speedVecs[i].v1.z;
        speedVecs[i].f1 = speedVecs[i].f1 * frictionPow->f1;
        
        distFromGround = ground - positionVecs[i].v1.z;

        if (distFromGround <= 0.0f)
            distFromGround = 0.0f;

        positionVecs[i].v1.z = distFromGround + positionVecs[i].v1.z;
        speedVecs[i].v1.z = distFromGround * invDeltaTime + speedVecs[i].v1.z + gravVal;
    }
}

void GFX_Explode_FastMeca(tGfxExplodeData* expl, tSpriteC0DEData* frictionPow, tGfxExplodeData* expl2, float deltaTime, float invDeltaTime, float gravVal, float ground)
{
    GFX_Explode_FastMeca_CalculateSpeedAndPosition(expl->positionVecs, expl->speedVecs, frictionPow, expl2, expl->num, deltaTime, ground, gravVal, invDeltaTime);
}

struct sGfxExplodeData* GFX_Explode_Create()
{
    tGfxExplodeData* expl = (tGfxExplodeData*)MEM_p_AllocAligned(sizeof(tGfxExplodeData), 16); // GFX_SetNumberOfSprites allocates ptrs on this struct
    int i;

    memset(expl, 0, sizeof(tGfxExplodeData));

    if (explodeRandomFactorsEnabled)
    {
        for (i = 255; i >= 0; i--) {
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
    if (expl->positionVecs)
        MEM_FreeAlign(expl->positionVecs);
    if (expl->speedVecs)
        MEM_FreeAlign(expl->speedVecs);
    if (expl->vertexColors)
        MEM_FreeAlign(expl->vertexColors - 4);

    if (expl->gao)
    {
        if (expl->gao->basic)
        {
            if (expl->gao->basic->actorObjectOrVisualObject)
            {
                ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROMAT = NULL;
                ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROGEOOrSomethingElse = NULL;
            }
        }
        OBJ_GameObject_Remove(expl->gao, TRUE);
    }

    expl->particlesTimeElapsed = NULL;
    expl->positionVecs = NULL;
    expl->speedVecs = NULL;
    expl->vertexColors = NULL;
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
    float lenSq;
    Vec3 tmp1, tmp2;

    MATH_RandFloat();
    expl->timeElapsed += TIM_gf_dt;

    if (expl->generationSpeed + expl->lastGenerationTimeStamp > expl->timeElapsed) return;
    if (!expl->piecesToGenerate) return;

    for (i = 0; i < expl->num; i++)
    {
        if (*((uint32_t*)&expl->particlesTimeElapsed[i]) == 1)
        {
            expl->positionVecs[i].v1 = expl->initialPos;
            MATH_VecMultScalar(&tmp1, &expl->initialPosVolumeX, (2 * MATH_RandFloat() - 1.0f));
            MATH_VecMultScalar(&tmp2, &expl->initialPosVolumeY, (2 * MATH_RandFloat() - 1.0f));
            MATH_VecAddVecWithoutBuffer(&tmp1, &tmp2);
            MATH_VecMultScalar(&tmp2, &expl->initialPosVolumeZ, (2 * MATH_RandFloat() - 1.0f));
            MATH_VecAddVecWithoutBuffer(&tmp1, &tmp2);
            MATH_VecAddVecWithoutBuffer(&expl->positionVecs[i].v1, &tmp1);
            expl->positionVecs[i].f1 = MATH_RandFloatRange(expl->initialSizeMin, expl->initialSizeMax);
            expl->vertexColors[i] = expl->phase1Color;
            expl->speedVecs[i].v1.x = MATH_RandFloatRange(expl->speedMin.x, expl->speedMax.x);
            expl->speedVecs[i].v1.y = MATH_RandFloatRange(expl->speedMin.y, expl->speedMax.y);
            expl->speedVecs[i].v1.z = MATH_RandFloatRange(expl->speedMin.z, expl->speedMax.z);
            expl->speedVecs[i].f1 = MATH_RandFloatRange(expl->growSpeedMin, expl->growSpeedMax);
            MATH_SubstractVec(&expl->speedVecs[i].v1, &expl->speedVecs[i].v1, &tmp1);
            MATH_VecAddVecWithoutBuffer(&expl->speedVecs[i].v1, &expl->mainPosSpeed);

            if ((lenSq = MATH_VecLenSquared(&expl->speedVecs[i].v1)) != 0.0f)
            {
                MATH_VecMultScalarWithoutBuffer(&expl->speedVecs[i].v1, MATH_RandFloatRange(expl->normSpeedMin, expl->normSpeedMax) * MATH_GetInverseSqrt(lenSq));
            }

            expl->particlesTimeElapsed[i] = 0.0f;
            expl->triangleNum++;
            expl->piecesToGenerate--;
            if ((4 * expl->triangleNum) > 5000)
                expl->triangleNum = 500;

            if ((expl->generationSpeed + expl->lastGenerationTimeStamp) <= expl->timeElapsed && expl->piecesToGenerate)
                expl->lastGenerationTimeStamp = expl->generationSpeed + expl->lastGenerationTimeStamp;
            else
                i = expl->num;
        }
    }
}

void GFX_Explode_Mecanic(tGfxExplodeData* expl, float deltaTime)
{
    int idx;
    int i, j;
    float totalTime;
    tSpriteC0DEData tmpV = {
        .v1 = expl->friction.v1,
        .f1 = expl->friction.f1
    };
 
    tmpV.v1.x = (float)pow(tmpV.v1.x, (deltaTime * 59.999996));
    tmpV.v1.y = (float)pow(tmpV.v1.y, (deltaTime * 59.999996));
    tmpV.v1.z = (float)pow(tmpV.v1.z, (deltaTime * 59.999996));
    tmpV.f1   = (float)pow(tmpV.f1,   (deltaTime * 59.999996));

    totalTime = expl->phase2Time + expl->phase1Time;

    GFX_Explode_FastMeca(expl, &tmpV, expl, deltaTime, (2.0f / deltaTime), (deltaTime * expl->gravity), expl->ground);

    for (i = 0, j = 0; i < expl->num; i++)
    {
        if (*(uint32_t*)&expl->particlesTimeElapsed[i] != 1) // 1 has a special meaning
        {
            if (*(uint32_t*)&expl->particlesTimeElapsed[i] <= (*(uint32_t*)&totalTime)) // fast float comparison 
            {
                if (*(uint32_t*)&expl->particlesTimeElapsed[i] <= (*(uint32_t*)&expl->phase1Time)) // fast float comparison 
                {
                    f2iMagic(expl->ooPhase1Time * expl->particlesTimeElapsed[i]);
                    idx = f2iMagic(expl->ooPhase1Time * expl->particlesTimeElapsed[i]); 
                    if (idx > 256)
                        idx = 256;
                    expl->vertexColors[i] = expl->interPolatedColors[idx];
                }
                else
                {
                    idx = f2iMagic((expl->particlesTimeElapsed[i] - expl->phase1Time) * expl->ooPhase2Time);
                    if (idx > 255)
                        idx = 255;
                    expl->vertexColors[i] = expl->interPolatedColors[idx + 256];
                }
                expl->particlesTimeElapsed[i] = (fTimeRandomFactors[j++ & 0xff] * expl->timeRandomFactor + 1.0f) * deltaTime + expl->particlesTimeElapsed[i];
            }
            else
            {
                (*(uint32_t*)&expl->particlesTimeElapsed[i]) = 1;
                expl->positionVecs[i].f1 = 0.0f;
                expl->speedVecs[i].f1 = 0.0f;
                expl->vertexColors[i] = 0x00000000;
            }
        }
    }

    MATH_AddScaleVector(&expl->initialPos, &expl->initialPos, &expl->mainPosSpeed, deltaTime);

    expl->mainPosSpeed.x = expl->mainPosFriction.x * expl->mainPosSpeed.x;
    expl->mainPosSpeed.y = expl->mainPosFriction.y * expl->mainPosSpeed.y;
    expl->mainPosSpeed.z = expl->mainPosFriction.z * expl->mainPosSpeed.z;
}

BOOL GFX_i_Explode_Render(struct sGfxExplodeData* expl, struct sGEOMAT* material)
{
    int i, j;
    int currVecIdx;
    int drawMaskBackup;
    int chunkSelectorIdx; 
    tGeoTriangleChunkElem* currTriangleChunkElemsOffsetPtr[4];   

    GFX_Explode_Generate(expl);
    
    expl->triangleNum = 0;
    for (i = 0; i < expl->num; i++)
    {
        if (*(uint32_t*)&expl->particlesTimeElapsed[i] != 1)
        {
            expl->triangleNum++;
            if ((4 * expl->triangleNum) > 5000)
                expl->triangleNum = 500;
        }
    }
    
    if (!expl->triangleNum)
        return expl->piecesToGenerate != 0;
   
    if (expl->GFXDirectly) p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists - 3;
   
    if (expl->GFXDirectly
        && ((char*)zListData->currMeshOffset - (char*)(&zListData->meshElems[0])) / sizeof(struct sZListMeshElem) +
        2 * (expl->triangleNum + sizeof(struct sZListElem)) <= 10) //check if at least 10 elems can fit into the buffer
    {
        GFX_NeedGeomEx(4 * expl->triangleNum, 4, 4, (expl->triangleNum / 2) + 8, 1);
        GFX_NeedGeomExElem(1, ((unsigned int)expl->triangleNum / 2) + 8);
        GFX_NeedGeomExElem(2, ((unsigned int)expl->triangleNum / 2) + 8);
        GFX_NeedGeomExElem(3, ((unsigned int)expl->triangleNum / 2) + 8);
        GFX_gpst_Geo->UVs[0].u = 0.0f;
        GFX_gpst_Geo->UVs[0].v = 0.0f;
        GFX_gpst_Geo->UVs[1].u = 1.0f;
        GFX_gpst_Geo->UVs[1].v = 0.0f;
        GFX_gpst_Geo->UVs[2].u = 1.0f;
        GFX_gpst_Geo->UVs[2].v = 1.0f;
        GFX_gpst_Geo->UVs[3].u = 0.0f;
        GFX_gpst_Geo->UVs[3].v = 1.0f;
        GFX_gpst_Geo->triangleChunks[0].materialNum = expl->material1ID;
        GFX_gpst_Geo->triangleChunks[1].materialNum = expl->material2ID;
        GFX_gpst_Geo->triangleChunks[2].materialNum = expl->material3ID;
        GFX_gpst_Geo->triangleChunks[3].materialNum = expl->material4ID;

        GFX_gpst_Geo->vertexColors[0] = 4 * expl->triangleNum;

        currTriangleChunkElemsOffsetPtr[0] = GFX_gpst_Geo->triangleChunks[0].triangleChunkElems;
        currTriangleChunkElemsOffsetPtr[1] = GFX_gpst_Geo->triangleChunks[1].triangleChunkElems;
        currTriangleChunkElemsOffsetPtr[2] = GFX_gpst_Geo->triangleChunks[2].triangleChunkElems;
        currTriangleChunkElemsOffsetPtr[3] = GFX_gpst_Geo->triangleChunks[3].triangleChunkElems;

        chunkSelectorIdx = 0;
        currVecIdx = 0;

        for (i = 0, j = 0; i < expl->num; i++)
        {
            if (*(uint32_t*)&expl->particlesTimeElapsed[i] != 1)
            {
                GFX_gpst_Geo->vertices[j + 0].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * expl->positionVecs[i].f1) + expl->positionVecs[i].v1.x;
                GFX_gpst_Geo->vertices[j + 0].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * expl->positionVecs[i].f1) + expl->positionVecs[i].v1.y;
                GFX_gpst_Geo->vertices[j + 0].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * expl->positionVecs[i].f1) + expl->positionVecs[i].v1.z;

                GFX_gpst_Geo->vertices[j + 0].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 0].x;
                GFX_gpst_Geo->vertices[j + 0].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 0].y;
                GFX_gpst_Geo->vertices[j + 0].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 0].z;

                GFX_gpst_Geo->vertices[j + 1].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * expl->positionVecs[i].f1) + expl->positionVecs[i].v1.x;
                GFX_gpst_Geo->vertices[j + 1].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * expl->positionVecs[i].f1) + expl->positionVecs[i].v1.y;
                GFX_gpst_Geo->vertices[j + 1].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * expl->positionVecs[i].f1) + expl->positionVecs[i].v1.z;

                GFX_gpst_Geo->vertices[j + 1].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * -expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 1].x;
                GFX_gpst_Geo->vertices[j + 1].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * -expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 1].y;
                GFX_gpst_Geo->vertices[j + 1].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * -expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 1].z;

                GFX_gpst_Geo->vertices[j + 2].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * -expl->positionVecs[i].f1) + expl->positionVecs[i].v1.x;
                GFX_gpst_Geo->vertices[j + 2].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * -expl->positionVecs[i].f1) + expl->positionVecs[i].v1.y;
                GFX_gpst_Geo->vertices[j + 2].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * -expl->positionVecs[i].f1) + expl->positionVecs[i].v1.z;

                GFX_gpst_Geo->vertices[j + 2].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * -expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 2].x;
                GFX_gpst_Geo->vertices[j + 2].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * -expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 2].y;
                GFX_gpst_Geo->vertices[j + 2].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * -expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 2].z;

                GFX_gpst_Geo->vertices[j + 3].x = (gpst_DX_stDD->ct.cam.objectMtx.v1.x * -expl->positionVecs[i].f1) + expl->positionVecs[i].v1.x;
                GFX_gpst_Geo->vertices[j + 3].y = (gpst_DX_stDD->ct.cam.objectMtx.v1.y * -expl->positionVecs[i].f1) + expl->positionVecs[i].v1.y;
                GFX_gpst_Geo->vertices[j + 3].z = (gpst_DX_stDD->ct.cam.objectMtx.v1.z * -expl->positionVecs[i].f1) + expl->positionVecs[i].v1.z;

                GFX_gpst_Geo->vertices[j + 3].x = (gpst_DX_stDD->ct.cam.objectMtx.v2.x * expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 3].x;
                GFX_gpst_Geo->vertices[j + 3].y = (gpst_DX_stDD->ct.cam.objectMtx.v2.y * expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 3].y;
                GFX_gpst_Geo->vertices[j + 3].z = (gpst_DX_stDD->ct.cam.objectMtx.v2.z * expl->positionVecs[i].f1) + GFX_gpst_Geo->vertices[j + 3].z;

                GFX_gpst_Geo->vertexColors[1 + j + 0] = expl->vertexColors[i]; // 1st offset is num, see above
                GFX_gpst_Geo->vertexColors[1 + j + 1] = expl->vertexColors[i];
                GFX_gpst_Geo->vertexColors[1 + j + 2] = expl->vertexColors[i];
                GFX_gpst_Geo->vertexColors[1 + j + 3] = expl->vertexColors[i];

                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][0].v1 = j + 0;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][0].v2 = j + 1;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][0].v3 = j + 2;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][0].uv1 = 0;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][0].uv2 = 1;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][0].uv3 = 2;

                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][1].v1 = j + 0;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][1].v2 = j + 2;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][1].v3 = j + 3;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][1].uv1 = 0;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][1].uv2 = 2;
                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3][1].uv3 = 3;

                currTriangleChunkElemsOffsetPtr[chunkSelectorIdx & 3] += 2;
                j += 4;
                chunkSelectorIdx++;
            }
        }

        GFX_gpst_Geo->triangleChunks[0].triangleNum = currTriangleChunkElemsOffsetPtr[0] - GFX_gpst_Geo->triangleChunks[0].triangleChunkElems;
        GFX_gpst_Geo->triangleChunks[1].triangleNum = currTriangleChunkElemsOffsetPtr[1] - GFX_gpst_Geo->triangleChunks[1].triangleChunkElems;
        GFX_gpst_Geo->triangleChunks[2].triangleNum = currTriangleChunkElemsOffsetPtr[2] - GFX_gpst_Geo->triangleChunks[2].triangleChunkElems;
        GFX_gpst_Geo->triangleChunks[3].triangleNum = currTriangleChunkElemsOffsetPtr[3] - GFX_gpst_Geo->triangleChunks[3].triangleChunkElems;

        drawMaskBackup = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x8000900;

        if (GFX_gpst_Current->aiGameObject->distance)
            GFX_gpst_Geo->intf->Render(GFX_gpst_GO);

        gpst_DX_stDD->drawMaskAllowedStuffCurrent = drawMaskBackup;
    }
    else
    {
        PRO_QPC_RecordTimeDecRef(7);

        if (expl->gao && expl->gao->basic && expl->gao->basic->actorObjectOrVisualObject) {
            ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->GROMAT = (tGroGeneric*)material;
            ((tGameObjectVisualObject*)(expl->gao->basic->actorObjectOrVisualObject))->objDrawMask &= ~0x8000940;
            GRO_Render(expl->gao);
        }        

        QueryPerformanceCounter(&PerfCounterStruct[7]);
    }
    if (ENG_gb_EngineRunning)
        GFX_Explode_Mecanic(expl, TIM_gf_dt);
    
    return TRUE;
}
