#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../gdi.h"
#include "../tables.h"
#include "../time.h"
#include "gro_common.h"

tGroIntf GRO_gast_Interface[12];

void GRO_Struct_Init(tGroGeneric* gro, int type)
{
    gro->intf = &GRO_gast_Interface[type];
    gro->usageCount = 0;
}

int GRO_l_Struct_Load(tGroGeneric* gro, char* readptr)
{
    char* readPtrBak;
    unsigned int groType;
    int additionalLength;

    readPtrBak = readptr;

    groType = LOA_ReadLong(&readptr, 0, 1);
    if (groType >= 12)
        groType = 0;
    
    gro->intf = &GRO_gast_Interface[groType];
    gro->usageCount = 0;
    additionalLength = LOA_ReadLong(&readptr, 0, 2);

    return (int)readptr - (int)readPtrBak + additionalLength;
}

tGroGeneric* GRO_p_UnknownCreateDefault()
{
    tGroGeneric* gro = (tGroGeneric*)MEM_p_Alloc(sizeof(tGroGeneric));

    gro->intf = &GRO_gast_Interface[0];
    gro->usageCount = 0;

    return gro;
}

tGroGeneric* GRO_p_UnknownCreateFromBuffer(tGroGeneric* gro, char** readptr, void* world)
{
    return GRO_p_UnknownCreateDefault();
}

void GRO_UnknownDestroy(tGroGeneric* gro)
{
    if (gro->usageCount <= 0)
    {
        LOA_DeleteAddress((int)gro);
        MEM_Free(gro);
    }
}

tGroGeneric* GRO_UnknownDuplicate(tGroGeneric* gro)
{
    return NULL;
}

BOOL GRO_l_UnknowHasSomethingToRender(tGameObjectVisualObject* gaovo, tGroGeneric** gro)
{
    if ((gpst_DX_stDD->presentationFlags & 0x20) == 0)
        return FALSE;
    
    *gro = NULL;
    
    return TRUE;
}

void GRO_UnknownAddRef(tGroGeneric* gro, int refCnt)
{
    gro->usageCount += refCnt;
}

void GRO_Unknown_Reinit(tGroGeneric* gro) {

}

int GRO_UnknownGetValidatedMask(void* geoMat, int materialID)
{
    return 0;
}

void GRO_Struct_InitInterfaces()
{
    tGroIntf* intf = GRO_gast_Interface;
    int i;

    for (i = 0; i < 12; i++)
    {
        intf->id = i;
        intf->Create = GRO_p_UnknownCreateDefault;
        intf->CreateFromBuffer = GRO_p_UnknownCreateFromBuffer;
        intf->Duplicate = GRO_UnknownDuplicate;
        intf->Destroy = GRO_UnknownDestroy;
        intf->HasSomethingToRender = GRO_l_UnknowHasSomethingToRender;
        intf->Render = NULL;
        intf->AddRef = GRO_UnknownAddRef;
        intf->Reinit = GRO_Unknown_Reinit;
        intf->GetValidatedMask = GRO_UnknownGetValidatedMask;
        ++intf;
    }
    GRO_gast_last = intf;
}

tGroGeneric* GRO_pst_Struct_Create(int intfIdx)
{
    return GRO_gast_Interface[intfIdx].Create();
}

struct sGroGeneric* GEO_ul_Load_ObjectCallback(uint32_t offset)
{
    BOOL isSpecial;
    int resetReadMode;
    tGroGeneric* groStruct;
    char* data;
    int size;
    tGroGeneric gro;

    isSpecial = LOA_IsSpecial(LOA_ul_GetCurrentKey());
    resetReadMode = 0;
    GEO_b_IsInFix = isSpecial;
    
    if (BIG_gi_ReadMode == 1 && !BIG_gi_SpecialRead && isSpecial)
    {
        resetReadMode = 1;
        BIG_gi_ReadMode = 0;
    }

    data = BIG_pc_ReadFileTmp(offset, &size);
    data += GRO_l_Struct_Load(&gro, data);

    groStruct = gro.intf->CreateFromBuffer(&gro, &data, WOR_gpst_WorldToLoadIn);
    
    if ((!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2) && gro.intf->id == 1)
    {
        COL_OK3_Build((tGameObjectMain*)groStruct, 0, 0);
    }
    if (isSpecial)
        groStruct->usageCount = 1000000;

    if (resetReadMode)
        BIG_gi_ReadMode = 1;

    return groStruct;
}

void GRO_Struct_DestroyTable(tableDescStruct* tbl)
{
    tableElemStructP* currTableElem;
    tableElemStructP* lastTableElem;
    int usageCnt;

    lastTableElem = ((tableElemStructP*)(tbl->nextElemPtr)) - 1;
    for (currTableElem = tbl->firstElemPtr; currTableElem <= lastTableElem; ++currTableElem)
    {
        ((tGroGeneric*)(currTableElem->objectPtr))->intf->AddRef(currTableElem->objectPtr, -1);
        if (ENG_gb_ExitApplication)
        {
            usageCnt = ((tGroGeneric*)(currTableElem->objectPtr))->usageCount;
            if (usageCnt >= 1000000)
                ((tGroGeneric*)(currTableElem->objectPtr))->usageCount = usageCnt - 1000000;
        }
        if (!((tGroGeneric*)(currTableElem->objectPtr))->usageCount)
        {
            ((tGroGeneric*)(currTableElem->objectPtr))->intf->Destroy(currTableElem->objectPtr);
            currTableElem->objectPtr = NULL;
        }
    }
    TAB_Ptable_Close(tbl);
}

void GRO_PushLookAtMatrix(tGdi_DisplayData* dd, tGameObjectMain* gao)
{
    Vec3 banking;
    Vec3 sight;
    Mtx4x4 rotMtx;

    PRO_PerfCounterStartBlock(&dd->rasters[23]);

    sight.x = gao->matrices->Pos.x - dd->ct.cam.objectMtx.Pos.x;
    sight.y = gao->matrices->Pos.y - dd->ct.cam.objectMtx.Pos.y;
    sight.z = gao->matrices->Pos.z - dd->ct.cam.objectMtx.Pos.z;
    banking.x = dd->ct.cam.objectMtx.v2.x * -1.0f;
    banking.y = dd->ct.cam.objectMtx.v2.y * -1.0f;
    banking.z = dd->ct.cam.objectMtx.v2.z * -1.0f;

    MATH_MakeRotationMatrix_UsingSight(&rotMtx, &sight, &banking, 1);
    
    banking.x = rotMtx.v2.x * -1.0f;
    banking.y = rotMtx.v2.y * -1.0f;
    banking.z = rotMtx.v2.z * -1.0f;
    
    rotMtx.v2.x = rotMtx.v3.x;
    rotMtx.v2.y = rotMtx.v3.y;
    rotMtx.v2.z = rotMtx.v3.z;

    rotMtx.v3.x = banking.x;
    rotMtx.v3.y = banking.y;
    rotMtx.v3.z = banking.z;

    rotMtx.Pos = gao->matrices->Pos;
    rotMtx.mtxflag |= 0x2;

    if ((OBJ_pst_GetAbsoluteMatrix(gao)->mtxflag & 0x8) != 0)
    {
        MATH_GetScale(&banking, OBJ_pst_GetAbsoluteMatrix(gao));
        MATH_SetScale(&rotMtx, &banking);
    }

    PRO_PerfCounterEndBlock(&dd->rasters[23]);

    PRO_PerfCounterStartBlock(&dd->rasters[24]);
    SOFT_l_MatrixStack_Push(&dd->matrixStack, &rotMtx);
    PRO_PerfCounterEndBlock(&dd->rasters[24]);
}

void GRO_Render(struct sGameObjectMain* gao)
{
    tGdi_DisplayData* dd = gpst_DX_stDD;
    tGameObjectVisualObject* gaovo = (tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject);
    tGEOGRO* geogro = (tGEOGRO*)(gaovo->GROGEOOrSomethingElse);
    int zListBufferIdx;
    tGroGeneric* groRet = NULL;
 
    if (geogro && geogro->intf && geogro->intf->Render)
    {
        zListBufferIdx = gaovo->boneDisplayOrder_zListBufferNum + 4;
        if (zListBufferIdx > 7)
            zListBufferIdx = 7;
        p_Current_SOFT_gst_ZList = &SOFT_gpc_BigBufferZlists[zListBufferIdx];
        QueryPerformanceCounter(&PerfCounterStruct[24]);

        dd->flags &= 0xC8804;
        dd->drawMaskAllowedStuffCurrent = gaovo->objDrawMask & dd->drawMaskAllowedStuff;
        dd->currTargetGao = gao;
        dd->GROGEO = NULL;
        dd->bigBuffer->flags = 0;
        if (geogro->intf->HasSomethingToRender(gaovo, &groRet))
        {
            ++(dd->rasters[3].unk2);
            if ((dd->presentationFlags & 0x80) != 0)
            {
                PRO_PerfCounterStartBlock(&dd->rasters[24]);
                SOFT_l_MatrixStack_Push(&dd->matrixStack, OBJ_pst_GetAbsoluteMatrix(gao));
                PRO_RecordTimeDecRef(&dd->rasters[24]);
                
                PRO_PerfCounterStartBlock(&dd->rasters[25]);
                dd->ct.cam.viewMtxPtr_setInGRORender = dd->matrixStack.actMatrixElem;
                dd->SetViewMatrix(dd->matrixStack.actMatrixElem);
                PRO_RecordTimeDecRef(&dd->rasters[25]);

                SOFT_l_MatrixStack_Pop(&dd->matrixStack);
            }
            else
            {
                if ((gao->flags2 & 0x800000) != 0)
                {
                    GRO_PushLookAtMatrix(dd, gao);
                }
                else
                {
                    PRO_PerfCounterStartBlock(&dd->rasters[24]);
                    SOFT_l_MatrixStack_Push(&dd->matrixStack, OBJ_pst_GetAbsoluteMatrix(gao));
                    PRO_RecordTimeDecRef(&dd->rasters[24]);
                }
                
                PRO_PerfCounterStartBlock(&dd->rasters[25]);
                
                dd->ct.cam.viewMtxPtr_setInGRORender = dd->matrixStack.actMatrixElem;
                dd->SetViewMatrix(dd->matrixStack.actMatrixElem);
                
                PRO_RecordTimeDecRef(&dd->rasters[25]);
                PRO_PerfCounterStartBlock(&dd->rasters[26]);
                
                PRO_QPC_RecordTimeDecRef(24);
                geogro->intf->Render(gao);
                QueryPerformanceCounter(&PerfCounterStruct[24]);
                
                PRO_RecordTimeDecRef(&dd->rasters[26]);

                SOFT_l_MatrixStack_Pop(&dd->matrixStack);
            }
        }

        dd->flags &= ~0x400;
        p_Current_SOFT_gst_ZList = SOFT_gpc_BigBufferZlists + 4;
        PRO_QPC_RecordTimeDecRef(24);
    }
}