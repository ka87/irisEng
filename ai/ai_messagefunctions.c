#include "ai_messagefunctions.h"
#include "../dbg.h"

void AI_EvalFunc_MSGSend_C(struct sGameObjectMain* a1, struct sMessage* Src)
{
    tGameObjectMessages* v2;

    if (a1->extended && a1->extended->messages)
    {
        v2 = a1->extended->messages;
        if (v2->count < 5u)
        {
            memcpy(&v2->msgs[v2->count], Src, sizeof(v2->msgs[v2->count]));
            ++v2->count;
        }
    }
}

tAINode* AI_EvalFunc_MSGSend(tAINode* arg0)
{
    tGameObjectMain* v2;
    tMessage dest;
    tVarStackElem a2;
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    AI_GetVarFromStack(&dest, &a2);
    /* Temporary: report what each script sends to whom. */
    {
        tMessage* m = (tMessage*)a2.ptr;
        if (m)
            DBG_OutputDebugString(
                "AI: MSG_Send beh 0x%08x -> gao %p  id %d  sender %p gao2 %p "
                "v1(%f %f %f) v2(%f %f %f) i1 %d i2 %d f1 %f f2 %f\n",
                AI_gpst_CurrentFunction ? AI_gpst_CurrentFunction->key : 0,
                (void*)a1, m->msgID, (void*)m->gao, (void*)m->gao2,
                m->v1.x, m->v1.y, m->v1.z, m->v2.x, m->v2.y, m->v2.z,
                m->Int1, m->Int2, m->f1, m->f2);
    }
    AI_EvalFunc_MSGSend_C(a1, (tMessage*)a2.ptr);
    return arg0 + 1;
}

struct sMessage* AI_EvalFunc_MSGGet_C(struct sGameObjectMain* a1)
{
    tMessage* v2;
    tGameObjectMessages* v3;

    v3 = a1->extended->messages;
    if (v3->count > 0)
        return &v3->msgs[--v3->count];
    v2 = &v3->msgs[v3->count];
    v2->msgID = 0;
    return v2;
}

struct sMessage* AI_EvalFunc_MSGGet_C_CURRENT()
{
    return AI_EvalFunc_MSGGet_C(AI_gpst_CurrentGameObject);
}

tAINode* AI_EvalFunc_MSGGet(tAINode* arg0)
{
    tGameObjectMain* v2;
    tVarStackElem a1;
    tMessage* v4;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    v4 = AI_EvalFunc_MSGGet_C(v2);
    a1.type = AI_gaw_EnumLink[41];
    a1.ptr = (char*)v4;
    AI_EvalVar(&a1);
    return arg0 + 1;
}

void AI_EvalFunc_MSGClear_C(struct sGameObjectMain* a1)
{
    a1->extended->messages->count = 0;
}

tAINode* AI_EvalFunc_MSGClear(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MSGClear_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_MSGOn_C(tGameObjectMain* a1)
{
    int a2;

    if ((a1->flags & 0x8000) == 0)
    {
        if (a1)
            a2 = a1->flags;
        else
            a2 = 0;
        OBJ_ChangeIdentityFlags(a1, a2 | 0x8000, -1);
    }
}

tAINode* AI_EvalFunc_MSGOn(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MSGOn_C(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MSGSendEx(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;
    tMessage Src;
    int v5;
    tGameObjectMessages* v6;
    tGameObjectMain* v7;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v2;
    if (!v2->extended)
        return a1 + 1;
    if (!v7->extended->messages)
        return a1 + 1;
    v3 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v6 = v7->extended->messages;
    memset(&Src, 0, sizeof(Src));
    Src.gao = AI_gpst_CurrentGameObject;
    Src.msgID = v5;
    Src.f2 = (float)v3;
    if (v6->count < 5u)
    {
        memcpy(&v6->msgs[v6->count], &Src, sizeof(v6->msgs[v6->count]));
        ++v6->count;
    }
    return a1 + 1;
}

struct sMessage* AI_EvalFunc_MSGGetByID_C(struct sGameObjectMain* a1, int a2)
{
    tMessage* v3;
    char Src[52];
    int i;
    tGameObjectMessages* v6;

    v6 = a1->extended->messages;
    if (v6->count > 0)
    {
        for (i = 0; i < v6->count; ++i)
        {
            if (v6->msgs[i].msgID == a2)
            {
                if (i != v6->count - 1)
                {
                    memcpy(Src, &v6->msgs[i], sizeof(Src));
                    memcpy(&v6->msgs[i], &v6->msgs[v6->count - 1], sizeof(v6->msgs[i]));
                    memcpy(&v6->msgs[v6->count - 1], Src, sizeof(v6->msgs[v6->count - 1]));
                    i = v6->count - 1;
                }
                v3 = &v6->msgs[i];
                --v6->count;
                return v3;
            }
        }
    }
    v3 = &gst_Lifo;
    gst_Lifo.msgID = 0;
    return v3;
}

tAINode* AI_EvalFunc_MSGGetByID(tAINode* arg0)
{
    tGameObjectMain* v2;
    int a2;
    tVarStackElem a1;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    a2 = AI_GetIntFromStack();
    a1.ptr = (char*)AI_EvalFunc_MSGGetByID_C(v2, a2);
    a1.type = AI_gaw_EnumLink[41];
    AI_EvalVar(&a1);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_MSGSendAll(tAINode* a1)
{
    tGameObjectMain* v2;
    tMessage Src;
    tGameObjectMessages* v4;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    if (!v2->extended)
        return a1 + 1;
    if (!v5->extended->messages)
        return a1 + 1;
    v4 = v5->extended->messages;
    Src.Int2 = AI_GetIntFromStack();
    Src.Int1 = AI_GetIntFromStack();
    Src.f1 = AI_GetFloatFromStack();
    Src.f2 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.v2 = *(Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.v1 = *(Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.gao2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.gao = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    Src.msgID = AI_GetIntFromStack();
    if (v4->count < 5u)
    {
        memcpy(&v4->msgs[v4->count], &Src, sizeof(v4->msgs[v4->count]));
        ++v4->count;
    }
    return a1 + 1;
}

int AI_EvalFunc_MSGTestInt1_C(tGameObjectMain* a1, int a2, int a3)
{
    int i;
    tGameObjectMessages* v5;

    if (!a1->extended)
        return 0;
    if (!a1->extended->messages)
        return 0;
    v5 = a1->extended->messages;
    for (i = 0; i < v5->count; ++i)
    {
        if (v5->msgs[i].msgID == a2 && (a3 & v5->msgs[i].Int1) != 0)
            return 1;
    }
    return 0;
}

tAINode* AI_EvalFunc_MSGTestInt1(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a3;
    int a2;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_MSGTestInt1_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

struct sMessage* AI_EvalFunc_MSGRead_C(struct sGameObjectMain* a1, int a2)
{
    tGameObjectMessages* v3;
    tMessage* v4;

    v3 = a1->extended->messages;
    if (a2 < v3->count)
        return &v3->msgs[a2];
    v4 = &gst_Lifo;
    gst_Lifo.msgID = 0;
    return v4;
}

tAINode* AI_EvalFunc_MSGRead(tAINode* arg0)
{
    tGameObjectMain* v2;
    int a2;
    tVarStackElem v4; 
    tGameObjectMain* a1;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a1 = v2;
    a2 = AI_GetIntFromStack();
    v4.type = AI_gaw_EnumLink[41];
    v4.ptr = (char*)AI_EvalFunc_MSGRead_C(v2, a2);
    AI_EvalVar(&v4);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_MSGReadByID(tAINode* arg0)
{
    tGameObjectMain* v2;
    int v3;
    tVarStackElem a1;
    int i;
    tGameObjectMessages* v6;
    tGameObjectMain* v7;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v7 = v2;
    v3 = AI_GetIntFromStack();
    v6 = v2->extended->messages;
    if (v6->count > 0)
    {
        for (i = 0; i < v6->count; ++i)
        {
            if (v6->msgs[i].msgID == v3)
            {
                a1.ptr = (char*)&v6->msgs[i];
                goto LABEL_8;
            }
        }
    }
    a1.ptr = (char*)&gst_Lifo;
    gst_Lifo.msgID = 0;
LABEL_8:
    a1.type = AI_gaw_EnumLink[41];
    AI_EvalVar(&a1);
    return arg0 + 1;
}

int AI_EvalFunc_MSGGetCount_C(struct sGameObjectMain* a1)
{
    if (!a1->extended)
        return 0;
    if (a1->extended->messages)
        return a1->extended->messages->count;
    return 0;
}

tAINode* AI_EvalFunc_MSGGetCount(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_MSGGetCount_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_MSGSendAllOwner(tAINode* a1)
{
    tGameObjectMain* v2;
    tMessage Src;
    tGameObjectMessages* v4;
    tGameObjectMain* v5;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v5 = v2;
    if (!MSG_gpo_Owner)
        return a1 + 1;
    if (!MSG_gpo_Owner->extended)
        return a1 + 1;
    if (!MSG_gpo_Owner->extended->messages)
        return a1 + 1;
    v4 = MSG_gpo_Owner->extended->messages;
    Src.Int2 = AI_GetIntFromStack();
    Src.Int1 = AI_GetIntFromStack();
    Src.f1 = AI_GetFloatFromStack();
    Src.f2 = AI_GetFloatFromStack();
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.v2 = *(Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.v1 = *(Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    Src.gao2 = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    Src.gao = AI_gpst_CurrentGameObject;
    Src.msgID = AI_GetIntFromStack();
    if (v4->count < 5u)
    {
        memcpy(&v4->msgs[v4->count], &Src, sizeof(v4->msgs[v4->count]));
        ++v4->count;
    }
    return a1 + 1;
}

void AI_EvalFunc_MSGRegOwner_C(struct sGameObjectMain* a1)
{
    MSG_gpo_Owner = a1;
}

tAINode* AI_EvalFunc_MSGRegOwner(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    MSG_gpo_Owner = v2;
    return a1 + 1;
}