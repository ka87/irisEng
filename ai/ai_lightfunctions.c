#include "../graphicsobjects/light.h"
#include "ai_lightfunctions.h"

// Every per-object light builtin starts the same way: the object has to
// exist, carry the light flag, and have a light behind it.
static tLIGHT* AI_l_GetLight(struct sGameObjectMain* gao)
{
    if (!gao || (gao->flags & 0x10) == 0)
        return NULL;
    return (tLIGHT*)(gao->extended->light);
}

// Near and far mean something only for these three kinds; the switch
// over them had one body per arm and all three bodies were the same.
static BOOL AI_l_LightHasRange(tLIGHT* light)
{
    int kind;

    kind = light->flags & 7;
    return kind == 0 || kind == 2 || kind == 3;
}

int AI_EvalFunc_LightGetColor_C(struct sGameObjectMain* gao)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (!light)
        return 0;
    return light->color;
}

tAINode* AI_EvalFunc_LightGetColor(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_LightGetColor_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_LightSetColor_C(struct sGameObjectMain* gao, int color)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (light)
        light->color = color;
}

tAINode* AI_EvalFunc_LightSetColor(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int color;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    color = AI_GetIntFromStack();
    AI_EvalFunc_LightSetColor_C(gaoUltra, color);
    return node + 1;
}

void AI_EvalFunc_LightSetNearFar_C(struct sGameObjectMain* gao, float fnear, float ffar)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (!light || !AI_l_LightHasRange(light))
        return;

    // a negative argument leaves that end of the range alone
    if (fnear >= 0.0f)
        light->fnear = fnear;
    if (ffar >= 0.0f)
        light->ffar = ffar;
}

tAINode* AI_EvalFunc_LightSetNearFar(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    float fnear;
    float ffar;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    ffar = AI_GetFloatFromStack();
    fnear = AI_GetFloatFromStack();
    AI_EvalFunc_LightSetNearFar_C(gaoUltra, fnear, ffar);
    return node + 1;
}

float AI_EvalFunc_LightGetNear_C(tGameObjectMain* gao)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (!light || !AI_l_LightHasRange(light))
        return 0.0f;
    return light->fnear;
}

tAINode* AI_EvalFunc_LightGetNear(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_LightGetNear_C(gaoUltra));
    return node + 1;
}

float AI_EvalFunc_LightGetFar_C(tGameObjectMain* gao)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (!light || !AI_l_LightHasRange(light))
        return 0.0f;
    return light->ffar;
}

tAINode* AI_EvalFunc_LightGetFar(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutFloatToStack(AI_EvalFunc_LightGetFar_C(gaoUltra));
    return node + 1;
}

void AI_EvalFunc_LightSetGO_C(struct sGameObjectMain* gao, struct sGameObjectMain* lightGao)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (light)
        light->lightGao = lightGao;
}

tAINode* AI_EvalFunc_LightSetGO(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_LightSetGO_C(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

void AI_EvalFunc_LightSetFlag_C(struct sGameObjectMain* gao, char bitIndex, int set)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (!light)
        return;

    if (set)
        light->flags |= 1 << bitIndex;
    else
        light->flags &= ~(1 << bitIndex);
}

tAINode* AI_EvalFunc_LightSetFlag(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char bitIndex;
    int set;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    set = AI_GetIntFromStack();
    bitIndex = AI_GetIntFromStack();
    AI_EvalFunc_LightSetFlag_C(gaoUltra, bitIndex, set);
    return node + 1;
}

BOOL AI_EvalFunc_LightGetFlag_C(struct sGameObjectMain* gao, char bitIndex)
{
    tLIGHT* light;

    light = AI_l_GetLight(gao);
    if (!light)
        return FALSE;
    return ((1 << bitIndex) & light->flags) != 0;
}

tAINode* AI_EvalFunc_LightGetFlag(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    char bitIndex;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    bitIndex = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_LightGetFlag_C(gaoUltra, bitIndex));
    return node + 1;
}

float AI_EvalFunc_LightGetRLIFactor_C()
{
    if (!gpst_DX_stDD)
        return 1.0f;
    return gpst_DX_stDD->RLIFactor;
}

tAINode* AI_EvalFunc_LightGetRLIFactor(tAINode* node)
{
    AI_PutFloatToStack(AI_EvalFunc_LightGetRLIFactor_C());
    return node + 1;
}

void AI_EvalFunc_LightSetRLIFactor_C(float factor)
{
    if (gpst_DX_stDD)
        gpst_DX_stDD->RLIFactor = factor;
}

tAINode* AI_EvalFunc_LightSetRLIFactor(tAINode* node)
{
    float factor;

    factor = AI_GetFloatFromStack();
    AI_EvalFunc_LightSetRLIFactor_C(factor);
    return node + 1;
}

int AI_EvalFunc_LightGetRLIDestColor_C()
{
    if (!gpst_DX_stDD)
        return 0;
    return gpst_DX_stDD->RLIDestColor;
}

tAINode* AI_EvalFunc_LightGetRLIDestColor(tAINode* node)
{
    AI_PutIntToStack(AI_EvalFunc_LightGetRLIDestColor_C());
    return node + 1;
}

void AI_EvalFunc_LightSetRLIDestColor_C(int color)
{
    if (gpst_DX_stDD)
        gpst_DX_stDD->RLIDestColor = color;
}

tAINode* AI_EvalFunc_LightSetRLIDestColor(tAINode* node)
{
    int color;

    color = AI_GetIntFromStack();
    AI_EvalFunc_LightSetRLIDestColor_C(color);
    return node + 1;
}

// Returns the state the flag had before the call, so a script can put
// it back. A state that is neither 0 nor 1 only reads it.
BOOL AI_EvalFunc_LightRLIFactorUsed_C(int state)
{
    BOOL wasUsed;

    if (!gpst_DX_stDD)
        return FALSE;

    wasUsed = (gpst_DX_stDD->drawMaskAllowedStuff & 0x2000000) == 0;
    if (state == 0)
    {
        gpst_DX_stDD->drawMaskAllowedStuff |= 0x2000000u;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent |= 0x2000000u;
    }
    else if (state == 1)
    {
        gpst_DX_stDD->drawMaskAllowedStuff &= ~0x2000000u;
        gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x2000000u;
    }
    return wasUsed;
}

tAINode* AI_EvalFunc_LightRLIFactorUsed(tAINode* node)
{
    int state;

    state = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_LightRLIFactorUsed_C(state));
    return node + 1;
}

BOOL AI_EvalFunc_LightFogState_C(int state)
{
    BOOL wasOn;

    if (!gpst_DX_stDD)
        return FALSE;

    wasOn = (gpst_DX_stDD->drawMaskAllowedStuff & 0x4000) != 0;
    if (state == 1)
        gpst_DX_stDD->drawMaskAllowedStuff |= 0x4000u;
    else if (state == 0)
        gpst_DX_stDD->drawMaskAllowedStuff &= ~0x4000u;
    return wasOn;
}

tAINode* AI_EvalFunc_LightFogState(tAINode* node)
{
    int state;

    state = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_LightFogState_C(state));
    return node + 1;
}

tGameObjectMain* AI_EvalFunc_LightGetFog_C()
{
    tableElemStructPF* elem;
    tableElemStructPF* end;
    tGameObjectMain* gao;
    tLIGHT* light;

    end = (tableElemStructPF*)WOR_gpst_CurrentWorld->lightObjectsTablePF.nextElemPtr;
    for (elem = WOR_gpst_CurrentWorld->lightObjectsTablePF.firstElemPtr; elem < end; ++elem)
    {
        // a free entry carries the tag 01 in its low two bits
        if (!elem->objectPtr || ((uintptr_t)elem->objectPtr & 3) == 1)
            continue;

        gao = (tGameObjectMain*)(elem->objectPtr);
        light = (tLIGHT*)(gao->extended->light);
        if (light && (light->flags & 8) != 0 && (light->flags & 7) == 3)
            return gao;
    }
    return NULL;
}

tAINode* AI_EvalFunc_LightGetFog(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMain* fog;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    fog = AI_EvalFunc_LightGetFog_C();
    AI_PutObjectToStack(&fog, AI_gaw_EnumLink[40]);
    return node + 1;
}

// With no object the factor is the display device's own; with one it is
// the visual object's, stored as a byte over 127.
float AI_EvalFunc_LightGetAttenuationFactor_C(tGameObjectMain* gao)
{
    tGameObjectVisualObject* visual;

    if (!gao)
    {
        if (!gpst_DX_stDD || (gpst_DX_stDD->drawMaskAllowedStuff & 0x40000000) != 0)
            return 0.0f;
        return gpst_DX_stDD->lightAttenuationFactor;
    }

    if (!gao->basic || !gao->basic->actorObjectOrVisualObject || (gao->flags & 0x4000) == 0)
        return 0.0f;

    visual = (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
    if ((visual->objDrawMask & 0x40000000) != 0)
        return 0.0f;
    return (float)((float)visual->vmilyenDistanceLightAttenuationFactor / 127.0);
}

tAINode* AI_EvalFunc_LightGetAttenuationFactor(tAINode* node)
{
    tGameObjectMain* gao;

    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_PutFloatToStack(AI_EvalFunc_LightGetAttenuationFactor_C(gao));
    return node + 1;
}

void AI_EvalFunc_LightSetAttenuationFactor_C(struct sGameObjectMain* gao, float factor)
{
    tGameObjectVisualObject* visual;
    BOOL attenuate;

    attenuate = factor > 0.0f && factor <= 1.0f;

    if (!gao)
    {
        if (!gpst_DX_stDD)
            return;

        if (attenuate)
        {
            gpst_DX_stDD->drawMaskAllowedStuff &= ~0x40000000u;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent &= ~0x40000000u;
            gpst_DX_stDD->lightAttenuationFactor = factor;
        }
        else
        {
            gpst_DX_stDD->drawMaskAllowedStuff |= 0x40000000u;
            gpst_DX_stDD->drawMaskAllowedStuffCurrent |= 0x40000000u;
            gpst_DX_stDD->lightAttenuationFactor = 0.0f;
        }
        return;
    }

    if (!gao->basic || !gao->basic->actorObjectOrVisualObject || (gao->flags & 0x4000) == 0)
        return;

    visual = (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
    if (attenuate)
    {
        visual->objDrawMask &= ~0x40000000u;
        visual->vmilyenDistanceLightAttenuationFactor = (unsigned int)(factor * 127.0);
    }
    else
    {
        visual->objDrawMask |= 0x40000000u;
        visual->vmilyenDistanceLightAttenuationFactor = 0;
    }
}

tAINode* AI_EvalFunc_LightSetAttenuationFactor(tAINode* node)
{
    tGameObjectMain* gao;
    float factor;

    factor = AI_GetFloatFromStack();
    gao = (tGameObjectMain*)AI_GetObjectFromStack();
    AI_EvalFunc_LightSetAttenuationFactor_C(gao, factor);
    return node + 1;
}
