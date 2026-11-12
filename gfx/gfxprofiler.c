#include "../memory.h"
#include "../time.h"
#include "gfxprofiler.h"

struct sGfxProfilerData* GFX_Profiler_Create(void)
{
    tGfxProfilerData* prof;

    prof = (tGfxProfilerData*)MEM_p_Alloc(sizeof(tGfxProfilerData));
    memset(prof, 0, sizeof(tGfxProfilerData));
    return prof;
}

void GFX_Profiler_Destroy(struct sGfxProfilerData* prof)
{
    if (!prof)
        return;

    if (prof->ptr1)
        MEM_Free(prof->ptr1);
    if (prof->ptr4)
        MEM_Free(prof->ptr4);
    if (prof->ptr3)
        MEM_Free(prof->ptr3);
    if (prof->ptr2)
        MEM_Free(prof->ptr2);
}

void GFX_i_Profiler_StartStop(tGfxProfilerData* prof, unsigned int running)
{
    if (!prof->uu)
        return;

    prof->ptr3[prof->currentSlice] = running;
    if (!running && ++prof->currentSlice == prof->num)
        prof->currentSlice = 0;
}

static int GFX_l_ProfilerNextSlice(tGfxProfilerData* prof, int slice)
{
    slice++;
    if (slice == prof->num)
        slice = 0;
    return slice;
}

void GFX_i_Profiler_UpdateMecanic(tGfxProfilerData* prof, float dt)
{
    tGfxProfilerPtr4Data* point;
    tGfxProfilerPtr4Data* sliceEnd;
    tGfxProfilerPtr2Data* params;
    unsigned int* slice;
    unsigned int* slicesEnd;
    double steps;
    float dampX;
    float dampY;
    float dampZ;

    if (!prof->uu)
        return;

    slice = prof->ptr3;
    slicesEnd = &slice[prof->num];
    steps = dt / 0.016666668;
    dampX = (float)pow(1.0 - prof->v1.x, steps);
    dampY = (float)pow(1.0 - prof->v1.y, (float)steps);
    dampZ = (float)pow(1.0 - prof->v1.z, (float)steps);

    point = prof->ptr4;
    while (slice < slicesEnd)
    {
        if (!*slice)
        {
            point += prof->PPPNum;
            slice++;
            continue;
        }

        sliceEnd = &point[prof->PPPNum];
        params = prof->ptr2;
        while (point < sliceEnd)
        {
            point->v2.z = dt * prof->f0 + point->v2.z;
            MATH_AddScaleVector(&point->v2, &point->v2, &params->v4, dt);
            MATH_AddScaleVector(&point->v1, &point->v1, &point->v2, dt);
            if (point->v1.z < (double)point->fff1)
                point->v1.z = point->fff1;
            point->v2.x = dampX * point->v2.x;
            point->v2.y = dampY * point->v2.y;
            point->v2.z = dampZ * point->v2.z;
            if ((point->xx & 0xFC000000) != 0)
                point->xx -= 0x2000000;
            point++;
            params++;
        }
        slice++;
    }
}

void GFX_i_Profiler_ComputeMeshState(tGfxProfilerData* prof)
{
    unsigned int* slice;
    unsigned int* slicesEnd;
    unsigned int prev;
    int pointsNum;
    int stitched;

    if (!prof->uu)
        return;

    slice = prof->ptr3;
    slicesEnd = &slice[prof->num];
    prev = slicesEnd[-1];
    pointsNum = 0;
    stitched = 0;
    while (slice < slicesEnd)
    {
        if (*slice)
        {
            *slice = (2 * pointsNum) | 1;
            pointsNum += prof->PPPNum;
            if (prev)
                stitched++;
        }
        prev = *slice++;
    }

    prof->triangleNum = 2 * prof->LPPNum * stitched;
    prof->pointsNum = pointsNum;
    prof->UVNum = (prof->xx + 1) * 2 * prof->LPPNum;
}

static void GFX_l_ProfilerRandomInBox(Vec3* out, Vec3* base, Vec3* extent)
{
    out->x = (float)(MATH_RandFloat() * extent->x + base->x);
    out->y = (float)(MATH_RandFloat() * extent->y + base->y);
    out->z = (float)(MATH_RandFloat() * extent->z + base->z);
}

static void GFX_l_ProfilerAddInBasis(Vec3* out, tGfxProfilerData* prof, Vec3* local)
{
    MATH_AddScaleVector(out, out, &prof->v2, local->x);
    MATH_AddScaleVector(out, out, &prof->v3, local->y);
    MATH_AddScaleVector(out, out, &prof->v4, local->z);
}

void GFX_i_Profiler_Update_With_XYT(tGfxProfilerData* prof)
{
    tGfxProfilerPtr4Data* point;
    tGfxProfilerPtr4Data* sliceEnd;
    tGfxProfilerPtr4Data* slice;
    tGfxProfilerPtr4Data* prevSlice;
    tGfxProfilerPtr2Data* params;
    Vec3 local;
    Vec3 head;
    Vec3 prevHead;
    Vec3 travel;
    float moved;
    int next;
    int fade;
    int i;

    if (!prof->uu || !prof->ptr3[prof->currentSlice])
        return;

    next = GFX_l_ProfilerNextSlice(prof, prof->currentSlice);
    prof->ptr3[next] = prof->ptr3[prof->currentSlice];
    prof->currentSlice = next;
    if (prof->xxxxx)
        prof->currentSlice = (prof->currentSlice ? prof->currentSlice : prof->num) - 1;

    point = &prof->ptr4[prof->PPPNum * prof->currentSlice];
    sliceEnd = &point[prof->PPPNum];
    params = prof->ptr2;
    while (point < sliceEnd)
    {
        point->v1 = prof->v5;
        GFX_l_ProfilerRandomInBox(&local, &params->v0, &params->v2);
        GFX_l_ProfilerAddInBasis(&point->v1, prof, &local);

        MATH_SetNullVector(&point->v2);
        GFX_l_ProfilerRandomInBox(&local, &params->v1, &params->v3);
        GFX_l_ProfilerAddInBasis(&point->v2, prof, &local);

        point->v3.x = 0.0;
        point->v3.y = 0.0;
        point->v3.z = 1.0;
        point->xx = params->i2 & ~0x1F000000u;
        point->fff1 = params->f3;
        point++;
        params++;
    }
    prof->xxxxx = 0;

    if (prof->currentSlice && prof->ptr3[prof->currentSlice - 1])
    {
        slice = &prof->ptr4[prof->PPPNum * prof->currentSlice];
        MATH_VecAddVec(&head, &slice->v1, &slice[prof->PPPNum - 1].v1);
        if (prof->currentSlice)
            prevSlice = &prof->ptr4[prof->PPPNum * (prof->currentSlice - 1)];
        else
            prevSlice = &prof->ptr4[prof->PPPNum * (prof->num - 1)];
        MATH_VecAddVec(&prevHead, &prevSlice->v1, &prevSlice[prof->PPPNum - 1].v1);
        MATH_SubstractVec(&travel, &head, &prevHead);
        moved = MATH_VecLen(&travel) * 0.5f;

        if (moved >= (double)prof->f1)
        {
            // the slice moved far enough to keep, so the nine slices ahead
            // of it step one notch further down their fade
            fade = prof->currentSlice;
            for (i = 0; i < 9; i++)
            {
                if (fade < prof->num - 1)
                    fade++;
                else
                    fade = 0;
                point = &prof->ptr4[prof->PPPNum * fade];
                sliceEnd = &point[prof->PPPNum];
                while (point < sliceEnd)
                {
                    if ((point->xx & 0xE0000000) != 0)
                        point->xx -= 0x20000000;
                    point++;
                }
            }
        }
        else
        {
            prof->xxxxx = 1;
        }
    }

    prof->ptr3[GFX_l_ProfilerNextSlice(prof, prof->currentSlice)] = 0;
}

void GFX_Profiler_Validate(tGfxProfilerData* prof)
{
    if (prof->uu || !prof->PPPNum || !prof->LPPNum || !prof->num)
        return;

    prof->ptr1 = (tGfxProfilerPtr1Data*)
                 MEM_p_Alloc(sizeof(tGfxProfilerPtr1Data) * prof->LPPNum);
    memset(prof->ptr1, 0, sizeof(tGfxProfilerPtr1Data) * prof->LPPNum);
    prof->ptr2 = (tGfxProfilerPtr2Data*)
                 MEM_p_Alloc(sizeof(tGfxProfilerPtr2Data) * prof->PPPNum);
    memset(prof->ptr2, 0, sizeof(tGfxProfilerPtr2Data) * prof->PPPNum);
    prof->ptr3 = (unsigned int*)MEM_p_Alloc(4 * prof->num);
    memset(prof->ptr3, 0, 4 * prof->num);
    prof->ptr4 = (tGfxProfilerPtr4Data*)
                 MEM_p_Alloc(prof->num * sizeof(tGfxProfilerPtr4Data) * prof->PPPNum);
    memset(prof->ptr4, 0, prof->num * sizeof(tGfxProfilerPtr4Data) * prof->PPPNum);
    prof->uu = 1;
    prof->xx = 4;
}

int GFX_i_Profiler_Render(struct sGfxProfilerData* prof)
{
    tGfxProfilerPtr4Data* point;
    tGfxProfilerPtr4Data* sliceEnd;
    tGfxProfilerPtr1Data* line;
    tGfxProfilerPtr1Data* linesEnd;
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    unsigned int* slice;
    unsigned int* slicesEnd;
    unsigned int prev;
    unsigned int curBase;
    unsigned int prevBase;
    Vec3* verts;
    Vec3* normals;
    int* colors;
    float u;
    float uStep;
    int uvBase;
    int savedMask;
    int row;

    if (!prof->uu)
        return 1;

    GFX_i_Profiler_UpdateMecanic(prof, TIM_gf_dt);
    GFX_i_Profiler_Update_With_XYT(prof);
    GFX_i_Profiler_ComputeMeshState(prof);
    if (!prof->pointsNum || !prof->triangleNum)
        return 1;

    GFX_NeedGeom(prof->pointsNum, prof->UVNum, prof->triangleNum, 1);

    point = prof->ptr4;
    colors = GFX_gpst_Geo->vertexColors + 1;
    verts = GFX_gpst_Geo->vertices;
    normals = GFX_gpst_Geo->normals;
    slice = prof->ptr3;
    slicesEnd = &slice[prof->num];
    while (slice < slicesEnd)
    {
        if (!*slice)
        {
            point += prof->PPPNum;
            slice++;
            continue;
        }

        sliceEnd = &point[prof->PPPNum];
        while (point < sliceEnd)
        {
            *verts++ = point->v1;
            *normals++ = point->v3;
            *colors++ = point->xx;
            point++;
        }
        slice++;
    }

    linesEnd = &prof->ptr1[prof->LPPNum];
    uStep = (float)(1.0 / (double)prof->xx);
    u = 1.0;
    uvs = GFX_gpst_Geo->UVs;
    for (row = prof->xx + 1; row; row--)
    {
        for (line = prof->ptr1; line < linesEnd; line++)
        {
            uvs[0].u = u;
            uvs[1].u = u;
            uvs[0].v = line->f1;
            uvs[1].v = line->f2;
            uvs += 2;
        }
        u = u - uStep;
    }

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    slice = prof->ptr3;
    slicesEnd = &slice[prof->num];
    prev = slicesEnd[-1];
    uvBase = 0;
    while (slice < slicesEnd)
    {
        if ((prev & *slice) != 0)
        {
            for (line = prof->ptr1; line < linesEnd; line++)
            {
                curBase = *slice >> 1;
                prevBase = prev >> 1;

                tris[0].v1 = line->s1 + curBase;
                tris[0].v2 = line->s2 + curBase;
                tris[0].v3 = line->s2 + prevBase;
                tris[0].uv1 = uvBase + 2 * prof->LPPNum;
                tris[0].uv2 = uvBase + 2 * prof->LPPNum + 1;
                tris[0].uv3 = uvBase + 1;

                tris[1].v1 = line->s2 + prevBase;
                tris[1].v2 = line->s1 + prevBase;
                tris[1].v3 = line->s1 + curBase;
                tris[1].uv1 = uvBase + 1;
                tris[1].uv2 = uvBase;
                tris[1].uv3 = uvBase + 2 * prof->LPPNum;

                tris += 2;
                uvBase += 2;
            }
        }
        else
        {
            uvBase += 2 * prof->LPPNum;
        }
        if (uvBase == 2 * prof->LPPNum * (int)prof->xx)
            uvBase = 0;
        prev = *slice++;
    }

    savedMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask & ~0x900u;
    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    if (GFX_gpst_Current->aiGameObject->distance)
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask;
    return 1;
}

void GFX_SetNumberOfPPP(tGfxProfilerData* prof, int num)
{
    prof->PPPNum = num;
    GFX_Profiler_Validate(prof);
}

void GFX_SetNumberOfLPP(tGfxProfilerData* prof, int num)
{
    prof->LPPNum = num;
    GFX_Profiler_Validate(prof);
}

void GFX_SetNumberOfP(tGfxProfilerData* prof, unsigned int num)
{
    prof->num = num;
    prof->num2 = 255 / num;
    if (!prof->num2)
        prof->num2 = 1;
    if (prof->num2 >= 127u)
        prof->num2 = 127;
    prof->num2 <<= 24;
    GFX_Profiler_Validate(prof);
}

void GFX_Profiler_Seti(struct sGfxProfilerData* prof, int id, int value)
{
    switch (id)
    {
    case 12100:
        GFX_SetNumberOfPPP(prof, value);
        GFX_Profiler_Validate(prof);
        break;
    case 12101:
        GFX_SetNumberOfLPP(prof, value);
        GFX_Profiler_Validate(prof);
        break;
    case 12102:
        GFX_SetNumberOfP(prof, value);
        GFX_Profiler_Validate(prof);
        break;
    case 12103:
        GFX_i_Profiler_StartStop(prof, value);
        break;
    case 12104:
        prof->ptr2CurrID = value;
        break;
    case 12105:
        prof->ptr1CurrID = value;
        break;
    case 12106:
        prof->ptr1[prof->ptr1CurrID].s1 = value;
        break;
    case 12107:
        prof->ptr1[prof->ptr1CurrID].s2 = value;
        break;
    case 12108:
        prof->ptr1[prof->ptr1CurrID].s3 = value;
        break;
    case 12109:
        prof->ptr2[prof->ptr2CurrID].i2 = value;
        break;
    case 12110:
        prof->ptr2[prof->ptr2CurrID].i3 = value;
        break;
    case 12111:
        prof->xx = value;
        break;
    default:
        break;
    }
}

void GFX_Profiler_Setf(struct sGfxProfilerData* prof, int id, float value)
{
    switch (id)
    {
    case 12000:
        prof->f0 = value;
        break;
    case 12001:
        prof->ptr1[prof->ptr1CurrID].f1 = value;
        break;
    case 12002:
        prof->ptr1[prof->ptr1CurrID].f2 = value;
        break;
    case 12003:
        prof->ptr2[prof->ptr2CurrID].f3 = value;
        break;
    case 12004:
        prof->f1 = value;
        break;
    default:
        break;
    }
}

void GFX_Profiler_Setv(struct sGfxProfilerData* prof, int id, Vec3* value)
{
    switch (id)
    {
    case 12200:
        prof->ptr2[prof->ptr2CurrID].v0 = *value;
        break;
    case 12201:
        prof->ptr2[prof->ptr2CurrID].v1 = *value;
        break;
    case 12202:
        prof->v1 = *value;
        break;
    case 12203:
        prof->v2 = *value;
        break;
    case 12204:
        prof->v3 = *value;
        break;
    case 12205:
        prof->v4 = *value;
        break;
    case 12206:
        prof->v5 = *value;
        break;
    case 12207:
        prof->ptr2[prof->ptr2CurrID].v2 = *value;
        break;
    case 12208:
        prof->ptr2[prof->ptr2CurrID].v3 = *value;
        break;
    default:
        break;
    }
}
