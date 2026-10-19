#include "../graphicsobjects/light.h"
#include "ai_lightfunctions.h"

int AI_EvalFunc_LightGetColor_C(struct sGameObjectMain* a1)
{
    int result;
    tLIGHT* v2;

    if (!a1 || (a1->flags & 0x10) == 0)
        return 0;
    v2 = (tLIGHT*)(a1->extended->light);
    if (v2)
        result = v2->color;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_LightGetColor(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_LightGetColor_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_LightSetColor_C(struct sGameObjectMain* a1, int a2)
{
    tLIGHT* v2;

    if (a1 && (a1->flags & 0x10) != 0)
    {
        v2 = (tLIGHT*)(a1->extended->light);
        if (v2)
            v2->color = a2;
    }
}

tAINode* AI_EvalFunc_LightSetColor(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_LightSetColor_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_LightSetNearFar_C(struct sGameObjectMain* a1, float a2, float a3)
{
    int v3;
    tLIGHT* v4;

    if (a1)
    {
        if ((a1->flags & 0x10) != 0)
        {
            v4 = (tLIGHT*)(a1->extended->light);
            if (v4)
            {
                v3 = v4->flags & 7;
                if (v3)
                {
                    if (v3 == 2)
                    {
                        if (a2 >= 0.0)
                            v4->fnear = a2;
                        if (a3 >= 0.0)
                            v4->ffar = a3;
                    }
                    else if (v3 == 3)
                    {
                        if (a2 >= 0.0)
                            v4->fnear = a2;
                        if (a3 >= 0.0)
                            v4->ffar = a3;
                    }
                }
                else
                {
                    if (a2 >= 0.0)
                        v4->fnear = a2;
                    if (a3 >= 0.0)
                        v4->ffar = a3;
                }
            }
        }
    }
}

tAINode* AI_EvalFunc_LightSetNearFar(tAINode* a1)
{
    float a2;
    tGameObjectMain* v3;
    float a3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetFloatFromStack();
    a2 = AI_GetFloatFromStack();
    AI_EvalFunc_LightSetNearFar_C(v3, a2, a3);
    return a1 + 1;
}

float AI_EvalFunc_LightGetNear_C(tGameObjectMain* a1)
{
    int v2;
    tLIGHT* v3;

    if (!a1 || (a1->flags & 0x10) == 0)
        return 0.0;
    v3 = (tLIGHT*)(a1->extended->light);
    if (!v3)
        return 0.0;
    v2 = v3->flags & 7;
    switch (v2)
    {
    case 0:
        return v3->fnear;
    case 2:
        return v3->fnear;
    case 3:
        return v3->fnear;
    }
    return 0.0;
}

tAINode* AI_EvalFunc_LightGetNear(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_LightGetNear_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_LightGetFar_C(tGameObjectMain* a1)
{
    int v2;
    tLIGHT* v3;

    if (!a1 || (a1->flags & 0x10) == 0)
        return 0.0;
    v3 = (tLIGHT*)(a1->extended->light);
    if (!v3)
        return 0.0;
    v2 = v3->flags & 7;
    switch (v2)
    {
    case 0:
        return v3->ffar;
    case 2:
        return v3->ffar;
    case 3:
        return v3->ffar;
    }
    return 0.0;
}

tAINode* AI_EvalFunc_LightGetFar(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v2 = AI_EvalFunc_LightGetFar_C(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_LightSetGO_C(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    tLIGHT* v2;

    if (a1 && (a1->flags & 0x10) != 0)
    {
        v2 = (tLIGHT*)(a1->extended->light);
        if (v2)
            v2->lightGao = a2;
    }
}

tAINode* AI_EvalFunc_LightSetGO(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_LightSetGO_C(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

void AI_EvalFunc_LightSetFlag_C(struct sGameObjectMain* a2, char a3, int a4)
{
    tLIGHT* v3;

    if (a2)
    {
        if ((a2->flags & 0x10) != 0)
        {
            v3 = (tLIGHT*)(a2->extended->light);
            if (v3)
            {
                if (a4)
                    v3->flags |= 1 << a3;
                else
                    v3->flags &= ~(1 << a3);
            }
        }
    }
}

tAINode* AI_EvalFunc_LightSetFlag(tAINode* a1)
{
    char v1;
    int v3;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_LightSetFlag_C(v4, v1, v3);
    return a1 + 1;
}

BOOL AI_EvalFunc_LightGetFlag_C(struct sGameObjectMain* a1, char a2)
{
    BOOL result;
    tLIGHT* v3;

    if (!a1 || (a1->flags & 0x10) == 0)
        return 0;
    v3 = (tLIGHT*)(a1->extended->light);
    if (v3)
        result = ((1 << a2) & v3->flags) != 0;
    else
        result = 0;
    return result;
}

tAINode* AI_EvalFunc_LightGetFlag(tAINode* a1)
{
    char v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_LightGetFlag_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

float AI_EvalFunc_LightGetRLIFactor_C()
{
    float v1;

    if (gpst_DX_stDD)
        v1 = gpst_DX_stDD->RLIFactor;
    else
        v1 = 1.0;
    return v1;
}

tAINode* AI_EvalFunc_LightGetRLIFactor(tAINode* a1)
{
    float a1a;

    a1a = AI_EvalFunc_LightGetRLIFactor_C();
    AI_PutFloatToStack(a1a);
    return a1 + 1;
}

void AI_EvalFunc_LightSetRLIFactor_C(float a1)
{
    if (gpst_DX_stDD)
        gpst_DX_stDD->RLIFactor = a1;
}

tAINode* AI_EvalFunc_LightSetRLIFactor(tAINode* a1)
{
    float v2;

    v2 = AI_GetFloatFromStack();
    AI_EvalFunc_LightSetRLIFactor_C(v2);
    return a1 + 1;
}

int AI_EvalFunc_LightGetRLIDestColor_C()
{
    int v1;

    if (gpst_DX_stDD)
        v1 = gpst_DX_stDD->RLIDestColor;
    else
        v1 = 0;
    return v1;
}

tAINode* AI_EvalFunc_LightGetRLIDestColor(tAINode* a1)
{
    int v1;

    v1 = AI_EvalFunc_LightGetRLIDestColor_C();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_LightSetRLIDestColor_C(int a1)
{
    if (gpst_DX_stDD)
        gpst_DX_stDD->RLIDestColor = a1;
}

tAINode* AI_EvalFunc_LightSetRLIDestColor(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    AI_EvalFunc_LightSetRLIDestColor_C(v1);
    return a1 + 1;
}

BOOL AI_EvalFunc_LightRLIFactorUsed_C(int a1)
{
    BOOL v2;

    if (!gpst_DX_stDD)
        return 0;
    v2 = (gpst_DX_stDD->drawMaskAllowedStuff & 0x2000000) == 0;
    if (a1)
    {
        if (a1 == 1)
        {
            gpst_DX_stDD->drawMaskAllowedStuff &= ~0x2000000u;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x2000000u;
        }
    }
    else
    {
        gpst_DX_stDD->drawMaskAllowedStuff |= 0x2000000u;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent |= 0x2000000u;
    }
    return v2;
}

tAINode* AI_EvalFunc_LightRLIFactorUsed(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_LightRLIFactorUsed_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

BOOL AI_EvalFunc_LightFogState_C(int a1)
{
    BOOL v2;

    if (!gpst_DX_stDD)
        return 0;
    v2 = (gpst_DX_stDD->drawMaskAllowedStuff & 0x4000) != 0;
    if (a1 == 1)
    {
        gpst_DX_stDD->drawMaskAllowedStuff |= 0x4000u;
    }
    else if (!a1)
    {
        gpst_DX_stDD->drawMaskAllowedStuff &= ~0x4000u;
    }
    return v2;
}

tAINode* AI_EvalFunc_LightFogState(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_LightFogState_C(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tGameObjectMain* AI_EvalFunc_LightGetFog_C()
{
    tLIGHT* light;
    tableElemStructPF* i;

    for (i = WOR_gpst_CurrentWorld->lightObjectsTablePF.firstElemPtr; i <= &((tableElemStructPF*)(WOR_gpst_CurrentWorld->lightObjectsTablePF.nextElemPtr))[-1]; ++i)
    {
        if (i->objectPtr && ((int)i->objectPtr & 3) != 1)
        {
            light = (tLIGHT*)(((tGameObjectMain*)(i->objectPtr))->extended->light);
            if (light)
            {
                if ((light->flags & 8) != 0 && (light->flags & 7) == 3)
                    return (tGameObjectMain*)(i->objectPtr);
            }
        }
    }
    return NULL;
}

tAINode* AI_EvalFunc_LightGetFog(tAINode* a1)
{
    int v2[3];
    tGameObjectMain* v3;
    tGameObjectMain* v4;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v4 = v3;
    v2[0] = (int)AI_EvalFunc_LightGetFog_C();
    AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
    return a1 + 1;
}

float AI_EvalFunc_LightGetAttenuationFactor_C(tGameObjectMain* a1)
{
    double result;
    float v2;

    if (a1)
    {
        if (a1->basic)
        {
            if (a1->basic->actorObjectOrVisualObject && (a1->flags & 0x4000) != 0)
            {
                if ((((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask & 0x40000000) != 0)
                {
                    result = 0.0;
                }
                else
                {
                    v2 = (float)((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->vmilyenDistanceLightAttenuationFactor;
                    result = v2 / 127.0;
                }
            }
            else
            {
                result = 0.0;
            }
        }
        else
        {
            result = 0.0;
        }
    }
    else if (!gpst_DX_stDD || (gpst_DX_stDD->drawMaskAllowedStuff & 0x40000000) != 0)
    {
        result = 0.0;
    }
    else
    {
        result = gpst_DX_stDD->lightAttenuationFactor;
    }
    return (float)result;
}

tAINode* AI_EvalFunc_LightGetAttenuationFactor(tAINode* a1)
{
    float v2;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v2 = AI_EvalFunc_LightGetAttenuationFactor_C(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

void AI_EvalFunc_LightSetAttenuationFactor_C(struct sGameObjectMain* a1, float a2)
{
    if (a1)
    {
        if (a1->basic && a1->basic->actorObjectOrVisualObject && (a1->flags & 0x4000) != 0)
        {
            if (a2 > 0.0f && a2 <= 1.0f)
            {
                ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask &= ~0x40000000u;
                ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->vmilyenDistanceLightAttenuationFactor = (unsigned int)(a2 * 127.0);
            }
            else
            {
                ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->objDrawMask |= 0x40000000u;
                ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->vmilyenDistanceLightAttenuationFactor = 0;
            }
        }
    }
    else if (gpst_DX_stDD)
    {
        if (a2 > 0.0f && a2 <= 1.0f)
        {
            gpst_DX_stDD->drawMaskAllowedStuff &= ~0x40000000u;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x40000000u;
            gpst_DX_stDD->lightAttenuationFactor = a2;
        }
        else
        {
            gpst_DX_stDD->drawMaskAllowedStuff |= 0x40000000u;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent |= 0x40000000u;
            gpst_DX_stDD->lightAttenuationFactor = 0.0f;
        }
    }
}

tAINode* AI_EvalFunc_LightSetAttenuationFactor(tAINode* a1)
{
    float a2;

    a2 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    AI_EvalFunc_LightSetAttenuationFactor_C(*(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack], a2);
    return a1 + 1;
}