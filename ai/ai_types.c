#include "../ai.h"
#include "../handle.h"
#include "../math.h"
#include "../gameobject.h"
#include "../textget.h"

void AI_EvalType_GetInt(int32_t* src, int32_t* dest)
{
    *dest = *src;
}

/* An object reference, between two of the four-byte slots the file
   formats fix. What is in such a slot is a handle for the object, or
   one of the values that is not a handle at all -- nought for no
   object, and one and two for the two main actors, which a script
   names rather than refers to.
 *
 * Those two are turned into the handles of the actors they name, and
 * not into the actors' addresses: everything downstream of here is
 * another four-byte slot, and a slot holding an address for two values
 * and a handle for the rest would be two representations at once. The
 * one place a handle becomes an address is where a value leaves the
 * stack for a C variable, in AI_GetObjectFromStack. */
void AI_EvalType_GetGAO(uint32_t* src, uint32_t* dest)
{
    *dest = *src;
    if (*dest == AI_MAIN_ACTOR_FIRST)
        *dest = HDL_ul_Handle(AI_MainActor[0]);
    else if (*dest == AI_MAIN_ACTOR_SECOND)
        *dest = HDL_ul_Handle(AI_MainActor[1]);
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
                                    {0x20, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // bool
                                    {0x21, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // int
                                    {0x22, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // int
                                    {0x25, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x26, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // string
                                    {0x27, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // pointerref
                                    {0x28, 4, (void (*)(void *, void *))AI_EvalType_GetGAO}, // gao
                                    {0x29, sizeof(tMessage), (void (*)(void *, void *))AI_EvalType_GetMessage}, // message
                                    {0x2A, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // pointerref
                                    {0x2B, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // pointerref
                                    {0x2C, sizeof(tTextGet), (void (*)(void *, void *))AI_EvalType_GetText}, // text
                                    {0x2D, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // key
                                    {0x2E, 4, (void (*)(void *, void *))AI_EvalType_GetInt}, // color
                                    {0x7C, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x7D, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x7E, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x7F, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x80, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x81, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x82, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x83, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    {0x84, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}, // vec
                                    // Twelve, like the nine other members of
                                    // this family and like the copier beside
                                    // it, which moves a vector. The original
                                    // table says four here, with its author's
                                    // own "? size" against it; this is a
                                    // deliberate correction and the only place
                                    // this table departs from it.
                                    {0x85, sizeof(Vec3), (void (*)(void *, void *))AI_EvalType_GetVector}  // vec

};
