#include "../ai.h"
#include "../math.h"
#include "../gameobject.h"
#include "../textget.h"

void AI_EvalType_GetInt(int32_t* src, int32_t* dest)
{
    *dest = *src;
}

void AI_EvalType_GetGAO(struct sGameObjectMain **src, struct sGameObjectMain **dest)
{
    *dest = *src;
    if (*dest == (struct sGameObjectMain*)1)
    {
        *dest = AI_MainActor[0];
    }
    else if (*dest == (struct sGameObjectMain*)2)
    {
        *dest = AI_MainActor[1];
    }
}

void AI_EvalType_GetVector(Vec3* src, Vec3* dest)
{
    *dest = *src;
}

void AI_EvalType_GetMessage(tMessage* src, tMessage* dest)
{
    memcpy(dest, src, sizeof(tMessage));
}

void AI_EvalType_GetText(tTextGet* src, tTextGet* dest)
{
    memcpy(dest, src, sizeof(tTextGet));
}

// TODO: create unique function names (eg color: color, not int)
tOfc_scriptengine_functions_types AI_gast_Types[23] = {
                                    {0x20, 4, AI_EvalType_GetInt}, // bool
                                    {0x21, 4, AI_EvalType_GetInt}, // int
                                    {0x22, 4, AI_EvalType_GetInt}, // int
                                    {0x25, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x26, 4, AI_EvalType_GetInt}, // string
                                    {0x27, 4, AI_EvalType_GetInt}, // pointerref
                                    {0x28, 4, AI_EvalType_GetGAO}, // gao
                                    {0x29, sizeof(tMessage), AI_EvalType_GetMessage}, // message
                                    {0x2A, 4, AI_EvalType_GetInt}, // pointerref
                                    {0x2B, 4, AI_EvalType_GetInt}, // pointerref
                                    {0x2C, sizeof(tTextGet), AI_EvalType_GetText}, // text
                                    {0x2D, 4, AI_EvalType_GetInt}, // key
                                    {0x2E, 4, AI_EvalType_GetInt}, // color
                                    {0x7C, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x7D, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x7E, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x7F, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x80, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x81, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x82, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x83, sizeof(Vec3), AI_EvalType_GetVector}, // vec
                                    {0x84, 4, AI_EvalType_GetVector}, // ? size  // vec
                                    {0x85, sizeof(Vec3), AI_EvalType_GetVector}  // vec

};
