#pragma once
#include "tables.h"
#include "common.h"

typedef struct sGroup
{
    tableDescStruct* tablePF;
    int unk2;
    int groupElemNum;
} tGroup;

void GRP_ObjDetachGroup(struct sGameObjectMain* gao);
void GRP_DuplicateGroupObjects(struct sWorld* world, struct sGroup* srcGroup, struct sGroup* destGroup);
struct sGroup* GRP_pst_CreateNewGroup();
struct sGroup* GRP_ul_LoadGrpCallback(uint32_t Offset);