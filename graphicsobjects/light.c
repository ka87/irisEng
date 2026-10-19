#include "../memory.h"
#include "../loading.h"
#include "../gdi.h"
#include "../time.h"
#include "light.h"

tGroGeneric* LIGHT_pst_Create()
{
    tLIGHT* lgt = (tLIGHT*)MEM_p_Alloc(sizeof(tLIGHT));

    memset(lgt, 0, sizeof(tLIGHT));
    lgt->flags = 0x2928;
    lgt->color = 0xffffff;
    lgt->fnear = 2.0f;
    lgt->ffar = 5.0f;
    lgt->density = 0.1f;
    lgt->i8 = 0.2f;
    GRO_Struct_Init((tGroGeneric*)lgt, 2);
    return (tGroGeneric*)lgt;
}

tGroGeneric* LIGHT_p_CreateFromBuffer(struct sGroGeneric* gro, char** readptr, struct sWorld *world)
{
    int gaoKey;
    tLIGHT* lgt = (tLIGHT*)LIGHT_pst_Create();

    lgt->flags = LOA_ReadLong(readptr, 0, 1);
    lgt->color = LOA_ReadLong(readptr, 0, 1);
    lgt->fnear = LOA_ReadFloat(readptr, 0, 1);
    lgt->ffar = LOA_ReadFloat(readptr, 0, 1);
    lgt->density = LOA_ReadFloat(readptr, 0, 1);
    lgt->i8 = LOA_ReadFloat(readptr, 0, 1);
    gaoKey = LOA_ReadLong(readptr, 0, 1);
    if (gaoKey == -1)
        lgt->lightGao = NULL;
    else
        LOA_MakeFileRef(gaoKey, &lgt->lightGao, OBJ_ul_GameObjectCallback, 1);
    return (tGroGeneric*)lgt;
}

struct sGroGeneric* LIGHT_Duplicate(tGroGeneric* a1)
{
    return 0;
}

void LIGHT_Free(tGroGeneric* gro)
{
    if (gro && gro->intf->id == 2 && gro->usageCount <= 0)
    {
        LOA_DeleteAddress((int)gro);
        MEM_Free(gro);
    }
}

BOOL LIGHT_l_HasSomethingToRender(struct sGameObjectVisualObject* gaovo, struct sGroGeneric** gro)
{
    return gpst_DX_stDD->presentationFlags & 2;
}

void LIGHT_Init()
{
    unsigned int i, tmp;

    GRO_gast_Interface[2].Create = LIGHT_pst_Create;
    GRO_gast_Interface[2].CreateFromBuffer = LIGHT_p_CreateFromBuffer;
    GRO_gast_Interface[2].Duplicate = LIGHT_Duplicate;
    GRO_gast_Interface[2].Destroy = LIGHT_Free;
    GRO_gast_Interface[2].HasSomethingToRender = LIGHT_l_HasSomethingToRender;
    
    if (!LIGHT_gpul_CurrentTable)
    {
        for (i = 0; i < 256; i++)
        {
            tmp = (((i * i) / 256) * ((i * i) / 256)) / 256;
            LIGHT_gaul_LinearTable[i] = i;
            LIGHT_gaul_ExponantTable[i] = (tmp * tmp) / 256;
        }
    }
}
