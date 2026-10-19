#include "../memory.h"
#include "../time.h"
#include "gfxripple.h"

struct sGfxRippleData* GFX_Ripple_Create(struct sGameObjectMain* a1)
{
    tGfxRippleData* v1;
    Mtx4x4* v2;
    float v3;
    Mtx4x4* v4;

    v1 = (tGfxRippleData*)MEM_p_Alloc(sizeof(tGfxRippleData));
    memset(v1, 0, sizeof(tGfxRippleData));
    v2 = a1->matrices;
    v3 = v2->Pos.x;
    v1->pos.x = v3;
    v1->pos.y = v2->Pos.y;
    v1->pos.z = v2->Pos.z;
    v4 = OBJ_pst_GetAbsoluteMatrix(a1);
    v1->v1.x = -v4->v2.x;
    v1->v1.y = -v4->v2.y;
    v1->v1.z = -v4->v2.z;
    v1->v2 = v4->v3;
    v1->f2 = 1.0f;
    v1->f4 = 1.0f;
    v1->f1 = 5.0f;
    v1->f3 = 0.2f;
    v1->color2 = 0xFFFFFFFF;
    v1->color1 = 0;
    return v1;
}

void GFX_Ripple_Generate(tGfxRippleData* a1, float a2)
{
    double v2;
    int v3;
    tGfxRippleInnerData* v4;
    int v5;
    tGfxRippleInnerData* v6;
    int v7;

    if (a1->f3 >= 0.0 && (a1->flags & 1) == 0)
    {
        v2 = a2 + a1->fff;
        a1->fff = (float)v2;
        if (v2 > a1->f3)
        {
            do
            {
                v3 = a1->rippleMaxElemID;
                if (v3 == a1->num)
                    break;
                v4 = a1->ptr;
                ++a1->numUnk;
                v5 = v3;
                a1->rippleMaxElemID = v3 + 1;
                v4[v5].f1 = a1->f1;
                v4[v5].pos.x = a1->pos.x;
                v6 = &v4[v3];
                v6->pos.y = a1->pos.y;
                v6->pos.z = a1->pos.z;
                v6->v1Inv.x = -a1->v1.x;
                v6->v1Inv.y = -a1->v1.y;
                v6->v1Inv.z = -a1->v1.z;
                v6->v2 = a1->v2;
                if (a1->f3 == 0.0)
                {
                    a1->f3 = -1.0;
                    break;
                }
                a1->fff = a1->fff - a1->f3;
            } while (a1->fff > (double)a1->f3);
        }
        if (a1->numUnk == a1->num)
        {
            v7 = a1->flags;
            if ((v7 & 2) != 0)
                a1->flags = v7 | 1;
        }
    }
}

void GFX_Ripple_Update(tGfxRippleData* a1, float a2)
{
    tGfxRippleInnerData* v2;
    tGfxRippleInnerData* v3;
    double v4;
    int v5;
    double v6;
    float v7;
    float v8;

    v2 = a1->ptr;
    v3 = &v2[a1->rippleMaxElemID];
    while (v2 < v3)
    {
        v4 = v2->f1 - a2;
        v2->f1 = (float)v4;
        if (v4 >= 0.0)
        {
            ++v2;
            v6 = a2 * a1->f4;
            v7 = (float)(v2[-1].v1Inv.y * v6);
            v8 = (float)(v2[-1].v1Inv.z * v6);
            v2[-1].pos.x = (float)(v6 * v2[-1].v1Inv.x + v2[-1].pos.x);
            v2[-1].pos.y = v2[-1].pos.y + v7;
            v2[-1].pos.z = v2[-1].pos.z + v8;
        }
        else
        {
            v5 = a1->rippleMaxElemID - 1;
            a1->rippleMaxElemID = v5;
            memcpy(v2, &a1->ptr[v5], sizeof(tGfxRippleInnerData));
            --v3;
        }
    }
}

void GFX_Ripple_Alloc(tGfxRippleData* a1, int a2)
{
    int v2;
    tGfxRippleInnerData* v3;
    size_t v4;

    if (a2 > 0)
    {
        v2 = a1->num;
        if (a2 != v2)
        {
            v4 = sizeof(tGfxRippleInnerData) * a2;
            if (v2)
            {
                v3 = (tGfxRippleInnerData*)MEM_p_Realloc(a1->ptr, v4);
                a1->num = a2;
                a1->ptr = v3;
            }
            else
            {
                a1->ptr = (tGfxRippleInnerData*)MEM_p_Alloc(v4);
                a1->num = a2;
            }
        }
    }
    else if (a1->num)
    {
        a1->num = 0;
        MEM_Free(a1->ptr);
    }
}

void GFX_Ripple_Seti(struct sGfxRippleData* a1, int a2, int a3)
{
    if (a1 && a2 == 20100)
    {
        if (a3 == -1)
        {
            a1->flags |= 1u;
        }
        else if (a3 == -2)
        {
            a1->flags |= 2u;
        }
        else
        {
            GFX_Ripple_Alloc(a1, a3);
        }
    }
}

void GFX_Ripple_Setf(struct sGfxRippleData* a1, int a2, float a3)
{
    if (a1)
    {
        if (a2 == 20002)
        {
            a1->f3 = a3;
            a1->fff = a3;
        }
    }
}

void GFX_Ripple_Destroy(struct sGfxRippleData* a1)
{
    if (a1)
    {
        if (a1->num)
        {
            a1->num = 0;
            MEM_Free(a1->ptr);
        }
    }
}

// TODO FIXME átnézni, eléggé át lett írva
BOOL GFX_i_Ripple_Render(struct sGfxRippleData* rd)
{
    int v1; // eax
    tGEOGRO* v3; // ebp
    tGeoTriangleChunkElem* v4; // edi
    tGfxRippleInnerData* v5; // edx
    Vec3* v6; // ecx
    int* v7; // ebx
    Vec3* v8; // esi
    tGfxRippleInnerData* v9; // ebp
    double v10; // st7
    double v11; // rt0
    double v12; // st6
    double v13; // st6
    double v14; // st5
    double v15; // st4
    tGeoTriangleChunkElem* v16; // edi
    double v17; // st4
    unsigned __int16 v18; // dx
    unsigned int v19; // eax
    unsigned int* v20; // ebx
    int v21; // esi
    float v22; // [esp+0h] [ebp-54h]
    tGfxRippleInnerData* v23; // [esp+14h] [ebp-40h]
    float v24; // [esp+18h] [ebp-3Ch]
    tGfxRippleInnerData* v25; // [esp+1Ch] [ebp-38h]
    float v26; // [esp+20h] [ebp-34h]
    Vec3* v27; // [esp+20h] [ebp-34h]
    float v28; // [esp+28h] [ebp-2Ch]
    float v29; // [esp+2Ch] [ebp-28h]
    float v30; // [esp+38h] [ebp-1Ch]
    float v31; // [esp+44h] [ebp-10h]
    float v32; // [esp+50h] [ebp-4h]

    GFX_Ripple_Generate(rd, TIM_gf_dt);
    GFX_Ripple_Update(rd, TIM_gf_dt);
    v1 = rd->rippleMaxElemID;
    if (!v1)
        return (rd->flags & 1) == 0;
    if ((rd->flags & 2) != 0 || GFX_gpst_Current->aiGameObject->distance)
    {
        GFX_NeedGeom(4 * v1, 4u, 2 * v1, 1);
        GFX_gpst_Geo->UVs->u = 0.0;
        GFX_gpst_Geo->UVs->v = 0.0;
        GFX_gpst_Geo->UVs[1].u = 1.0;
        GFX_gpst_Geo->UVs[1].v = 0.0;
        GFX_gpst_Geo->UVs[2].u = 1.0;
        GFX_gpst_Geo->UVs[2].v = 1.0;
        GFX_gpst_Geo->UVs[3].u = 0.0;
        GFX_gpst_Geo->UVs[3].v = 1.0;
        v3 = GFX_gpst_Geo;
        v4 = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
        v5 = rd->ptr;
        v6 = GFX_gpst_Geo->vertices;
        v25 = &v5[rd->rippleMaxElemID];
        v7 = GFX_gpst_Geo->vertexColors + 1;
        v23 = v5;
        if (rd->f1 == 0.0)
            v24 = 0.0;
        else
            (*(uint32_t *)&v24) = MATH_gl_TableInverse[((*(int32_t *)&(rd->f1)) >> 13) & 0x3FF]  - ((*(uint32_t *)&(rd->f1)) & 0xFF800000)  + 0x7E800000;
        if (v5 < v25)
        {
            v8 = v6;
            v9 = v5;
            while (1)
            {
                v10 = (rd->f1 - v9->f1) * rd->f2 + rd->f555;
                v28 = (float)(v9->v2.y * v10);
                v11 = v10 * v9->v2.x;
                v12 = v10 * v9->v2.z;
                v29 = (float)v12;
                v13 = v12 * v9->v1Inv.y - v28 * v9->v1Inv.z;
                v14 = v11 * v9->v1Inv.z - v29 * v9->v1Inv.x;
                v30 = (float)(v28 * v9->v1Inv.x - v11 * v9->v1Inv.y);
                v31 = (float)(v29 * 0.5);
                v6->x = (float)(v5->pos.x - v11 * 0.5);
                v8->y = (float)(v9->pos.y - v28 * 0.5);
                v8->z = v9->pos.z - v31;
                v32 = (float)(v30 * 0.5);
                v26 = (float)(v13 * 0.5 + v6->x);
                v6->x = v26;
                v8->y = (float)(v14 * 0.5 + v8->y);
                v15 = v32 + v8->z;
                v16 = v4 + 1;
                v8->z = (float)v15;
                v17 = v26;
                v27 = v6 + 4;
                v8[1].x = (float)(v17 + v11);
                v8[1].y = v28 + v8->y;
                v8[1].z = v29 + v8->z;
                v8[2].x = (float)(v8[1].x - v13);
                v8[2].y = (float)(v8[1].y - v14);
                v8[2].z = v8[1].z - v30;
                v8[3].x = (float)(v8[2].x - v11);
                v8[3].y = v8[2].y - v28;
                v8[3].z = v8[2].z - v29;
                v8 += 4;
                v18 = ((char*)v6 - ((char*)(GFX_gpst_Geo->vertices))) / sizeof(Vec3);
                v16[-1].v1 = v18;
                v16[-1].uv1 = 0;
                v16[-1].uv2 = 1;
                v16[-1].v2 = v18 + 1;
                v16[-1].uv3 = 2;
                v16[-1].v3 = v18 + 2;
                v16->v1 = v18 + 2;
                v16->v2 = v18 + 3;
                v16->v3 = v18;
                v16->uv1 = 2;
                v16->uv2 = 3;
                v16->uv3 = 0;
                v4 = v16 + 1;
                v22 = v24 * v9->f1;
                v19 = LIGHT_ul_Interpol2Colors(rd->color1, rd->color2, v22);
                *v7 = v19;
                v20 = (unsigned int*)(v7 + 1);
                *v20++ = v19;
                *v20++ = v19;
                *v20 = v19;
                v7 = (int*)(v20 + 1);
                v9++;
                if (++v23 >= v25)
                    break;
                v5 = v23;
                v6 = v27;
            }
            v3 = GFX_gpst_Geo;
        }
        v3->triangleChunks->materialNum = 0;
        v21 = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v21 & ~0x8000900u;
        GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
        gpst_DX_stDD->drawMaskAllowedStuffCurrent = v21;
    }
    return 1;
}