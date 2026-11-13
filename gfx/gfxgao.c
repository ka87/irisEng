#include "../memory.h"
#include "gfxgao.h"

struct sGfxGaoData* GFX_Gao_Create(struct sGameObjectMain* gao)
{
    tGfxGaoData* gfx;

    if (!gao || !gao->basic || !gao->basic->actorObjectOrVisualObject)
        return 0;

    gfx = (tGfxGaoData*)MEM_p_Alloc(sizeof(tGfxGaoData));
    gfx->gao = gao;
    ((tGameObjectVisualObject*)(gao->basic->actorObjectOrVisualObject))->renderOnlyWithGFXRender = 1;
    gfx->focale = 0.0f;
    return gfx;
}

void GFX_Gao_Render(struct sGfxGaoData* gfx)
{
    float savedFocale;
    int overrideFocale;

    savedFocale = 0.0f;
    overrideFocale = (*(uint32_t*)&gfx->focale != 0);
    if (overrideFocale)
    {
        savedFocale = gpst_DX_stDD->ct.cam.focale;
        gpst_DX_stDD->ct.cam.focale = gfx->focale;
        gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    }

    if ((gfx->gao->flags2 & 2) != 0)
        GRO_Render(gfx->gao);

    if (overrideFocale)
    {
        gpst_DX_stDD->ct.cam.focale = savedFocale;
        gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    }
}

void GFX_Gao_Destroy(struct sGfxGaoData* gfx)
{
    ((tGameObjectVisualObject*)(gfx->gao->basic->actorObjectOrVisualObject))->renderOnlyWithGFXRender = 0;
}
