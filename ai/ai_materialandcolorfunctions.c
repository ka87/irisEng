#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
#include "../materialanddraw.h"
#include "ai_materialandcolorfunctions.h"

tGEOMAT* MAT_ValidateSUBMAT_C(tGameObjectMain* a1, unsigned int a2)
{
    tGEOMAT* v2;
    tGEOMAT* v4;
    unsigned int a1a;

    v4 = 0;
    if ((a2 & 0xFF000000) != 0)
    {
        a1a = BIG_ul_SearchKeyToPos(a2);
        if (a1a != -1)
        {
            v4 = (tGEOMAT*)LOA_ul_SearchAddress(a1a);
            if (v4 == (tGEOMAT*)-1)
                v4 = 0;
        }
    }
    else if ((a1->flags & 0x4000) != 0 
        && a1->basic->actorObjectOrVisualObject
        && ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT)
    {
        if (((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT->intf->id == 4)
        {
            v2 = (tGEOMAT*)((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
            v4 = v2->multiMaterialPtrIfType4ThenDiffuseColor[a2 % v2->multiMaterialNumIfType4ThenAmbiantColor];
        }
        else
        {
            v4 = (tGEOMAT*)((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->GROMAT;
        }
    }
    return v4;
}

void MAT_GetColorPtr(tGEOMAT* a1, void** a2, int a3, int a4)
{
    tGEOMAT* v4;
    int v5;
    int v6;

    v4 = a1;
    *a2 = 0;
    if (a1->intf->id == 4)
    {
        v5 = a1->multiMaterialNumIfType4ThenAmbiantColor - 1;
        if (a3 <= v5)
            v5 = a3;
        v4 = a1->multiMaterialPtrIfType4ThenDiffuseColor[v5];
    }
    v6 = v4->intf->id;
    if (v6 != 4 && (v6 == 3 || v6 == 5))
    {
        if (a4)
        {
            if (a4 == 1)                            // constant
            {
                *a2 = &v4->solidVertexColor;
            }
            else if (a4 == 2)                       // ambiant
            {
                *a2 = &v4->multiMaterialNumIfType4ThenAmbiantColor;
            }
        }
        else
        {
            *a2 = &v4->multiMaterialPtrIfType4ThenDiffuseColor;// diffuse
        }
    }
}

tMultiTextureElemStruct** MAT_GetMtxLvlPtr(tGEOMAT* a1, tMultiTextureElemStruct** a2, int multimatId, int multiMatElemId)
{
    tMultiTextureElemStruct** result;
    tGEOMAT* v5;
    int v6;
    tMultiTextureElemStruct* v7;
    tMultiTextureElemStruct* v9;

    result = a2;
    v5 = a1;
    *a2 = 0;
    if (a1->intf->id == 4)
    {
        v6 = a1->multiMaterialNumIfType4ThenAmbiantColor;
        if (multimatId <= v6)
            v6 = multimatId;
        v5 = a1->multiMaterialPtrIfType4ThenDiffuseColor[v6];
    }
    if (v5->intf->id == 5)
    {
        v7 = (tMultiTextureElemStruct*)v5->idxOrPtr;
        *a2 = v7;
        if (v7)
        {
            do
            {
                if (!multiMatElemId)
                    break;
                v9 = (*a2)->nextTextureKey;
                --multiMatElemId;
                *a2 = v9;
            } while (v9);
        }
    }
    return result;
}

void AI_EvalFunc_MATSetConstant_C(struct sGameObjectMain* a1, int a2, int a3)
{
    int* v3;
    tGEOMAT* v4;

    v4 = MAT_ValidateSUBMAT_C(a1, a3);
    v3 = 0;
    if (v4)
        MAT_GetColorPtr(v4, (void**)&v3, 0, 1);
    if (v3)
        *v3 = a2;
}

tAINode* AI_EvalFunc_MATSetConstant(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_MATSetConstant_C(v2, a2, a3);
    return a1 + 1;
}

int AI_EvalFunc_MATGetConstant_C(struct sGameObjectMain* a1, unsigned int a2)
{
    int v3;
    int* v4;
    tGEOMAT* v5;

    v5 = MAT_ValidateSUBMAT_C(a1, a2);
    v3 = 0;
    v4 = 0;
    if (v5)
        MAT_GetColorPtr(v5, (void**)&v4, 0, 1);
    if (v4)
        v3 = *v4;
    return v3;
}

tAINode* AI_EvalFunc_MATGetConstant(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2; 

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATGetConstant_C(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_MATSetDiffuse_C(struct sGameObjectMain* a1, int* arg4, unsigned int a2)
{
    int** v3;
    tGEOMAT* v4;

    v4 = MAT_ValidateSUBMAT_C(a1, a2);
    v3 = 0;
    if (v4)
        MAT_GetColorPtr(v4, (void**)&v3, 0, 0);
    if (v3)
        *v3 = arg4;
}

tAINode* AI_EvalFunc_MATSetDiffuse(tAINode* a1)
{
    tGameObjectMain* v2;
    int* v3;
    int v4;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v3 = (int*)AI_GetIntFromStack();
    AI_EvalFunc_MATSetDiffuse_C(v2, v3, v4);
    return a1 + 1;
}

int AI_EvalFunc_MATGetDiffuse_C(struct sGameObjectMain* a1, unsigned int a2)
{
    int v3;
    int* v4;
    tGEOMAT* v5;

    v5 = MAT_ValidateSUBMAT_C(a1, a2);
    v3 = 0;
    v4 = 0;
    if (v5)
        MAT_GetColorPtr(v5, (void**)&v4, 0, 0);
    if (v4)
        v3 = *v4;
    return v3;
}

tAINode* AI_EvalFunc_MATGetDiffuse(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATGetDiffuse_C(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATSetAmbient(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int* v4; 
    int** a2;
    tGEOMAT* v6; 

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v6 = MAT_ValidateSUBMAT_C(v3, v1);
    v4 = (int*)AI_GetIntFromStack();
    a2 = 0;
    if (v6)
        MAT_GetColorPtr(v6, (void**)&a2, 0, 2);
    if (a2)
        *a2 = v4;
    return a1 + 1;
}

tAINode* AI_EvalFunc_MATGetAmbient(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int v4;
    int* a2;
    tGEOMAT* v6;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v6 = MAT_ValidateSUBMAT_C(v3, v1);
    v4 = 0;
    a2 = 0;
    if (v6)
        MAT_GetColorPtr(v6, (void**)&a2, 0, 2);
    if (a2)
        v4 = *a2;
    AI_PutIntToStack(v4);
    return a1 + 1;
}

void AI_EvalFunc_MATEnableMltxtLvl_C(struct sGameObjectMain* a1, unsigned int a2, int multiMatElemId, int a4)
{
    unsigned int v4;
    tMultiTextureElemStruct* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
        MAT_GetMtxLvlPtr(v6, &v5, 0, multiMatElemId);
    if (v5)
    {
        v4 = v5->subTextureFlags & 0xFFF;
        if (a4)
            v4 = v5->subTextureFlags & 0x7FF;
        else {
            int v4a = v4;
            v4 &= 0xffff0000;
            v4 |= ((v4a | 0x800) & 0xffff);
        }
        v5->subTextureFlags = v4 & 0xFFF | v5->subTextureFlags & 0xFFFFF000;
    }
}

tAINode* AI_EvalFunc_MATEnableMltxtLvl(tAINode* a1)
{
    tGameObjectMain* v2;
    int a4;
    int multiMatElemId;
    int a2;

    a4 = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MATEnableMltxtLvl_C(v2, a2, multiMatElemId, a4);
    return a1 + 1;
}

void AI_EvalFunc_MATEnableMltxtLvlMT_C(struct sGameObjectMain* a1, unsigned int a2, int a3, int a4, int a5)
{
    unsigned int v5;
    tMultiTextureElemStruct* v6;
    tGEOMAT* v7;

    v6 = 0;
    v7 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v7)
        MAT_GetMtxLvlPtr(v7, &v6, a4, a3);
    if (v6)
    {
        v5 = v6->subTextureFlags & 0xFFF;
        if (a5)
            v5 = v6->subTextureFlags & 0x7FF;
        else {
            int v5a = v5;
            v5 &= 0xffff0000;
            v5 |= ((v5a | 0x800) & 0xffff);
        }
        v6->subTextureFlags = v5 & 0xFFF | v6->subTextureFlags & 0xFFFFF000;
    }
}

tAINode* AI_EvalFunc_MATEnableMltxtLvlMT(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a4;
    int a5;
    int a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_MATEnableMltxtLvlMT_C(v3, v1, a3, a4, a5);
    return a1 + 1;
}

int AI_EvalFunc_GetLocalAlpha_C(struct sGameObjectMain* a1, unsigned int a2, int multiMatElemId)
{
    int v4;
    tMultiTextureElemStruct* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
        MAT_GetMtxLvlPtr(v6, &v5, 0, multiMatElemId);
    if (v5)
        v4 = (v5->phongOrPlanarParam & 0xF800) >> 11;
    else
        v4 = 0;
    return 8 * v4;
}

tAINode* AI_EvalFunc_GetLocalAlpha(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;
    int multiMatElemId;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    multiMatElemId = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_GetLocalAlpha_C(v3, a2, multiMatElemId);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_SetLocalAlpha_C(struct sGameObjectMain* a1, unsigned int a2, int a3, int a4)
{
    tMultiTextureElemStruct* v4;
    tGEOMAT* v5;

    v4 = 0;
    v5 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v5)
        MAT_GetMtxLvlPtr(v5, &v4, 0, a3);
    if (v4)
        v4->phongOrPlanarParam = ((uint16_t)(a4 >> 3) << 11) | v4->phongOrPlanarParam & 0x7FF;
}

tAINode* AI_EvalFunc_SetLocalAlpha(tAINode* a1)
{
    tGameObjectMain* v2;
    int a4;
    int a2;
    int a3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_SetLocalAlpha_C(v2, a2, a3, a4);
    return a1 + 1;
}

unsigned int AI_EvalFunc_GetALPHATRESH_C(struct sGameObjectMain* a1, unsigned int a2, int a3)
{
    unsigned int v4;
    tMultiTextureElemStruct* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
        MAT_GetMtxLvlPtr(v6, &v5, 0, a3);
    if (v5)
        v4 = 4 * ((v5->subTextureFlags & 0x3F000000) >> 24);
    else
        v4 = 0;
    return v4;
}

tAINode* AI_EvalFunc_GetALPHATRESH(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;
    int a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_GetALPHATRESH_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_SetALPHATRESH_C(struct sGameObjectMain* a1, unsigned int a2, int a3, int a4)
{
    tMultiTextureElemStruct* v4;
    tGEOMAT* v5;

    v4 = 0;
    v5 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v5)
        MAT_GetMtxLvlPtr(v5, &v4, 0, a3);
    if (v4)
        v4->subTextureFlags = (a4 >> 2 << 24) & 0x3F000000 | v4->subTextureFlags & 0xC0FFFFFF;
}

tAINode* AI_EvalFunc_SetALPHATRESH(tAINode* a1)
{
    tGameObjectMain* v2;
    int a4;
    int a2;
    int a3;

    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SetALPHATRESH_C(v2, a2, a3, a4);
    return a1 + 1;
}

void AI_EvalFunc_SetFlag_C(struct sGameObjectMain* a1, unsigned int a2, int multiMatElemId, int a4, __int16 a5)
{
    tMultiTextureElemStruct* v5;
    tGEOMAT* v6;

    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
    {
        if (multiMatElemId == -1)
        {
            v6->negativeZeroOrOneUnknown |= a4;
            v6->negativeZeroOrOneUnknown &= ~a5;
        }
        else if (multiMatElemId < 1000)
        {
            MAT_GetMtxLvlPtr(v6, &v5, 0, multiMatElemId);
            if (v5)
            {
                v5->subTextureFlags |= a4;
                v5->subTextureFlags &= ~a5;
            }
        }
        else
        {
            MAT_GetMtxLvlPtr(v6, &v5, 0, multiMatElemId - 1000);
            if (v5)
            {
                v5->phongOrPlanarParam |= a4;
                v5->phongOrPlanarParam &= ~a5;
            }
        }
    }
}

tAINode* AI_EvalFunc_SetFlag(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    __int16 a5;
    int a4; 
    int multiMatElemId;

    a5 = AI_GetIntFromStack();
    a4 = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SetFlag_C(v3, v1, multiMatElemId, a4, a5);
    return a1 + 1;
}

int AI_EvalFunc_GetFlag_C(struct sGameObjectMain* gao, unsigned int subMatID, int multiMatElemID)
{
    int result;
    tMultiTextureElemStruct* v4;
    tGEOMAT* v5;

    v5 = MAT_ValidateSUBMAT_C(gao, subMatID);
    if (!v5)
        goto LABEL_9;
    if (multiMatElemID == -1)
        return v5->negativeZeroOrOneUnknown;
    if (multiMatElemID >= 1000)
    {
        MAT_GetMtxLvlPtr(v5, &v4, 0, multiMatElemID - 1000);
        if (v4)
            return v4->phongOrPlanarParam;
    }
    MAT_GetMtxLvlPtr(v5, &v4, 0, multiMatElemID);
    if (v4)
        result = v4->subTextureFlags;
    else
        LABEL_9:
    result = 0;
    return result;
}

tAINode* AI_EvalFunc_GetFlag(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int v4;
    int v5;

    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_GetFlag_C(v3, v4, v5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

struct sTexProceduralStruct* AI_EvalFunc_MATGetPro_C(struct sGameObjectMain* a1, unsigned int a2, int multiMatElemId)
{
    tMultiTextureElemStruct* v4;
    tTexProceduralStruct* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
    {
        MAT_GetMtxLvlPtr(v6, &v4, 0, multiMatElemId);
        if (v4)
            v5 = TEX_pst_Procedural_Get(v4->texElemIdx);
    }
    return v5;
}

tAINode* AI_EvalFunc_MATGetPro(tAINode* a1)
{
    tTexProceduralStruct* v1;
    tGameObjectMain* v3;
    int a2;
    int multiMatElemId;

    multiMatElemId = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_MATGetPro_C(v3, a2, multiMatElemId);
    AI_PutIntToStack((int)v1);
    return a1 + 1;
}

void AI_EvalFunc_MATProSeti_C(struct sTexProceduralStruct* a1, int a2, int param1)
{
    float v3; 

    if (a1)
    {
        if (a1->proceduralIntf->id == 4)
        {
            if (a2)
            {
                if (a2 == 1)
                {
                    ((tTexProcPhoto*)(a1->proceduralTexPtr))->b3 = 1;
                }
                else if (a2 == 2)
                {
                    TEXPRO_Photo_SetMapGao(a1, (tGameObjectMain*)param1);
                }
                else if (a2 == 3 && ((tTexProcPhoto*)(a1->proceduralTexPtr))->x == 0x10000000)
                {
                    ((tTexProcPhoto*)(a1->proceduralTexPtr))->x = 128;
                    v3 = (float)param1;
                    ((tTexProcPhoto*)(a1->proceduralTexPtr))->f1 = (float)(v3 / 1000.0);
                    ((tTexProcPhoto*)(a1->proceduralTexPtr))->remainingTime = ((tTexProcPhoto*)(a1->proceduralTexPtr))->f1;
                }
            }
            else
            {
                ((tTexProcPhoto*)(a1->proceduralTexPtr))->b2 = param1;
            }
        }
        else if (a1->proceduralIntf->id == 3)
        {
            if (a2)
            {
                switch (a2)
                {
                case 1:
                    if (param1)
                        ((tTexProcMpeg*)(a1->proceduralTexPtr))->flags |= 2u;
                    else
                        ((tTexProcMpeg*)(a1->proceduralTexPtr))->flags &= ~2u;
                    break;
                case 10:
                    ((tTexProcMpeg*)(a1->proceduralTexPtr))->flags = (((tTexProcMpeg*)(a1->proceduralTexPtr))->flags & ~0x00ff0000) | (param1 << 16); //BYTE2(a1->proceduralTexPtr->flags = param1;
                    break;
                case 11:
                    TEX_Procedural_UpdateWithAnother(a1, (tTexProceduralStruct*)param1);
                    break;
                }
            }
            else if (param1)
            {
                ((tTexProcMpeg*)(a1->proceduralTexPtr))->flags |= 1u;
            }
            else
            {
                ((tTexProcMpeg*)(a1->proceduralTexPtr))->flags &= ~1u;
            }
        }
    }
}

tAINode* AI_EvalFunc_MATProSeti(tAINode* a1)
{
    tTexProceduralStruct* v1;
    int a2;
    int a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = (tTexProceduralStruct*)AI_GetIntFromStack();
    AI_EvalFunc_MATProSeti_C(v1, a2, a3);
    return a1 + 1;
}

float AI_EvalFunc_GetUV_C(tGameObjectMain* a1, unsigned int a2, int multiMatElemId, int a4)
{
    double result;
    tMultiTextureElemStruct* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
        MAT_GetMtxLvlPtr(v6, &v5, 0, multiMatElemId);
    if (!v5)
        return 0.0;
    switch (a4)
    {
    case 0:
        result = MAT_GetSpeed(v5->transform1);
        break;
    case 1:
        result = MAT_GetSpeed(v5->transform2);
        break;
    case 2:
        result = MAT_GetScale(v5->transform1);
        break;
    case 3:
        result = MAT_GetScale(v5->transform2);
        break;
    case 4:
        result = MAT_GetRotation(v5->transform1, v5->transform2);
        break;
    default:
        return 0.0;
    }
    return (float)result;
}

tAINode* AI_EvalFunc_GetUV(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;
    int a2;
    int a4;
    int multiMatElemId;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a4 = AI_GetIntFromStack();
    multiMatElemId = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_GetUV_C(v3, a2, multiMatElemId, a4);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_SetUV_C(struct sGameObjectMain* a1, int a2, int a3, int a4, float a5)
{
    tMultiTextureElemStruct* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
        MAT_GetMtxLvlPtr(v6, &v5, 0, a3);
    if (v5)
    {
        switch (a4)
        {
        case 0:
            MAT_SetPos(&v5->transform1, a5);
            break;
        case 1:
            MAT_SetPos(&v5->transform2, a5);
            break;
        case 2:
            MAT_SetScale(&v5->transform1, a5);
            break;
        case 3:
            MAT_SetScale(&v5->transform2, a5);
            break;
        case 4:
            MAT_SetRotation(&v5->transform1, &v5->transform2, a5);
            break;
        default:
            return;
        }
    }
}

tAINode* AI_EvalFunc_SetUV(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a4;
    float a5; 
    int a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetFloatFromStack();
    a4 = AI_GetIntFromStack();
    a3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_SetUV_C(v3, v1, a3, a4, a5);
    return a1 + 1;
}

BOOL AI_EvalFunc_MATProGeti_C(struct sTexProceduralStruct* a1, int a2)
{
    if (!a1)
        return 0;
    if (a1->proceduralIntf->id != 4)
    {
        if (a1->proceduralIntf->id == 3 && !a2)
            return (((tTexProcMpeg*)(a1->proceduralTexPtr))->flags & 1) != 0;
        return 0;
    }
    if (a2)
        return 0;
    return ((tTexProcPhoto*)(a1->proceduralTexPtr))->b1 == (char)((tTexProcPhoto*)(a1->proceduralTexPtr))->b2 && !((tTexProcPhoto*)(a1->proceduralTexPtr))->b3;
}

tAINode* AI_EvalFunc_MATProGeti(tAINode* a1)
{
    int v1;
    int a2;
    tTexProceduralStruct* v4;

    a2 = AI_GetIntFromStack();
    v4 = (tTexProceduralStruct*)AI_GetIntFromStack();
    v1 = AI_EvalFunc_MATProGeti_C(v4, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tTexAnimatedElem* AI_EvalFunc_MATAntGet_C(tGameObjectMain* a1, unsigned int a2, int multiMatElemId)
{
    tMultiTextureElemStruct* v4;
    tTexAnimatedElem* v5;
    tGEOMAT* v6;

    v5 = 0;
    v6 = MAT_ValidateSUBMAT_C(a1, a2);
    if (v6)
    {
        MAT_GetMtxLvlPtr(v6, &v4, 0, multiMatElemId);
        if (v4)
            v5 = TEX_pst_Anim_Get(v4->texElemIdx);
    }
    return v5;
}

tAINode* AI_EvalFunc_MATAntGet(tAINode* a1)
{
    tTexAnimatedElem* v1;
    tGameObjectMain* v3;
    int a2;
    int multiMatElemId;

    multiMatElemId = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_MATAntGet_C(v3, a2, multiMatElemId);
    AI_PutIntToStack((int)v1);
    return a1 + 1;
}

void AI_EvalFunc_MATAntSeti_C(tTexAnimatedElem* a1, int a2, int a3)
{
    if (a1)
    {
        if (a2 == 1)
        {
            if (a3)
                a1->animationFlags |= 1u;
            else
                a1->animationFlags &= ~1u;
        }
        else if (a2 == 2)
        {
            if (a3 == -1)
            {
                a1->animationFlags &= ~2u;
            }
            else
            {
                a1->animationFlags |= 2u;
                a1->b3 = a3;
            }
        }
    }
}

tAINode* AI_EvalFunc_MATAntSeti(tAINode* a1)
{
    tTexAnimatedElem* v1;
    int a2;
    int a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = (tTexAnimatedElem*)AI_GetIntFromStack();
    AI_EvalFunc_MATAntSeti_C(v1, a2, a3);
    return a1 + 1;
}

int AI_EvalFunc_COLORSetRGBA_C(unsigned __int8 a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4)
{
    return a1 | (a2 << 8) | (a3 << 16) | (a4 << 24);
}

tAINode* AI_EvalFunc_COLORSetRGBA(tAINode* a1)
{
    int v1;
    unsigned __int8 v3;
    unsigned __int8 v4;
    unsigned __int8 v5;
    unsigned __int8 v6;

    v4 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v6 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_COLORSetRGBA_C(v6, v3, v5, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_COLORGetRed_C(unsigned __int8 a1)
{
    return a1;
}

tAINode* AI_EvalFunc_COLORGetRed(tAINode* a1)
{
    unsigned __int8 v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_COLORGetRed_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_COLORGetGreen_C(__int16 a1)
{
    return (a1 & 0xFF00) >> 8;
}

tAINode* AI_EvalFunc_COLORGetGreen(tAINode* a1)
{
    __int16 v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_COLORGetGreen_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_COLORGetBlue_C(int a1)
{
    return (a1 & 0xFF0000) >> 16;
}

tAINode* AI_EvalFunc_COLORGetBlue(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_COLORGetBlue_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

unsigned int AI_EvalFunc_COLORGetAlpha_C(int a1)
{
    return (a1 & 0xFF000000) >> 24;
}

tAINode* AI_EvalFunc_COLORGetAlpha(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_COLORGetAlpha_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

unsigned int ul_Interpol2PackedColor(int a1, int a2, float a3)
{
    unsigned int v4;
    unsigned int v5;
    unsigned int v6;
    unsigned int v7;
    unsigned int v8;
    unsigned int v9;
    unsigned int v10;
    float v11;
    int v11a;

    if (a3 >= 0.98000002)
        return a2;
    if (a3 <= 0.02)
        return a1;
    v11 = (float)(a3 + 32768.0 + 16384.0);
    v11a = *(uint32_t*)&v11;

    if ((v11a & 0x80) != 0)
        v10 = (a2 & 0xFEFEFEFE) >> 1;
    else
        v10 = (a1 & 0xFEFEFEFE) >> 1;
    if ((v11a & 0x40) != 0)
        v9 = (a2 & 0xFCFCFCFC) >> 2;
    else
        v9 = (a1 & 0xFCFCFCFC) >> 2;
    if ((v11a & 0x20) != 0)
        v8 = (a2 & 0xF8F8F8F8) >> 3;
    else
        v8 = (a1 & 0xF8F8F8F8) >> 3;
    if ((v11a & 0x10) != 0)
        v7 = (a2 & 0xF0F0F0F0) >> 4;
    else
        v7 = (a1 & 0xF0F0F0F0) >> 4;
    if ((v11a & 8) != 0)
        v6 = (a2 & 0xE0E0E0E0) >> 5;
    else
        v6 = (a1 & 0xE0E0E0E0) >> 5;
    if ((v11a & 4) != 0)
        v5 = (a2 & 0xC0C0C0C0) >> 6;
    else
        v5 = (a1 & 0xC0C0C0C0) >> 6;
    if ((v11a & 2) != 0)
        v4 = (a2 & 0x80808080) >> 7;
    else
        v4 = (a1 & 0x80808080) >> 7;
    return v4 + v5 + v6 + v7 + v8 + v9 + v10;
}

int AI_EvalFunc_COLORBlend_C(int a1, int a2, float a3)
{
    if (a3 < 0.0)
        a3 = 0.0;
    if (a3 > 1.0)
        a3 = 1.0;
    return ul_Interpol2PackedColor(a1, a2, a3);
}

tAINode* AI_EvalFunc_COLORBlend(tAINode* a1)
{
    int v1;
    float v3;
    int v4;
    int v5;

    v3 = AI_GetFloatFromStack();
    v5 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_COLORBlend_C(v4, v5, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

unsigned int AI_EvalFunc_COLORBlack_C()
{
    return 0xFF000000;
}

tAINode* AI_EvalFunc_COLORWhite(tAINode* a1)
{
    AI_PutIntToStack(0xFFFFFFFF);
    return a1 + 1;
}

tAINode* AI_EvalFunc_COLORBlack(tAINode* a1)
{
    AI_PutIntToStack(0xFF000000);
    return a1 + 1;
}