#include "../memory.h"
#include "gfxgao.h"

struct sGfxGaoData* GFX_Gao_Create(struct sGameObjectMain* a1)
{
    tGameObjectBasic* v1;
    tGfxGaoData* result;

    if (!a1)
        return 0;
    v1 = a1->basic;
    if (!v1 || !v1->actorObjectOrVisualObject)
        return 0;
    result = (tGfxGaoData*)MEM_p_Alloc(sizeof(tGfxGaoData));
    result->gao = a1;
    ((tGameObjectVisualObject*)(a1->basic->actorObjectOrVisualObject))->renderOnlyWithGFXRender = 1;
    result->focale = 0.0f;
    return result;
}

void draw_game_object_in_GFX_sub_45DBB0(struct sGfxGaoData* a1)
{
    float v2;
    tGameObjectMain* v3;
    float v4;

    if (*(uint32_t *)&(a1->focale) != 0)
    {
        v2 = gpst_DX_stDD->ct.cam.focale;
        gpst_DX_stDD->ct.cam.focale = a1->focale;
        v4 = v2;
        gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
        v3 = a1->gao;
        if ((v3->flags2 & 2) != 0)
            GRO_Render(v3);
        gpst_DX_stDD->ct.cam.focale = v4;
        gpst_DX_stDD->SetProjectionMatrix(&gpst_DX_stDD->ct.cam);
    }
    else if ((a1->gao->flags2 & 2) != 0)
    {
        GRO_Render(a1->gao);
    }
}

void GFX_Gao_Destroy(struct sGfxGaoData* a1)
{
    ((tGameObjectVisualObject*)(a1->gao->basic->actorObjectOrVisualObject))->renderOnlyWithGFXRender = 0;
}