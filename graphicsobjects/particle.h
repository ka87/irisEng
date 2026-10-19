#pragma once
#include "gro_common.h"

struct sPAGStruct* PAG_pst_Create();
struct sGroGeneric* PAG_p_CreateFromBuffer(struct sGroGeneric* a1, char** a2, struct sWorld* world);
struct sGroGeneric* PAG_p_Duplicate(struct sGroGeneric* a1);
void PAG_Free(struct sGroGeneric* a1);
void PAG_Reinit(struct sGroGeneric* a1);
void PAG_Init();
struct sPAGStruct* PAG_pst_GetGameObjectParticleGenerator(struct sGameObjectMain* a1);
int PAG_i_AddGenerator(struct sPAGStruct* a1, struct sGameObjectMain* a2, float a3);
void PAG_DelGenerator(struct sPAGStruct* a1, int a2);
void PAG_SetGeneratorFrequency(struct sPAGStruct* a1, int generatorID, float a3);
