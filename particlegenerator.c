#include "particlegenerator.h"
#include "modifiers/stpmodifier.h"
#include "time.h"
#include "memory.h"
#include "gameobject.h"

int PAGRenderingEnabled = 1;

static void PAG_l_ScaleToRandomSpeed(tPAGElemStruct* elem, float speedMin, float speedMax)
{
    double len;
    double speed;
    double scale;

    len = sqrt(elem->v1.x * elem->v1.x + elem->v1.y * elem->v1.y + elem->v1.z * elem->v1.z);
    if (len == 0.0)
    {
        scale = 0.0;
    }
    else
    {
        speed = MATH_RandFloatRange(speedMin, speedMax);
        scale = speed / (float)len;
    }
    elem->v1.x = (float)(scale * elem->v1.x);
    elem->v1.y = (float)(scale * elem->v1.y);
    elem->v1.z = (float)(scale * elem->v1.z);
}

void PAG_Update(struct sGameObjectMain* gao, struct sPAGStruct* pag, float dt)
{
    tPAGElemStruct* elem;
    double pending;
    double phase;
    float stepTime;
    int remaining;
    int steps;
    int step;
    int recycled;
    int lastIdx;
    int flags;
    unsigned int spin;
    int16_t spinSpeed;
    Vec3 accelStep;
    float dx;
    float dy;
    float distSq;

    pending = dt + pag->timeElapsed;
    elem = pag->pagElems;
    remaining = pag->triangleNum2;
    steps = (unsigned int)(60.0 * pending);
    stepTime = (float)((double)steps * 0.016666999);
    pag->timeElapsed = (float)(pending - stepTime);
    while (remaining)
    {
        --remaining;
        recycled = 0;
        while (elem->ff2 < (double)stepTime)
        {
            if ((elem->particleState & 1) != 0)
            {
                elem->particleState = 0;
                phase = MATH_RandFloatRange(pag->ffb13, pag->ffb14);
            }
            else if ((elem->particleState & 2) != 0)
            {
                lastIdx = pag->triangleNum2 - 1;
                pag->triangleNum2 = lastIdx;
                if (!remaining)
                    return;
                memcpy(elem, &pag->pagElems[lastIdx], sizeof(tPAGElemStruct));
                recycled = 1;
                break;
            }
            else
            {
                elem->particleState = 2;
                phase = MATH_RandFloatRange(pag->ffb15, pag->ffb16);
            }
            elem->ff2 = (float)phase;
            elem->ff = (float)(1.0 / phase);
        }
        if (recycled)
            continue;
        elem->ff2 = elem->ff2 - stepTime;
        elem->fff = stepTime + elem->fff;
        if ((pag->flags & 0x1000) != 0)
        {
            spin = elem->f5;
            if ((spin & 0x8000) == 0)
            {
                elem->f5 = 65537 * spin;
            }
            else
            {
                spinSpeed = elem->f5;

                elem->f5 &= 0x0000ffff; //HIWORD(v5->f5) = 0;
                elem->f5 = ((int16_t)(((spin >> 16) & 0xffff) - (spinSpeed & 0x7FFF)) << 16) | elem->f5;
            }
        }
        for (step = steps; step; --step)
        {
            accelStep.x = (float)(pag->v1.x * 0.016666999);
            accelStep.y = (float)(pag->v1.y * 0.016666999);
            accelStep.z = (float)(pag->v1.z * 0.016666999);
            MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &accelStep, 0.0083330004f);           // MATH_AddScaleVector inline x2
            MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &elem->v1, 0.016666999f);
            if ((pag->flags & 0x8000u) != 0)
                MATH_VecAddVec(&elem->v1, &elem->v1, &accelStep);
            flags = pag->flags;
            if ((flags & 0x400) != 0 && elem->particlePos.z - elem->ffff2 < pag->i3)
            {
                dx = elem->particlePos.x - gao->matrices->Pos.x;
                dy = elem->particlePos.y - gao->matrices->Pos.y;
                distSq = dx * dx + dy * dy;
                if (pag->ffa9 == 0.0 || distSq > pag->ffa9)
                {
                    elem->particlePos.z = elem->ffff2 + pag->i3;
                    elem->v1.z = -(pag->ff * elem->v1.z);
                }
            }
            else if ((flags & 0x800) != 0 && elem->ffff2 + elem->particlePos.z > pag->ffa1)
            {
                elem->particlePos.z = pag->ffa1 - elem->ffff2;
                elem->v1.z = -(pag->ffa2 * elem->v1.z);
            }
            else
            {
                MATH_VecAddVec(&elem->v1, &elem->v1, &accelStep);
            }
            if ((pag->flags & 0x2000) != 0)
                MATH_VecMultScalarWithoutBuffer(&elem->v1, pag->f1);
        }
        if (pag->ffa7 != 0.0)
            elem->particlePos.x = MATH_GetSin(pag->ffa7 * elem->fff) * stepTime + elem->particlePos.x;
        if (pag->ffa8 != 0.0)
            elem->particlePos.y = MATH_GetSin(elem->fff * pag->ffa8) * stepTime + elem->particlePos.y;
        ++elem;
    }
}

void PAG_Generate(tGameObjectMain* gao, tPAGStruct* pag, float dt)
{
    tGEOGRO* gro;
    tModifier* mdf;
    tStpMdfStruct* stp;
    tPAGGeneratorElemStruct* gen;
    tPAGElemStruct* elem;
    Mtx4x4* shapeMtx;
    Mtx4x4* genMtx;
    Vec3* vertices = NULL;
    Vec3* vertex;
    int num;
    int genIdx;
    int spawnCount;
    int spawn;
    int vertexIdx;
    int skipped;
    double pending;
    double offset;
    double size;
    double life;
    double spinRate;
    float azimuth;
    float elevation;
    float radius;
    float elevCos;
    float yaw;
    float pitch;
    float pitchCos;
    float speed;
    float coneAngle;
    float rnd;

    num = pag->num;
    gro = (tGEOGRO*)OBJ_p_GetGro(pag->gao1);
    if (gro && gro->intf->id != 1)
        gro = NULL;
    if (pag->mode == 6)
    {
        if (!gro)
            return;
        vertices = gro->vertices;
        mdf = MDF_pst_GetByType(pag->gao1, 21);       // stp
        if (mdf)
        {
            stp = (tStpMdfStruct*)mdf->mdfPrivateDataPtr;
            if (stp->hasStp)
                vertices = stp->verticesStored;
        }
        if (pag->gaoForMatrixRef)
            shapeMtx = OBJ_pst_GetAbsoluteMatrix(pag->gaoForMatrixRef);
        else
            shapeMtx = OBJ_pst_GetAbsoluteMatrix(pag->gao1);
    }
    else
    {
        shapeMtx = NULL; // TODO FIXME: ez a kódban is hibás
    }
    for (genIdx = 0; genIdx < num; genIdx++)
    {
        gen = &pag->generators[genIdx];
        if (gen->generatorFreq == 0.0)
            continue;
        pending = dt + gen->fff22;
        spawnCount = (unsigned int)(pending * gen->generatorFreq);
        gen->fff22 = (float)(pending - (double)spawnCount / gen->generatorFreq);
        if (pag->triangleNum2 + spawnCount >= pag->triangleNum)
            spawnCount = pag->triangleNum - pag->triangleNum2 - 1;
        if (spawnCount <= 0)
            continue;
        if (gen->gao)
            genMtx = OBJ_pst_GetAbsoluteMatrix(gen->gao);
        else
            genMtx = OBJ_pst_GetAbsoluteMatrix(gao);
        if (pag->mode != 6)
            shapeMtx = genMtx;
        for (spawn = spawnCount; spawn; --spawn)
        {
            elem = &pag->pagElems[pag->triangleNum2++];
            elem->ff2 = 0.0;
            elem->fff = 0.0;
            if (pag->fd21 != 0.0)
            {
                elem->particleState = 1;
                elem->ff2 = MATH_RandFloatRange(pag->ffb17, pag->fd21);
            }
            if (elem->ff2 == 0.0)
            {
                elem->particleState = 0;
                life = MATH_RandFloatRange(pag->ffb13, pag->ffb14);
                elem->ff2 = (float)life;
                if (life == 0.0 && pag->ffb16 != 0.0)
                {
                    elem->particleState = 2;
                    elem->ff2 = MATH_RandFloatRange(pag->ffb15, pag->ffb16);
                }
            }
            elem->ff = (float)(1.0 / elem->ff2);
            size = MATH_RandFloatRange(pag->ffb9, pag->ffb10);
            elem->pagPosAndVertexIndexFloatBase = (float)size;
            if ((pag->flags & 4) == 0)
                size = MATH_RandFloatRange(pag->ffb11, pag->ffb12);
            elem->ffff2 = (float)size;
            skipped = 0;
            switch (pag->mode)                 // csomó addScaleVector inline
            {
            case 0u:
                elem->particlePos = shapeMtx->Pos;
                break;
            case 1u:
                rnd = MATH_RandFloat();
                offset = (rnd + rnd - 1.0) * pag->ffb2;
                MATH_AddScaleVector(&elem->particlePos, &shapeMtx->Pos, &shapeMtx->v1, (float)offset);
                rnd = MATH_RandFloat();
                offset = (rnd + rnd - 1.0) * pag->ffb3;
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v3, (float)offset);
                break;
            case 2u:
                azimuth = (float)(MATH_RandFloat() * 12.566371);
                radius = MATH_RandFloat() * pag->ffb2;
                MATH_AddScaleVector(&elem->particlePos, &shapeMtx->Pos, &shapeMtx->v1, MATH_GetCos(azimuth) * radius);
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v3, MATH_GetSin(azimuth) * radius);
                break;
            case 3u:
                azimuth = (float)(MATH_RandFloat() * 12.566371);
                radius = MATH_RandFloat() * pag->ffb2;
                MATH_AddScaleVector(&elem->particlePos, &shapeMtx->Pos, &shapeMtx->v1, MATH_GetCos(azimuth) * radius);
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v3, MATH_GetSin(azimuth) * radius);
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v2, -(MATH_RandFloat() * pag->ffb3));
                break;
            case 4u:
                azimuth = (float)(MATH_RandFloat() * 6.2831855);
                elevation = (float)(MATH_RandFloat() * 3.1415927 - 1.5707964);
                radius = MATH_RandFloat() * pag->ffb2;
                elevCos = MATH_GetCos(elevation);
                MATH_AddScaleVector(&elem->particlePos, &shapeMtx->Pos, &shapeMtx->v1, MATH_GetCos(azimuth) * elevCos * radius);
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v2, MATH_GetCos(elevation) * MATH_GetSin(azimuth) * radius);
                // preserved: the third axis takes cos(elevation) where subMode 2's double cone, the same shape, takes sin
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v3, MATH_GetCos(elevation) * radius);
                break;
            case 5u:
                rnd = MATH_RandFloat();
                offset = (rnd + rnd - 1.0) * pag->ffb2;
                MATH_AddScaleVector(&elem->particlePos, &shapeMtx->Pos, &shapeMtx->v1, (float)offset);
                rnd = MATH_RandFloat();
                offset = (rnd + rnd - 1.0) * pag->ffb3;
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v3, (float)offset);
                MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v2, -(MATH_RandFloat() * pag->ffb4));
                break;
            case 6u:
                vertexIdx = gro->vertexNum * RND_JADErand() / 0x7FFF;
                vertex = &vertices[vertexIdx];
                if (vertex->z < (double)pag->i3 || vertex->z > (double)pag->ffa1)
                {
                    --pag->triangleNum2;
                    skipped = 1;
                }
                else
                {
                    MATH_TransformVertex(&elem->particlePos, shapeMtx, vertex);
                }
                break;
            default:
                break;
            }
            if (skipped)
                continue;
            MATH_AddScaleVector(&elem->particlePos, &elem->particlePos, &shapeMtx->v2, -pag->ffb1);
            switch (pag->subMode)
            {
            case 0u:
                MATH_VecMultScalar(&elem->v1, &genMtx->v2, -MATH_RandFloatRange(pag->ffb5, pag->ffb6));
                break;
            case 1u:
                coneAngle = MATH_RandFloatRange(-pag->ffb7, pag->ffb7);
                speed = MATH_RandFloatRange(pag->ffb5, pag->ffb6);
                MATH_VecMultScalar(&elem->v1, &genMtx->v2, -(MATH_GetCos(coneAngle) * speed));
                MATH_AddScaleVector(&elem->v1, &elem->v1, &genMtx->v1, MATH_GetSin(coneAngle) * speed);
                break;
            case 2u:
                yaw = MATH_RandFloatRange(-pag->ffb7, pag->ffb7);
                // preserved: the pitch range runs from -ffb7 to ffb8, where every other cone range here is symmetric
                pitch = MATH_RandFloatRange(-pag->ffb7, pag->ffb8);
                speed = MATH_RandFloatRange(pag->ffb5, pag->ffb6);
                pitchCos = MATH_GetCos(pitch);
                MATH_VecMultScalar(&elem->v1, &genMtx->v2, -(MATH_GetCos(yaw) * pitchCos * speed));
                MATH_AddScaleVector(&elem->v1, &elem->v1, &genMtx->v1, MATH_GetCos(pitch) * MATH_GetSin(yaw) * speed);
                MATH_AddScaleVector(&elem->v1, &elem->v1, &genMtx->v3, MATH_GetSin(pitch) * speed);
                break;
            case 3u:
                elem->v1.x = elem->particlePos.x - shapeMtx->Pos.x;
                elem->v1.y = elem->particlePos.y - shapeMtx->Pos.y;
                elem->v1.z = elem->particlePos.z - shapeMtx->Pos.z;
                PAG_l_ScaleToRandomSpeed(elem, pag->ffb5, pag->ffb6);
                break;
            case 4u:
                GEO_UseNormals(gro);
                // preserved: vertexIdx is only ever set by mode 6, and gro is only ever non-NULL there too
                MATH_VecMultScalar(&elem->v1, &gro->normals[vertexIdx], -MATH_RandFloatRange(pag->ffb5, pag->ffb6));
                break;
            case 5u:
                elem->v1.x = pag->ffa3 - elem->particlePos.x;
                elem->v1.y = pag->ffa4 - elem->particlePos.y;
                elem->v1.z = pag->ffa5 - elem->particlePos.z;
                PAG_l_ScaleToRandomSpeed(elem, pag->ffb5, pag->ffb6);
                break;
            default:
                break;
            }
            if ((pag->flags & 0x1000) != 0)
            {
                elem->f5 = (unsigned int)(MATH_RandFloatRange(pag->ffa3, pag->ffa4) * 162.97466) << 22;
                spinRate = MATH_RandFloatRange(pag->ffa5, pag->ffa6) * 10430.378;
                if (spinRate < 0.0)
                {
                    spinRate = -spinRate;
                    elem->f5 |= 0x8000u;
                }
                elem->f5 |= (unsigned int)spinRate & 0x7FFF;
            }
            else
            {
                elem->f5 = 0;
            }
        }
    }
}

void PAG_RenderCommon(struct sGameObjectMain* gao, struct sPAGStruct* pag)
{
    tGdi_DisplayData* dd;
    tGameObjectBasic* basic;
    tGameObjectVisualObject* visual;
    tGeoTriangleChunkElem* tri;
    tGeoSpriteElem* sprite;
    tPAGElemStruct* elem;
    tPAGElemStruct* end;
    Mtx4x4* objMtx;
    Vec3* axisU;
    Vec3* axisV;
    Vec3* vertex;
    int* colors;
    int flags;
    int unsorted;
    int fixedSize;
    int color;
    int savedDrawMask;
    int savedFlags;
    int base;
    uint16_t savedMatrixElem;
    unsigned int spin;
    unsigned int spinIdx;
    float spinSin;
    float spinCos;
    float size;
    float height;
    float factor;
    double growth;
    double spriteSize;
    Vec3 right;
    Vec3 up;
    Vec3 fixedRight;
    Vec3 fixedUp;
    Vec3 spun;

    flags = pag->flags;
    unsorted = (flags & 0x10000) == 0;
    if ((flags & 0x40) == 0 || !gao->distance)
        return;
    dd = gpst_DX_stDD;
    if ((pag->flags & 0x10000) != 0 && (flags & 1) != 0 && (gpst_DX_stDD->flags & 4) == 0)
    {
        SOFT_AddCurrentObjectInZList(gpst_DX_stDD, gao);
        return;
    }
    if (pag->renderCnt != gpst_DX_stDD->rasters[4].unk1)
    {
        if (ENG_gb_EngineRunning && (flags & 0x80u) == 0)
        {
            PAG_Update(gao, pag, TIM_gf_dt);
            PAG_Generate(gao, pag, TIM_gf_dt);
            dd = gpst_DX_stDD;
        }
        pag->renderCnt = dd->rasters[4].unk1;
        dd = gpst_DX_stDD;
    }
    if (!pag->triangleNum2)
        return;
    savedDrawMask = dd->drawMaskAllowedStuffCurrent;
    savedFlags = dd->flags;
    if (unsorted)
    {
        dd->flags |= 8u;
        dd = gpst_DX_stDD;
        if ((pag->flags & 1) != 0)
        {
            gpst_DX_stDD->flags |= 0x200u;
            dd = gpst_DX_stDD;
        }
    }
    savedMatrixElem = dd->matrixStack.currElemNum;
    dd->matrixStack.actMatrixElem = &dd->ct.cam.cameraMtx;
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
    objMtx = &gpst_DX_stDD->ct.cam.objectMtx;
    if (pag->ffb10 == pag->ffb9 && pag->ffb12 == pag->ffb11 && (flags = pag->flags, (flags & 0x30) == 0))
    {
        MATH_VecMultScalar(&fixedRight, &gpst_DX_stDD->ct.cam.objectMtx.v1, pag->ffb9);
        if ((flags & 4) != 0)
            MATH_VecMultScalar(&fixedUp, &objMtx->v2, pag->ffb9);
        else
            MATH_VecMultScalar(&fixedUp, &objMtx->v2, pag->ffb11);
        right = fixedRight;
        up = fixedUp;
        axisU = &fixedRight;
        axisV = &fixedUp;
        fixedSize = 1;
    }
    else
    {
        axisU = &gpst_DX_stDD->ct.cam.objectMtx.v1;
        axisV = &gpst_DX_stDD->ct.cam.objectMtx.v2;
        fixedSize = 0;
    }
    basic = gao->basic;
    if (!basic)
        return;
    visual = basic->actorObjectOrVisualObject;
    if (!visual)
        return;
    ((tGameObjectVisualObject*)(GFX_gpst_GO->basic->actorObjectOrVisualObject))->GROMAT = visual->GROMAT;
    if (PAGRenderingEnabled)
    {
        GFX_NeedGeom(4 * pag->triangleNum2, 4u, 2 * pag->triangleNum2, 1);
        GFX_gpst_Geo->triangleChunks->materialNum = 0;
        tri = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        colors = GFX_gpst_Geo->vertexColors + 1;
        vertex = GFX_gpst_Geo->vertices;
        GFX_SetQuadUVs();
        elem = pag->pagElems;
        end = &elem[pag->triangleNum2];
        while (elem < end)
        {
            color = pag->color;
            size = elem->pagPosAndVertexIndexFloatBase;
            height = elem->ffff2;
            if ((elem->particleState & 1) != 0)
            {
                flags = pag->flags;
                factor = (float)(1.0 - elem->ff2 * elem->ff);
                if ((flags & 0x100) != 0)
                {
                    color = COLOR_ul_Mul(color, factor);
                    flags = pag->flags;
                    if ((flags & 2) == 0)
                        color = (color ^ pag->color) & 0xFFFFFF ^ color;
                }
                if ((flags & 0x200) != 0)
                {
                    size = factor * size;
                    height = factor * height;
                }
            }
            else if ((elem->particleState & 2) != 0)
            {
                flags = pag->flags;
                factor = elem->ff2 * elem->ff;
                if ((flags & 8) != 0)
                {
                    color = COLOR_ul_Mul(color, factor);
                    flags = pag->flags;
                    if ((flags & 2) == 0)
                        color = (color ^ pag->color) & 0xFFFFFF ^ color;
                }
                if ((flags & 0x10) != 0)
                {
                    size = factor * size;
                    height = factor * height;
                }
                else if ((flags & 0x20) != 0)
                {
                    growth = (1.0 - factor) * pag->fd22 + 1.0;
                    size = (float)(size * growth);
                    height = (float)(growth * height);
                }
            }
            if (!fixedSize)
            {
                MATH_VecMultScalar(&right, axisU, size);
                MATH_VecMultScalar(&up, axisV, height);
            }
            spin = elem->f5;
            if (spin)
            {
                if (fixedSize)
                {
                    right = *axisU;
                    up = *axisV;
                }
                spinIdx = spin >> 22;
                // preserved: the sine index reaches 1024, one past the end of MATH_gf_TableSin[1024]
                spinSin = MATH_gf_TableSin[spinIdx + 1];
                spinCos = MATH_gf_TableSin[(spinIdx + 256) % 0x3FF + 1];
                MATH_VecMultScalar(&spun, &right, spinCos);
                MATH_AddScaleVector(&spun, &spun, &up, spinSin);
                MATH_VecMultScalarWithoutBuffer(&up, spinCos);
                MATH_AddScaleVector(&up, &up, &right, -spinSin);
                right = spun;
            }
            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            colors += 4;
            base = ((char*)vertex - (char*)GFX_gpst_Geo->vertices) / sizeof(Vec3);
            tri[0].v1 = base;
            tri[0].v2 = base + 1;
            tri[0].v3 = base + 2;
            tri[0].uv1 = 0;
            tri[0].uv2 = 1;
            tri[0].uv3 = 2;
            tri[1].v1 = base + 2;
            tri[1].v2 = base + 3;
            tri[1].v3 = base;
            tri[1].uv1 = 2;
            tri[1].uv2 = 3;
            tri[1].uv3 = 0;
            tri += 2;
            vertex[0].x = elem->particlePos.x - right.x;
            vertex[0].y = elem->particlePos.y - right.y;
            vertex[0].z = elem->particlePos.z - right.z;
            vertex[0].x = vertex[0].x - up.x;
            vertex[0].y = vertex[0].y - up.y;
            vertex[0].z = vertex[0].z - up.z;
            vertex[1].x = elem->particlePos.x - right.x;
            vertex[1].y = elem->particlePos.y - right.y;
            vertex[1].z = elem->particlePos.z - right.z;
            vertex[1].x = up.x + vertex[1].x;
            vertex[1].y = up.y + vertex[1].y;
            vertex[1].z = up.z + vertex[1].z;
            vertex[2].x = right.x + elem->particlePos.x;
            vertex[2].y = right.y + elem->particlePos.y;
            vertex[2].z = right.z + elem->particlePos.z;
            vertex[2].x = up.x + vertex[2].x;
            vertex[2].y = up.y + vertex[2].y;
            vertex[2].z = up.z + vertex[2].z;
            vertex[3].x = right.x + elem->particlePos.x;
            vertex[3].y = right.y + elem->particlePos.y;
            vertex[3].z = right.z + elem->particlePos.z;
            vertex[3].x = vertex[3].x - up.x;
            vertex[3].y = vertex[3].y - up.y;
            vertex[3].z = vertex[3].z - up.z;
            vertex += 4;
            ++elem;
        }
    }
    else
    {
        GFX_NeedGeomEx(pag->triangleNum2, 4u, 0, 0, 1);
        GFX_NeedGeomNbElemSprites(1u);
        GFX_NeedGeomElemNbSprites(0, pag->triangleNum2);
        GFX_gpst_Geo->spritesPtr->matIdx = 0;
        GFX_gpst_Geo->spritesPtr->globalSize = 1.0;
        GFX_gpst_Geo->spritesPtr->globalRatio = 1.0;
        vertex = GFX_gpst_Geo->vertices;
        sprite = GFX_gpst_Geo->spritesPtr->spriteElems;
        colors = GFX_gpst_Geo->vertexColors + 1;
        GFX_SetQuadUVs();
        elem = pag->pagElems;
        end = &elem[pag->triangleNum2];
        while (elem < end)
        {
            spriteSize = elem->pagPosAndVertexIndexFloatBase;
            color = pag->color;
            if ((elem->particleState & 1) != 0)
            {
                flags = pag->flags;
                factor = (float)(1.0 - elem->ff2 * elem->ff);
                if ((flags & 0x100) != 0)
                {
                    color = COLOR_ul_Mul(color, factor);
                    flags = pag->flags;
                    if ((flags & 2) == 0)
                        color ^= (color ^ pag->color) & 0xFFFFFF;
                }
                if ((flags & 0x200) != 0)
                    spriteSize = spriteSize * factor;
            }
            else if ((elem->particleState & 2) != 0)
            {
                flags = pag->flags;
                factor = elem->ff2 * elem->ff;
                if ((flags & 8) != 0)
                {
                    color = COLOR_ul_Mul(color, factor);
                    flags = pag->flags;
                    if ((flags & 2) == 0)
                        color ^= (color ^ pag->color) & 0xFFFFFF;
                }
                if ((flags & 0x10) != 0)
                    spriteSize = spriteSize * factor;
                else if ((flags & 0x20) != 0)
                    spriteSize = spriteSize * ((1.0 - factor) * pag->fd22 + 1.0);
            }
            colors[0] = color;
            sprite->size = (float)spriteSize;
            ++colors;
            sprite->idx = (uint16_t)((char*)vertex - (char*)GFX_gpst_Geo->vertices) / sizeof(Vec3);
            vertex->x = elem->particlePos.x;
            vertex->y = elem->particlePos.y;
            vertex->z = elem->particlePos.z;
            ++sprite;
            ++vertex;
            ++elem;
        }
    }
    gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x8000900u;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedDrawMask;
    gpst_DX_stDD->flags = savedFlags;
    gpst_DX_stDD->matrixStack.currElemNum = savedMatrixElem;
    gpst_DX_stDD->matrixStack.actMatrixElem = &gpst_DX_stDD->matrixStack.elems[savedMatrixElem];
    gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender = gpst_DX_stDD->matrixStack.actMatrixElem;
    gpst_DX_stDD->SetViewMatrix(gpst_DX_stDD->ct.cam.viewMtxPtr_setInGRORender);
}
