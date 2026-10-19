#include "../memory.h"
#include "../time.h"
#include "../loading.h"
#include "otcmodifier.h"

void GEO_ModifierOnduleTonCorps_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tOtcMdfStruct* result;

    result = (tOtcMdfStruct*)MEM_p_Alloc(sizeof(tOtcMdfStruct));
    a2->mdfPrivateDataPtr = result;
    result->flags = 0;
    if (a3)
    {
        memcpy(a2->mdfPrivateDataPtr, a3, sizeof(tOtcMdfStruct));
    }
    else
    {
        result->offset = 0.0;
        result->intensity = 1.0;
        result->timeMultiplier = 0.1f;
        result->speed = 1.0;
    }
}

void GEO_ModifierOnduleTonCorps_Destroy(struct sModifier* a1)
{
    MEM_Free(a1->mdfPrivateDataPtr);
}

void GEO_ModifierOnduleTonCorps_UnApply(struct sModifier* mdf, struct sGEOGRO* geogro) {
}

void GEO_ModifierOnduleTonCorps_Apply(struct sModifier* mdf, struct sGEOGRO* geogro)
{
    tOtcMdfStruct* v2;
    tGdi_DisplayData* v3;
    int v4;
    tGameObjectMain* v5;
    Vec3* v6;
    Vec3* v7;
    int v8;
    int* v9;
    int* v10;
    uint8_t* v11;
    uint8_t* v12;
    uint8_t* v13;
    uint8_t* v14;
    uint8_t* v15;
    uint8_t* v16;
    uint8_t* v17;
    uint8_t* v18;
    uint8_t* v19;
    uint8_t* v20;
    uint8_t* v21;
    uint8_t* v22;
    double v27;
    float a1; 
    float a1a; 
    float a1b; 
    float a1c; 
    float a1d; 
    float a1e; 
    float a1f; 
    float a1g; 
    float a1h; 
    float a1i; 
    float a1j; 
    float a1k; 
    float a1l; 
    float a1m; 
    float a1n; 
    float a1o; 
    float a1p; 
    float a1q; 
    float a1r; 
    float a1s; 
    float a1t; 
    float a1u; 
    float a1v; 
    float a1w; 
    float a1y; 
    float a1z; 
    float a1ba;
    float a1bb;
    float a1bc;
    float a1bd;
    float a1be;
    float a1bf;
    float mdfb;
    float mdfc;
    float mdfd;
    float mdfe;
    float mdff;
    float mdfg;
    float mdfh;
    float mdfi;
    float mdfj;
    float mdfk;
    float mdfl;
    float mdfm;
    float mdfn;
    float mdfa;
    float geogroa;
    float geogrob;
    float geogroc;
    float geogrod;
    float geogroe;
    float geogrof;
    float geogrog;

    v2 = (tOtcMdfStruct*)mdf->mdfPrivateDataPtr;
    v3 = gpst_DX_stDD;
    v4 = gpst_DX_stDD->flags;
    if ((v4 & 0x400) == 0)
    {
        gpst_DX_stDD->flags = v4 | 0x400;
        gpst_DX_stDD->verticesCurrent = (Vec3*)gpst_DX_stDD->bigBuffer->data4;
        memcpy(gpst_DX_stDD->bigBuffer->data4, geogro->vertices, sizeof(Vec3) * geogro->vertexNum);
        v3 = gpst_DX_stDD;
    }
    v5 = v3->currTargetGao;
    v6 = (Vec3*)v3->bigBuffer->data4;
    v7 = &v6[geogro->vertexNum];
    v8 = v2->flags;
    v9 = ((tGameObjectVisualObject*)(v5->basic->actorObjectOrVisualObject))->RLIVertexColors;
    if ((v2->flags & 0x10) != 0 && v9)
    {
        v10 = v9 + 1;
        if ((v8 & 0x20) != 0)
        {
            if ((v8 & 1) != 0)
            {
                if ((v8 & 8) != 0)
                {
                    if (v6 < v7)
                    {
                        v11 = (uint8_t*)v10 + 3;   // eloszor a cast utána a 3, tehát ez azt jelenti hogy a 3. bájtot nézzük
                        do
                        {
                            mdfb = (float)((1.0 - (double)*v11 * 0.00390625) * v2->intensity);
                            a1 = v6->y * v2->speed + v2->offset;
                            geogroa = (float)(MATH_GetSin(a1) * mdfb + v6->x);
                            v6->x = geogroa;
                            a1a = v6->z * v2->speed + v2->offset;
                            ++v6;
                            v11 += 4;
                            v6[-1].x = (float)(MATH_GetSin(a1a) * mdfb + geogroa);
                        } while (v6 < v7);
                    }
                }
                else if (v6 < v7)
                {
                    v12 = (uint8_t*)v10 + 3;
                    do
                    {
                        mdfc = (float)((1.0 - (double)*v12 * 0.00390625) * v2->intensity);
                        a1b = v2->speed * v6->x + v2->offset;
                        v6->y = (float)(MATH_GetSin(a1b) * mdfc + v6->y);
                        a1c = v2->speed * v6->x + v2->offset;
                        ++v6;
                        v12 += 4;
                        v6[-1].z = MATH_GetCos(a1c) * mdfc + v6[-1].z;
                    } while (v6 < v7);
                }
            }
            else if ((v8 & 2) != 0)
            {
                if ((v8 & 8) != 0)
                {
                    if (v6 < v7)
                    {
                        v13 = (uint8_t*)v10 + 3;
                        do
                        {
                            mdfd = (float)((1.0 - (double)*v13 * 0.00390625) * v2->intensity);
                            a1d = v2->speed * v6->x + v2->offset;
                            geogrob = (float)(MATH_GetSin(a1d) * mdfd + v6->y);
                            v6->y = geogrob;
                            a1e = v6->z * v2->speed + v2->offset;
                            ++v6;
                            v13 += 4;
                            v6[-1].y = (float)(MATH_GetSin(a1e) * mdfd + geogrob);
                        } while (v6 < v7);
                    }
                }
                else if (v6 < v7)
                {
                    v14 = (uint8_t*)v10 + 3;
                    do
                    {
                        mdfe = (float)((1.0 - (double)*v14 * 0.00390625) * v2->intensity);
                        a1f = v6->y * v2->speed + v2->offset;
                        v6->x = (float)(MATH_GetSin(a1f) * mdfe + v6->x);
                        a1g = v6->y * v2->speed + v2->offset;
                        ++v6;
                        v14 += 4;
                        v6[-1].z = MATH_GetCos(a1g) * mdfe + v6[-1].z;
                    } while (v6 < v7);
                }
            }
            else if ((v8 & 4) != 0)
            {
                if ((v8 & 8) != 0)
                {
                    if (v6 < v7)
                    {
                        v15 = (uint8_t*)v10 + 3;
                        do
                        {
                            mdff = (float)((1.0 - (double)*v15 * 0.00390625) * v2->intensity);
                            a1h = v2->speed * v6->x + v2->offset;
                            geogroc = (float)(MATH_GetSin(a1h) * mdff + v6->z);
                            v6->z = geogroc;
                            a1i = v6->y * v2->speed + v2->offset;
                            ++v6;
                            v15 += 4;
                            v6[-1].z = (float)(MATH_GetSin(a1i) * mdff + geogroc);
                        } while (v6 < v7);
                    }
                }
                else if (v6 < v7)
                {
                    v16 = (uint8_t*)v10 + 3;
                    do
                    {
                        mdfg = (float)((1.0 - (double)*v16 * 0.00390625) * v2->intensity);
                        a1j = v6->z * v2->speed + v2->offset;
                        v6->x = (float)(MATH_GetSin(a1j) * mdfg + v6->x);
                        a1k = v6->z * v2->speed + v2->offset;
                        ++v6;
                        v16 += 4;
                        v6[-1].y = MATH_GetCos(a1k) * mdfg + v6[-1].y;
                    } while (v6 < v7);
                }
            }
        }
        else if ((v8 & 1) != 0)
        {
            if ((v8 & 8) != 0)
            {
                if (v6 < v7)
                {
                    v17 = (uint8_t*)v10 + 3;
                    do
                    {
                        mdfh = (float)((double)*v17 * v2->intensity * 0.00390625);
                        a1l = v6->y * v2->speed + v2->offset;
                        geogrod = (float)(MATH_GetSin(a1l) * mdfh + v6->x);
                        v6->x = geogrod;
                        a1m = v6->z * v2->speed + v2->offset;
                        ++v6;
                        v17 += 4;
                        v6[-1].x = (MATH_GetSin(a1m) * mdfh + geogrod);
                    } while (v6 < v7);
                }
            }
            else if (v6 < v7)
            {
                v18 = (uint8_t*)v10 + 3;
                do
                {
                    mdfi = (float)((double)*v18 * v2->intensity * 0.00390625);
                    a1n = v2->speed * v6->x + v2->offset;
                    v6->y = (float)(MATH_GetSin(a1n) * mdfi + v6->y);
                    a1o = v2->speed * v6->x + v2->offset;
                    ++v6;
                    v18 += 4;
                    v6[-1].z = MATH_GetCos(a1o) * mdfi + v6[-1].z;
                } while (v6 < v7);
            }
        }
        else if ((v8 & 2) != 0)
        {
            if ((v8 & 8) != 0)
            {
                if (v6 < v7)
                {
                    v19 = (uint8_t*)v10 + 3;
                    do
                    {
                        mdfj = (float)((double)*v19 * v2->intensity * 0.00390625);
                        a1p = v2->speed * v6->x + v2->offset;
                        geogroe = (float)(MATH_GetSin(a1p) * mdfj + v6->y);
                        v6->y = geogroe;
                        a1q = v6->z * v2->speed + v2->offset;
                        ++v6;
                        v19 += 4;
                        v6[-1].y = (float)(MATH_GetSin(a1q) * mdfj + geogroe);
                    } while (v6 < v7);
                }
            }
            else if (v6 < v7)
            {
                v20 = (uint8_t*)v10 + 3;
                do
                {
                    mdfk = (float)((double)*v20 * v2->intensity * 0.00390625);
                    a1r = v6->y * v2->speed + v2->offset;
                    v6->x = (float)(MATH_GetSin(a1r) * mdfk + v6->x);
                    a1s = v6->y * v2->speed + v2->offset;
                    ++v6;
                    v20 += 4;
                    v6[-1].z = MATH_GetCos(a1s) * mdfk + v6[-1].z;
                } while (v6 < v7);
            }
        }
        else if ((v8 & 4) != 0)
        {
            if ((v8 & 8) != 0)
            {
                if (v6 < v7)
                {
                    v21 = (uint8_t*)v10 + 3;
                    do
                    {
                        mdfl = (float)((double)*v21 * v2->intensity * 0.00390625);
                        a1t = v2->speed * v6->x + v2->offset;
                        geogrof = (float)(MATH_GetSin(a1t) * mdfl + v6->z);
                        v6->z = geogrof;
                        a1u = v6->y * v2->speed + v2->offset;
                        ++v6;
                        v21 += 4;
                        v6[-1].z = (float)(MATH_GetSin(a1u) * mdfl + geogrof);
                    } while (v6 < v7);
                }
            }
            else if (v6 < v7)
            {
                v22 = (uint8_t*)v10 + 3;
                do
                {
                    mdfm = (float)((double)*v22 * v2->intensity * 0.00390625);
                    a1v = v6->z * v2->speed + v2->offset;
                    v6->x = (float)(MATH_GetSin(a1v) * mdfm + v6->x);
                    a1w = v6->z * v2->speed + v2->offset;
                    ++v6;
                    v22 += 4;
                    v6[-1].y = MATH_GetCos(a1w) * mdfm + v6[-1].y;
                } while (v6 < v7);
            }
        }
    }
    else if ((v8 & 1) != 0)
    {
        if ((v8 & 8) != 0)
        {
            for (; v6 < v7;)
            {
                v6->x = (float)(MATH_GetSin(v6->y * v2->speed + v2->offset) * v2->intensity + v6->x);
                v6->x = (float)(MATH_GetSin(v6->z * v2->speed + v2->offset) * v2->intensity + v6->x);
                ++v6;
            }
        }
        else
        {
            for (; v6 < v7;)
            {
                v6->y = (float)(MATH_GetSin(v2->speed * v6->x + v2->offset) * v2->intensity + v6->y);
                v6->z = MATH_GetCos(v2->speed * v6->x + v2->offset) * v2->intensity + v6->z;
                ++v6;
            }
        }
    }
    else if ((v8 & 2) != 0)
    {
        if ((v8 & 8) != 0)
        {
            for (; v6 < v7; v6[-1].y = (float)(MATH_GetSin(a1z) * v2->intensity + mdfn))
            {
                a1y = v2->speed * v6->x + v2->offset;
                mdfn = (float)(MATH_GetSin(a1y) * v2->intensity + v6->y);
                v6->y = mdfn;
                a1z = v6->z * v2->speed + v2->offset;
                ++v6;
            }
        }
        else
        {
            for (; v6 < v7; v6[-1].z = MATH_GetCos(a1bb) * v2->intensity + v6[-1].z)
            {
                a1ba = v6->y * v2->speed + v2->offset;
                v6->x = (float)(MATH_GetSin(a1ba) * v2->intensity + v6->x);
                a1bb = v6->y * v2->speed + v2->offset;
                ++v6;
            }
        }
    }
    else if ((v8 & 4) != 0)
    {
        if ((v8 & 8) != 0)
        {
            v27 = MATH_GetSin(v2->offset);
            for (mdfa = (float)(v27 * v2->intensity + v27 * v2->intensity);
                v6 < v7;
                v6[-1].z = (float)((MATH_GetSin(a1bd) + geogrog) * mdfa + v6[-1].z))
            {
                a1bc = v6->y * v2->speed;
                geogrog = (float)(MATH_GetSin(a1bc));
                a1bd = v2->speed * v6->x;
                ++v6;
            }
        }
        else
        {
            for (; v6 < v7; v6[-1].y = MATH_GetCos(a1bf) * v2->intensity + v6[-1].y)
            {
                a1be = v6->z * v2->speed + v2->offset;
                v6->x = (float)(MATH_GetSin(a1be) * v2->intensity + v6->x);
                a1bf = v6->z * v2->speed + v2->offset;
                ++v6;
            }
        }
    }
    v2->offset = (float)(TIM_gf_dt * v2->timeMultiplier * 10.0 + v2->offset);
}

int GEO_ul_ModifierOnduleTonCorps_Load(tModifier* a1, char* a2)
{
    tOtcMdfStruct* v2;
    char* v3;
    char* v5;

    v2 = (tOtcMdfStruct*)a1->mdfPrivateDataPtr;
    v3 = a2;
    v5 = a2;
    LOA_ReadLong(&v5, 0, 2);
    v2->flags = LOA_ReadLong(&v5, 0, 1);
    v2->offset = LOA_ReadFloat(&v5, 0, 1);
    v2->intensity = LOA_ReadFloat(&v5, 0, 1);
    v2->speed = LOA_ReadFloat(&v5, 0, 1);
    v2->timeMultiplier = LOA_ReadFloat(&v5, 0, 1);
    LOA_ReadLong(&v5, 0, 2);
    v2->offset = 0.0;
    return v5 - v3;
}