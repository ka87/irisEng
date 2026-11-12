#include "../memory.h"
#include "../time.h"
#include "../math.h"
#include "gfxlightning.h"

void GFX_Lightning_SampleZero(tGfxLightningData* bolt)
{
    int i;

    if (!bolt->pointsNumAllocd)
        return;

    for (i = 0; i < bolt->pointsNum; i++)
    {
        bolt->points[i].x = 0.0;
        bolt->points[i].y = 0.0;
        bolt->points[i].z = (float)((double)(i + 1) / (double)(bolt->pointsNum + 1));
    }
}

struct sGfxLightningData* GFX_Lightning_Create(void)
{
    tGfxLightningData* bolt;

    bolt = (tGfxLightningData*)MEM_p_Alloc(sizeof(tGfxLightningData));
    bolt->v1.x = -10.0f;
    bolt->v1.y = -10.0f;
    bolt->v1.z = 3.0f;
    bolt->v2.x = 10.0f;
    bolt->v2.y = 10.0f;
    bolt->v2.z = 3.0f;
    bolt->v3.x = 0.0;
    bolt->v3.y = 0.0;
    bolt->v3.z = 1.0;
    bolt->f = 0.1f;
    bolt->sampleFractalFloat = 2.0f;
    bolt->pointsNum = 15;
    bolt->pointsNumAllocd = 0;
    bolt->vertexColor = -1;
    bolt->timeElapsed = 0.0;
    bolt->f3 = 0.0;
    bolt->timeStart = 3.402822e38f;
    bolt->deathTime = 3.402822e38f;
    bolt->points = 0;
    GFX_Lightning_SampleZero(bolt);
    return bolt;
}

void GFX_Lightning_Destroy(struct sGfxLightningData* bolt)
{
    if (!bolt || !bolt->pointsNumAllocd)
        return;

    bolt->pointsNumAllocd = 0;
    MEM_Free(bolt->points);
}

// the subdivision needs one point fewer than a power of two, so the count
// is snapped to the nearer of the two candidates around it
static int GFX_l_LightningSnapCount(int count)
{
    int bits;
    int scan;
    int lower;
    int upper;

    bits = 0;
    scan = count;
    do
    {
        scan >>= 1;
        bits++;
    } while (scan);

    lower = (1 << (bits - 1)) - 1;
    upper = (1 << bits) - 1;
    if (upper - count <= count - lower)
        return upper;
    return lower;
}

void GFX_AllocPattern(tGfxLightningData* bolt)
{
    int oldNum;
    int newNum;
    int step;
    int i;

    if (!bolt->pointsNum)
    {
        bolt->pointsNumAllocd = 0;
        MEM_Free(bolt->points);
        return;
    }

    bolt->pointsNum = GFX_l_LightningSnapCount(bolt->pointsNum);
    newNum = bolt->pointsNum;
    oldNum = bolt->pointsNumAllocd;

    if (!oldNum)
    {
        bolt->points = (Vec3*)MEM_p_Alloc(sizeof(Vec3) * newNum);
        bolt->pointsNumAllocd = newNum;
        GFX_Lightning_SampleZero(bolt);
        return;
    }

    if (newNum >= oldNum)
    {
        bolt->points = (Vec3*)MEM_p_Realloc(bolt->points, sizeof(Vec3) * newNum);
        // the spread below reads the samples back out of the buffer this
        // memset has just cleared, so it copies zeros; the fractal pass
        // fills the pattern in again either way
        memset(bolt->points, 0, sizeof(Vec3) * newNum);
        step = (bolt->pointsNum + 1) / (bolt->pointsNumAllocd + 1);
        for (i = 0; i < bolt->pointsNumAllocd; i++)
            bolt->points[step - 1 + i * step] = bolt->points[i];
        bolt->pointsNumAllocd = bolt->pointsNum;
        return;
    }

    step = (oldNum + 1) / (newNum + 1);
    for (i = 0; i < newNum; i++)
        bolt->points[i] = bolt->points[step - 1 + i * step];
    bolt->points = (Vec3*)MEM_p_Realloc(bolt->points, sizeof(Vec3) * newNum);
    bolt->pointsNumAllocd = bolt->pointsNum;
}

void GFX_Lightning_SampleFractal(struct sGfxLightningData* bolt)
{
    Vec3 right;
    double invCount;
    float detail;
    float keep;
    float angleBase;
    float angleSpan;
    float amplitude;
    float prevX;
    float prevY;
    float prevZ;
    int levels;
    int scan;
    int level;
    int span;
    int count;
    int mid;
    int rightIdx;
    int k;

    if (!bolt->pointsNumAllocd)
        return;

    levels = 0;
    scan = bolt->pointsNum;
    if (scan)
    {
        do
        {
            scan >>= 1;
            levels++;
        } while (scan);
    }

    for (level = 0; level < levels; level++)
    {
        span = 1 << (levels - 1 - level);
        count = 1 << level;
        invCount = 1.0 / (double)count;
        detail = (float)(1.0 / (double)span);
        keep = 1.0f - detail;
        angleBase = (float)(invCount * -6.2831855);
        angleSpan = (float)(invCount * 6.2831855 - angleBase);
        amplitude = (float)(invCount * bolt->sampleFractalFloat);
        prevX = 0.0;
        prevY = 0.0;
        prevZ = 0.0;

        for (k = 0; k < count; k++)
        {
            mid = (2 * k + 1) * span - 1;
            rightIdx = (2 * k + 2) * span - 1;
            if (rightIdx == bolt->pointsNum)
            {
                right.x = 0.0;
                right.y = 0.0;
                right.z = 1.0;
            }
            else
            {
                right = bolt->points[rightIdx];
            }

            bolt->points[mid].z = (float)((right.z + prevZ) * 0.5);
            bolt->points[mid].x = (float)((angleSpan * (float)((double)rand() * 0.000030518509)
                                           + angleBase + (right.x + prevX) * 0.5) * detail
                                          + keep * bolt->points[mid].x);
            bolt->points[mid].y = (float)(((right.y + prevY) * 0.5
                                           + (double)rand() * 0.000030518509 * amplitude) * detail
                                          + keep * bolt->points[mid].y);
            prevX = right.x;
            prevY = right.y;
            prevZ = right.z;
        }
    }
}

// where along the pattern a 0..1 fraction of the bolt's length falls, and
// how far into that sample's span
static float GFX_l_LightningSplit(tGfxLightningData* bolt, float fraction, int* sample)
{
    float span;
    int i;

    for (i = 0; i < bolt->pointsNum; i++)
    {
        if (bolt->points[i].z >= (double)fraction)
            break;
    }
    *sample = i;

    if (i)
    {
        if (i == bolt->pointsNum)
            span = (float)(1.0 - bolt->points[i - 1].z);
        else
            span = bolt->points[i].z - bolt->points[i - 1].z;
        return (float)((fraction - bolt->points[i - 1].z) / span);
    }
    if (bolt->pointsNum)
        return (float)(fraction / bolt->points->z);
    return fraction;
}

static int GFX_l_LightningCulled(Vec3* a, Vec3* b, Vec3* plane)
{
    return plane->y * a->y + plane->z * a->z + a->x * plane->x < 0.0
        && plane->y * b->y + plane->z * b->z + b->x * plane->x < 0.0;
}

int GFX_i_Lightning_Render(struct sGfxLightningData* bolt)
{
    tGeoTriangleChunkElem* tris;
    tGeoUVElem* uvs;
    Vec3* verts;
    Vec3* cur;
    Vec3* next;
    Vec3* scratch;
    Vec3* spare;
    Vec3* swap;
    Vec3 startInCam;
    Vec3 endInCam;
    Vec3 axis;
    Vec3 side;
    Vec3 closest;
    Vec3 segment;
    Vec3 width;
    Vec3 ping;
    Vec3 pong;
    int* colors;
    double elapsed;
    float along;
    float fraction;
    int firstQuad;
    int sample;
    int uvIdx;
    int i;

    elapsed = TIM_gf_dt + bolt->timeElapsed;
    bolt->timeElapsed = (float)elapsed;
    if (elapsed > bolt->deathTime)
        return 0;

    if (bolt->pointsNumAllocd != bolt->pointsNum)
        GFX_AllocPattern(bolt);
    if (!bolt->pointsNumAllocd || !GFX_gpst_Current->aiGameObject->distance)
        return 1;

    MATH_TransformVertex(&startInCam, &gpst_DX_stDD->ct.cam.cameraMtx, &bolt->v1);
    MATH_TransformVertex(&endInCam, &gpst_DX_stDD->ct.cam.cameraMtx, &bolt->v2);
    if (startInCam.z < 0.0099999998 && endInCam.z < 0.0099999998)
    {
        --gpst_DX_stDD->rasters[14].unk2;
        return 1;
    }
    if (GFX_l_LightningCulled(&startInCam, &endInCam, &gpst_DX_stDD->ct.cam.planeLeft)
        || GFX_l_LightningCulled(&startInCam, &endInCam, &gpst_DX_stDD->ct.cam.planeRight))
    {
        --gpst_DX_stDD->rasters[14].unk2;
        return 1;
    }

    if (ENG_gb_EngineRunning)
        GFX_Lightning_SampleFractal(bolt);

    if ((bolt->flags & 2) != 0)
    {
        GFX_NeedGeom(2 * bolt->pointsNum + 4, 2 * bolt->pointsNum + 4,
                     2 * bolt->pointsNum + 2, 1);
    }
    else
    {
        GFX_NeedGeom(2 * bolt->pointsNum + 4, 4u, 2 * bolt->pointsNum + 2, 1);
        uvs = GFX_gpst_Geo->UVs;
        uvs[0].u = 0.0;
        uvs[0].v = 0.0;
        uvs[1].u = 1.0;
        uvs[1].v = 0.0;
        uvs[2].u = 0.0;
        uvs[2].v = 1.0;
        uvs[3].u = 1.0;
        uvs[3].v = 1.0;
    }

    MATH_SubstractVec(&axis, &bolt->v2, &bolt->v1);
    MATH_CrossProduct(&side, &bolt->v3, &axis);
    MATH_NormalizeEqualVector(&side);

    MATH_SubstractVec(&closest, &gpst_DX_stDD->ct.cam.objectMtx.Pos, &bolt->v1);
    MATH_SubstractVec(&segment, &bolt->v2, &bolt->v1);
    along = MATH_DotProduct(&segment, &closest) / MATH_VecLenSquared(&segment);
    if (!(along >= 0.0))
        closest = bolt->v1;
    else if (along > 1.0)
        closest = bolt->v2;
    else
        MATH_AddScaleVector(&closest, &bolt->v1, &segment, along);
    MATH_SubstractVec(&closest, &closest, &gpst_DX_stDD->ct.cam.objectMtx.Pos);

    MATH_CrossProduct(&width, &axis, &closest);
    if (MATH_b_NulVector(&width))
    {
        MATH_VecMultScalar(&width, &bolt->v3, bolt->f);
    }
    else
    {
        MATH_NormalizeEqualVector(&width);
        MATH_VecMultScalarWithoutBuffer(&width, bolt->f);
    }

    verts = GFX_gpst_Geo->vertices;
    colors = GFX_gpst_Geo->vertexColors + 1;
    cur = &bolt->v1;
    scratch = &ping;
    spare = &pong;
    for (i = 0; i <= bolt->pointsNum; i++)
    {
        if (i < bolt->pointsNum)
        {
            MATH_AddScaleVector(scratch, &bolt->v1, &axis, bolt->points[i].z);
            MATH_AddScaleVector(scratch, scratch, &side,
                                (float)(cos(bolt->points[i].x) * bolt->points[i].y));
            MATH_AddScaleVector(scratch, scratch, &bolt->v3,
                                (float)(sin(bolt->points[i].x) * bolt->points[i].y));
            next = scratch;
        }
        else
        {
            next = &bolt->v2;
        }

        MATH_VecAddVec(&verts[2 * i], cur, &width);
        MATH_SubstractVec(&verts[2 * i + 1], cur, &width);
        colors[2 * i] = bolt->vertexColor;
        colors[2 * i + 1] = bolt->vertexColor;

        cur = next;
        swap = scratch;
        scratch = spare;
        spare = swap;
    }

    i = bolt->pointsNum + 1;
    MATH_VecAddVec(&verts[2 * i], cur, &width);
    MATH_SubstractVec(&verts[2 * i + 1], cur, &width);
    colors[2 * i] = 0;
    colors[2 * i + 1] = 0;
    colors[0] = 0;
    colors[1] = 0;

    // time advances a second time here, as the original has it
    elapsed = TIM_gf_dt + bolt->timeElapsed;
    bolt->timeElapsed = (float)elapsed;
    firstQuad = 0;
    if (elapsed < bolt->f3)
    {
        fraction = GFX_l_LightningSplit(bolt, (float)(elapsed / bolt->f3), &sample);
        MATH_BlendVector(&verts[2 * sample + 2], &verts[2 * sample],
                         &verts[2 * sample + 2], fraction);
        MATH_BlendVector(&verts[2 * sample + 3], &verts[2 * sample + 1],
                         &verts[2 * sample + 3], fraction);
        GFX_gpst_Geo->triangleChunks->triangleNum = 2 * sample + 2;
    }

    if (bolt->timeElapsed > (double)bolt->timeStart)
    {
        fraction = GFX_l_LightningSplit(bolt,
                       (bolt->timeElapsed - bolt->timeStart)
                       / (bolt->deathTime - bolt->timeStart), &sample);
        MATH_BlendVector(&verts[2 * sample], &verts[2 * sample],
                         &verts[2 * sample + 2], fraction);
        MATH_BlendVector(&verts[2 * sample + 1], &verts[2 * sample + 1],
                         &verts[2 * sample + 3], fraction);
        firstQuad = sample;
        GFX_gpst_Geo->triangleChunks->triangleNum += -2 * sample;
    }

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    if ((bolt->flags & 2) != 0)
    {
        for (i = firstQuad; i <= bolt->pointsNum; i++)
        {
            uvs = GFX_gpst_Geo->UVs;
            if (i)
            {
                uvs[2 * i].u = bolt->points[i - 1].z;
                uvs[2 * i].v = 0.0;
                uvs[2 * i + 1].u = bolt->points[i - 1].z;
                uvs[2 * i + 1].v = 1.0;
            }
            else
            {
                uvs[0].u = 0.0;
                uvs[0].v = 0.0;
                uvs[1].u = 0.0;
                uvs[1].v = 1.0;
            }

            tris[0].v1 = 2 * i;
            tris[0].v2 = 2 * i + 1;
            tris[0].v3 = 2 * i + 3;
            tris[0].uv1 = 2 * i;
            tris[0].uv2 = 2 * i + 1;
            tris[0].uv3 = 2 * i + 3;
            tris[1].v1 = 2 * i + 3;
            tris[1].v2 = 2 * i + 2;
            tris[1].v3 = 2 * i;
            tris[1].uv1 = 2 * i + 3;
            tris[1].uv2 = 2 * i + 2;
            tris[1].uv3 = 2 * i;
            tris += 2;
        }

        uvIdx = 2 * i;
        uvs = GFX_gpst_Geo->UVs;
        uvs[uvIdx].u = 1.0;
        uvs[uvIdx].v = 0.0;
        uvs[uvIdx + 1].u = 1.0;
        uvs[uvIdx + 1].v = 1.0;
    }
    else
    {
        for (i = firstQuad; i <= bolt->pointsNum; i++)
        {
            tris[0].v1 = 2 * i;
            tris[0].v2 = 2 * i + 1;
            tris[0].v3 = 2 * i + 3;
            tris[0].uv1 = 2;
            tris[0].uv2 = 0;
            tris[0].uv3 = 1;
            tris[1].v1 = 2 * i + 3;
            tris[1].v2 = 2 * i + 2;
            tris[1].v3 = 2 * i;
            tris[1].uv1 = 1;
            tris[1].uv2 = 3;
            tris[1].uv3 = 2;
            tris += 2;
        }
    }

    GFX_gpst_Geo->triangleChunks->materialNum = 0;
    GFX_RenderGeom(0x8000900u);
    return 1;
}
