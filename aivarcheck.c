#include "aivarcheck.h"

#if AIVARCHK_ENABLED

#include "common.h"
#include "ai.h"
#include "dbg.h"

static unsigned int AIVARCHK_gul_Instances;
static int AIVARCHK_gb_Active;

/* The declared size of a type, and whether the index is one the type
   table can answer for at all. The table is small and the ids stored
   in a chunk are much larger, so an unmapped id reads past the end --
   which would give a copy length that is neither right nor obviously
   wrong. */
static int AIVARCHK_TypeSize(int typeUsed, int* inRange)
{
    *inRange = (typeUsed >= 0
                && typeUsed < (int)(sizeof(AI_gast_Types) / sizeof(AI_gast_Types[0])));
    return *inRange ? AI_gast_Types[typeUsed].dataTypeSize : -1;
}

static const char* AIVARCHK_Name(const AIVarNameElem* names, unsigned int idx)
{
    return names ? names[idx].varName : "(no names)";
}

/* The four bytes at an offset, read three ways, because which one is
   meaningful depends on the variable's type and the whole point is
   that the type may not be the one intended. */
static void AIVARCHK_Word(const char* what, const byte* base,
                          unsigned int offset, unsigned int limit)
{
    unsigned int raw;
    float asFloat;

    if (!base || offset + 4 > limit)
    {
        DBG_OutputDebugString("AIVARCHK     %s: offset %u is outside the"
                              " %u-byte block\n", what, offset, limit);
        return;
    }
    memcpy(&raw, &base[offset], 4);
    memcpy(&asFloat, &base[offset], 4);
    DBG_OutputDebugString("AIVARCHK     %s at %u: %08x = %d as int,"
                          " %f as float\n", what, offset, raw, (int)raw,
                          asFloat);
}

static int AIVARCHK_IsWatched(const char* name)
{
    static const char* watch[] = AIVARCHK_WATCH;
    unsigned int i;

    if (!watch[0])
        return 1;
    for (i = 0; i < sizeof(watch) / sizeof(watch[0]) && watch[i]; i++)
        if (!_stricmp(watch[i], name))
            return 1;
    return 0;
}

void AIVARCHK_Begin(struct sAI* ai)
{
    tAI* inst = (tAI*)ai;
    unsigned int i;

    AIVARCHK_gb_Active = 0;
    if (!inst || !inst->model || !inst->model->vars || !inst->varsInstance)
        return;
    if (AIVARCHK_gul_Instances >= AIVARCHK_MAX_INSTANCES)
        return;

    for (i = 0; i < inst->model->vars->varNum; i++)
    {
        if (AIVARCHK_IsWatched(AIVARCHK_Name(inst->model->vars->varNames, i)))
        {
            AIVARCHK_gb_Active = 1;
            break;
        }
    }
    if (!AIVARCHK_gb_Active)
        return;

    ++AIVARCHK_gul_Instances;
    DBG_OutputDebugString("AIVARCHK instance %u: model declares %u"
                          " variables in %u bytes, instance block sets %u"
                          " in %u bytes, model resolved %d\n",
                          AIVARCHK_gul_Instances,
                          inst->model->vars->varNum,
                          inst->model->vars->varFullSizeToAlloc,
                          inst->varsInstance->varNum,
                          inst->varsInstance->varFullSizeToAlloc,
                          (int)inst->model->isResolved);
}

void AIVARCHK_Pair(struct sAI* ai, unsigned int modelIdx,
                   unsigned int instIdx, unsigned int modelOffset,
                   unsigned int instOffset, int typeUsed,
                   unsigned int dims, unsigned int bytes)
{
    tAI* inst = (tAI*)ai;
    const char* modelName;
    const char* instName;
    int inRange;
    int size;

    if (!AIVARCHK_gb_Active || !inst || !inst->model || !inst->varsInstance)
        return;

    modelName = AIVARCHK_Name(inst->model->vars->varNames, modelIdx);
    instName  = AIVARCHK_Name(inst->varsInstance->varNames, instIdx);
    if (!AIVARCHK_IsWatched(modelName))
        return;

    size = AIVARCHK_TypeSize(typeUsed, &inRange);
    DBG_OutputDebugString("AIVARCHK   %s [model %u, offset %u, type %d,"
                          " count %08x]  <-  %s [instance %u, offset %u,"
                          " type %d, count %08x]\n",
                          modelName, modelIdx, modelOffset,
                          (int)inst->model->vars->variables[modelIdx].variableType,
                          inst->model->vars->variables[modelIdx].variableArrayCount,
                          instName, instIdx, instOffset,
                          (int)inst->varsInstance->variables[instIdx].variableType,
                          inst->varsInstance->variables[instIdx].variableArrayCount);

    if (_stricmp(modelName, instName))
        DBG_OutputDebugString("AIVARCHK     *** the two names differ, so"
                              " this value is going to the wrong"
                              " variable\n");
    if (!inRange)
        DBG_OutputDebugString("AIVARCHK     *** type %d is outside the type"
                              " table, so the copy length is not a declared"
                              " size\n", typeUsed);

    DBG_OutputDebugString("AIVARCHK     type used %d, declared size %d,"
                          " dims %u, copied %u bytes\n",
                          typeUsed, size, dims, bytes);
    AIVARCHK_Word("read", inst->varsInstance->varData, 4 * dims + instOffset,
                  inst->varsInstance->varFullSizeToAlloc);
    AIVARCHK_Word("wrote", inst->instanceVarDataInitial, 4 * dims + modelOffset,
                  inst->model->vars->varFullSizeToAlloc);
    AIVARCHK_Word("model default", inst->model->vars->varData,
                  4 * dims + modelOffset,
                  inst->model->vars->varFullSizeToAlloc);
}

void AIVARCHK_End(struct sAI* ai, int processedVarNum)
{
    tAI* inst = (tAI*)ai;
    unsigned int i, j;

    if (!AIVARCHK_gb_Active || !inst || !inst->model || !inst->varsInstance)
        return;

    /* An instance variable the loop never matched keeps the model's
       default, so its value is lost without anything saying so. */
    for (j = 0; j < inst->varsInstance->varNum; j++)
    {
        const char* instName = AIVARCHK_Name(inst->varsInstance->varNames, j);

        for (i = 0; i < inst->model->vars->varNum; i++)
            if (!_stricmp(AIVARCHK_Name(inst->model->vars->varNames, i),
                          instName))
                break;
        if (i == inst->model->vars->varNum)
            DBG_OutputDebugString("AIVARCHK   *** %s [instance %u] has no"
                                  " variable of that name in the model, so"
                                  " its value was dropped\n", instName, j);
    }

    DBG_OutputDebugString("AIVARCHK instance %u done: applied %d of %u\n",
                          AIVARCHK_gul_Instances, processedVarNum,
                          inst->varsInstance->varNum);
    AIVARCHK_gb_Active = 0;
}

#endif
