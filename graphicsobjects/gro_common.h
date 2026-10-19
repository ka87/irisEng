#pragma once
#include "../common.h"
#include "../gameobject.h"
#include "../world.h"

typedef struct sGroIntf
{
    int id;
    struct sGroGeneric* (*Create)();
    struct sGroGeneric* (*CreateFromBuffer)(struct sGroGeneric*, char**, struct sWorld*); //tGroGeneric *, char ** [readptr], tWorld * => GEO_ul_Load_ObjectCallback
    struct sGroGeneric* (*Duplicate)(struct sGroGeneric*);
    void (*Destroy)(struct sGroGeneric *);
    BOOL (*HasSomethingToRender)(struct sGameObjectVisualObject* a1, struct sGroGeneric** a2); // => GRO_Render
    void (*Render)(struct sGameObjectMain*);
    void (*AddRef)(struct sGroGeneric*, int);
    void (*Reinit)(struct sGroGeneric*);
    int (*GetValidatedMask)(void *, int); // (GEOMATStruct*) => validityFlag
} tGroIntf;


typedef struct sGroGeneric
{
    tGroIntf* intf;
    int usageCount;
} tGroGeneric;

extern tGroIntf GRO_gast_Interface[12];
tGroIntf* GRO_gast_last;
int GEO_b_IsInFix;

tGroGeneric* GRO_pst_Struct_Create(int a1);
void GRO_Struct_Init(tGroGeneric* a1, int a2);
struct sGroGeneric* GEO_ul_Load_ObjectCallback(uint32_t Offset);
void GRO_Struct_DestroyTable(tableDescStruct* a1);
void GRO_Render(struct sGameObjectMain* a2);
void GRO_Struct_InitInterfaces();