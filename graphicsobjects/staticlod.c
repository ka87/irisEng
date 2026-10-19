#include "../memory.h"
#include "../loading.h"
#include "staticlod.h"

tGEOSTATICLOD* GEO_pst_StaticLOD_Create(uint8_t lodNum)
{
    tGEOSTATICLOD* lod = (tGEOSTATICLOD*)MEM_p_Alloc(sizeof(tGEOSTATICLOD));
    memset(lod, 0, sizeof(tGEOSTATICLOD));
    
    GRO_Struct_Init((tGroGeneric*)lod, 8);
    
    lod->lodNum = lodNum;

    if (lodNum > 6)
        lod->lodNum = 6;

    return lod;
}

struct sGEOGRO* GEO_pst_StaticLOD_GetLOD(struct sGEOSTATICLOD* lod, uint8_t distance)
{
    int i;

    for (i = 0; i < lod->lodNum - 1; i++)
    {
        if (lod->distance[i] < distance)
            break;
    }
    return lod->geo[i];
}

void GEO_StaticLOD_FreeContent(tGEOSTATICLOD* lod)
{
    int i;

    if (!lod) return;
    
    for (i = 0; i < lod->lodNum; i++) 
    {
        lod->geo[i]->intf->AddRef((tGroGeneric*)lod->geo[i], -1);
        lod->geo[i]->intf->Destroy((tGroGeneric*)lod->geo[i]);
    }
}

void GEO_StaticLOD_Free(struct sGroGeneric* lod)
{
    if (lod && lod->intf->id == 8 && lod->usageCount <= 0)
    {
        GEO_StaticLOD_FreeContent((tGEOSTATICLOD*)lod);
        LOA_DeleteAddress((int)lod);
        MEM_Free(lod);
    }
}

void GEO_StaticLOD_Render(tGameObjectMain* gao)
{
    tGameObjectVisualObject* vo;
    tGEOSTATICLOD* lod;
    tGEOGRO* geo;

    vo = gao->basic->actorObjectOrVisualObject;
    lod = (tGEOSTATICLOD*)(vo->GROGEOOrSomethingElse);
    geo = GEO_pst_StaticLOD_GetLOD(lod, lod->currDistance);
 
    vo->GROGEOOrSomethingElse = (tGroGeneric*)geo;
    geo->intf->Render(gao);
    vo->GROGEOOrSomethingElse = (tGroGeneric*)lod;
}

tGroGeneric* GEO_p_StaticLOD_CreateFromBuffer(struct sGroGeneric* gro, char** readptr, struct sWorld *world)
{
    int i, key;
    tGEOSTATICLOD* lod = GEO_pst_StaticLOD_Create(0);

    lod->lodNum = LOA_ReadChar(readptr, 0, 1);
    
    if (lod->lodNum > 6)
        lod->lodNum = 6;

    LOA_ReadChar(readptr, 0, 2);
    
    for (i = 0; i < 6; i++)
        lod->distance[i] = LOA_ReadChar(readptr, 0, 1);

    for (i = 0; i < lod->lodNum; i++) {
        key = LOA_ReadLong(readptr, 0, 1);
        LOA_MakeFileRef(key, (void**)&(lod->geo[i]), GEO_ul_Load_ObjectCallback, 1);
    }

    return (tGroGeneric*)lod;
}

int GEO_l_StaticLOD_GetIndex(tGEOSTATICLOD* a1, uint8_t distance)
{
    int i;

    for (i = 0; i < a1->lodNum - 1; i++)
    {
        if (a1->distance[i] < distance)
            break;
    }
    return i;
}

BOOL GEO_l_StaticLOD_HasSomethingToRender(struct sGameObjectVisualObject* gaovo, struct sGroGeneric** gro)
{
    tGEOSTATICLOD* lod;
    BOOL ret;
    tGEOGRO* geo;

    gpst_DX_stDD->drawMaskAllowedStuffCurrent = gaovo->objDrawMask & gpst_DX_stDD->drawMaskAllowedStuff;
    lod = (tGEOSTATICLOD*)gaovo->GROGEOOrSomethingElse;
    lod->currDistance = gpst_DX_stDD->currTargetGao->distance;
    if ((geo = lod->geo[GEO_l_StaticLOD_GetIndex(lod, lod->currDistance)]) == NULL) ret = FALSE;

    *gro = (tGroGeneric*)geo;

    if (geo != NULL)
        ret = (gpst_DX_stDD->drawMaskAllowedStuffCurrent & 8) ? TRUE : FALSE;
    return ret;
}

void GEO_StaticLOD_Init()
{
    GRO_gast_Interface[8].CreateFromBuffer = GEO_p_StaticLOD_CreateFromBuffer;
    GRO_gast_Interface[8].Destroy = GEO_StaticLOD_Free;
    GRO_gast_Interface[8].HasSomethingToRender = GEO_l_StaticLOD_HasSomethingToRender;
    GRO_gast_Interface[8].Render = GEO_StaticLOD_Render;
}