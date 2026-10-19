#include "world.h"
#include "memory.h"
#include "loading.h"
#include "tables.h"
#include "bigfiles.h"
#include "gameobject.h"
#include "groups.h"

void GRP_ObjDetachGroup(tGameObjectMain* gao)
{
    tableElemStructPF* currElem;
    tableElemStructPF* lastElem;
    tGameObjectMain* currGao;

    if (gao->extended && gao->extended->grp)
    {
        if (gao->extended->grp->groupElemNum-- == 1)
        {
            if ((gao->flags & 0x1) != 0)
            {
                lastElem = ((tableElemStructPF*)(gao->extended->grp->tablePF->nextElemPtr)) - 1;
                for (currElem = gao->extended->grp->tablePF->firstElemPtr; currElem <= lastElem; currElem++)
                {
                    currGao = (tGameObjectMain*)currElem->objectPtr;
                    if (!TAB_TableElemIsInvalid(currElem->objectPtr))
                    {
                        if ((currGao->flags & 0x1) == 0)
                            break;

                        if (currGao != gao)
                        {
                            WOR_World_DetachObject(WOR_gpst_CurrentWorld, (tGameObjectMain*)currElem->objectPtr);
                            OBJ_GameObject_Remove(currGao, TRUE);
                        }
                    }
                }
            }
            LOA_DeleteAddress((int)gao->extended->grp);
            OBJ_FreeGroup(gao->extended->grp);
        }
        gao->extended->grp = NULL;
        gao->flags &= ~0x800000;
    }
}

struct sGroup* GRP_pst_CreateNewGroup()
{
    tGroup* grp;
    tableDescStruct* td;

    grp = (tGroup*)MEM_p_Alloc(sizeof(tGroup));
    
    grp->tablePF = NULL;
    grp->unk2 = 0;
    grp->groupElemNum = 0;
    
    td = (tableDescStruct*)MEM_p_Alloc(sizeof(tableDescStruct));
    grp->tablePF = td;
    TAB_PFtable_Init(td, 0, 0.1f);
    
    return grp;
}

// megadja  egy csoport n.ik elemhéz a másik csoport n-edik elemét
tGameObjectMain* GRP_pst_CorresGAO(tGroup* group1, tGroup* group2, tGameObjectMain* gaoOrGaoID)
{
    tGameObjectMain* retGao;
    tableElemStructPF* currElem;
    tableElemStructPF* lastElem;
    int twoGroupMemoryPosDistance;

    if (gaoOrGaoID == (tGameObjectMain*)1)
        return (tGameObjectMain*)1;
    if (gaoOrGaoID == (tGameObjectMain*)2)
        return (tGameObjectMain*)2;

    currElem = group1->tablePF->firstElemPtr;
    lastElem = ((tableElemStructPF*)(group1->tablePF->nextElemPtr)) - 1;
    
    if (currElem > lastElem)
        return NULL;
    
    // megnézi a két csoport közötti távolságot az elsõ elem ptr alapján, és így ki tudja számolni hozzá a másik csoportban lévõ gao helyét                                                                          
    twoGroupMemoryPosDistance = (char*)group2->tablePF->firstElemPtr - (char*)currElem; 
    
    while (1)
    {
        retGao = *(tGameObjectMain**)((char*)&currElem->objectPtr + twoGroupMemoryPosDistance);
        if (gaoOrGaoID == currElem->objectPtr)
            break;
        if (++currElem > lastElem)
            return NULL;
    }

    return retGao;
}

void GRP_RepercuteHierarchy(tGroup* srcGroup, tGroup* destGroup)
{
    tableElemStructPF* currElemG2;
    tableElemStructPF* currElemG1;
    tableElemStructPF* lastElem;
    tGameObjectMain* currGao1;
    tGameObjectMain* currGao2;
    tGameObjectMain* refGao;

    lastElem = ((tableElemStructPF*)(srcGroup->tablePF->nextElemPtr)) - 1;
    for (currElemG1 = srcGroup->tablePF->firstElemPtr, currElemG2 = destGroup->tablePF->firstElemPtr; currElemG1 <= lastElem; currElemG1++, currElemG2++)
    {
        currGao1 = (tGameObjectMain*)currElemG1->objectPtr;
        currGao2 = (tGameObjectMain*)currElemG2->objectPtr;
        if (!TAB_TableElemIsInvalid(currElemG1->objectPtr) && (currGao1->flags & 0x400000) != 0)
        {
            if (currGao1->basic->mtx->refGao)
            {
                refGao = (tGameObjectMain*)GRP_pst_CorresGAO(srcGroup, destGroup, currGao1->basic->mtx->refGao);
                currGao2->basic->mtx->refGao = refGao;
                currGao2->basic->mtx->refGao2 = refGao;
            }
        }

    }
}

void GRP_DuplicateGroupObjects(struct sWorld* world, struct sGroup* srcGroup, struct sGroup* destGroup)
{
    tableElemStructPF* lastElem, *currElem;
    tGameObjectMain* gao, *gao2;

    destGroup->unk2 = srcGroup->unk2;

    if (srcGroup->tablePF->firstElemPtr)
    {
        lastElem = ((tableElemStructPF*)(srcGroup->tablePF->nextElemPtr)) - 1;
        for (currElem = srcGroup->tablePF->firstElemPtr; currElem <= lastElem; currElem++)
        {
            gao = (tGameObjectMain*)currElem->objectPtr;
            if (!TAB_TableElemIsInvalid(gao))
            {
                gao2 = OBJ_GameObject_Duplicate(world, gao, 1, 0, 0);
                gao2->flags &= ~0x20000;
                OBJ_AddInGroup(destGroup, gao2);
            }
        }
        GRP_RepercuteHierarchy(srcGroup, destGroup);
    }
}

struct sGroup* GRP_ul_LoadGrpCallback(uint32_t offset)
{
    tGroup* grp;
    int golKey;
    char* currReadPtr;
    int size;

    grp = (tGroup*)MEM_p_Alloc(sizeof(tGroup));
    
    grp->tablePF = NULL;
    grp->unk2 = 0;
    grp->groupElemNum = 0;
    
    currReadPtr = BIG_pc_ReadFileTmp(offset, &size);
    golKey = LOA_ReadLong(&currReadPtr, 0, 1);
    
    LOA_MakeFileRef(golKey, &grp->tablePF, OBJ_ul_World_LoadGolCallback, 1);
    
    grp->unk2 = LOA_ReadLong(&currReadPtr, 0, 1);
    
    LOA_ReadLong(&currReadPtr, 0, 2);
    LOA_ReadLong(&currReadPtr, 0, 2);
    
    return grp;
}