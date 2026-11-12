#include "ai_messagefunctions.h"

// A game object holds at most five pending messages. Every sender in
// this file ends the same way, so the queue push is one function.
static void MSG_l_Push(tGameObjectMessages* messages, const tMessage* msg)
{
    if (messages->count < 5u)
    {
        memcpy(&messages->msgs[messages->count], msg, sizeof(messages->msgs[0]));
        ++messages->count;
    }
}

// The reader functions all hand back the same cleared global when they
// find nothing, which is how a script sees "no message".
static tMessage* MSG_l_NoMessage(void)
{
    gst_Lifo.msgID = 0;
    return &gst_Lifo;
}

static int MSG_l_FindByID(tGameObjectMessages* messages, int msgID)
{
    int i;

    for (i = 0; i < messages->count; ++i)
    {
        if (messages->msgs[i].msgID == msgID)
            return i;
    }
    return -1;
}

// The seven arguments MSGSendAll and MSGSendAllOwner share, popped in
// the order the two have them.
static void MSG_l_PopCommonFields(tMessage* msg)
{
    msg->Int2 = AI_GetIntFromStack();
    msg->Int1 = AI_GetIntFromStack();
    msg->f1 = AI_GetFloatFromStack();
    msg->f2 = AI_GetFloatFromStack();
    msg->v2 = *(Vec3*)AI_PopStackSlot();
    msg->v1 = *(Vec3*)AI_PopStackSlot();
    msg->gao2 = (tGameObjectMain*)AI_GetObjectFromStack();
}

void AI_EvalFunc_MSGSend_C(struct sGameObjectMain* gao, struct sMessage* msg)
{
    if (gao->extended && gao->extended->messages)
        MSG_l_Push(gao->extended->messages, msg);
}

tAINode* AI_EvalFunc_MSGSend(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tMessage converted;
    tVarStackElem var;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    AI_GetVarFromStack(&converted, &var);
    AI_EvalFunc_MSGSend_C(gaoUltra, (tMessage*)var.ptr);
    return node + 1;
}

struct sMessage* AI_EvalFunc_MSGGet_C(struct sGameObjectMain* gao)
{
    tGameObjectMessages* messages;

    messages = gao->extended->messages;
    if (messages->count > 0)
        return &messages->msgs[--messages->count];

    // an empty queue reads as the slot one past its end, cleared
    messages->msgs[messages->count].msgID = 0;
    return &messages->msgs[messages->count];
}

struct sMessage* AI_EvalFunc_MSGGet_C_CURRENT()
{
    return AI_EvalFunc_MSGGet_C(AI_gpst_CurrentGameObject);
}

tAINode* AI_EvalFunc_MSGGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tVarStackElem var;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    var.type = AI_gaw_EnumLink[41];
    var.ptr = (char*)AI_EvalFunc_MSGGet_C(gaoUltra);
    AI_EvalVar(&var);
    return node + 1;
}

void AI_EvalFunc_MSGClear_C(struct sGameObjectMain* gao)
{
    gao->extended->messages->count = 0;
}

tAINode* AI_EvalFunc_MSGClear(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MSGClear_C(gaoUltra);
    return node + 1;
}

void AI_EvalFunc_MSGOn_C(tGameObjectMain* gao)
{
    if ((gao->flags & 0x8000) == 0)
        OBJ_ChangeIdentityFlags(gao, gao->flags | 0x8000, -1);
}

tAINode* AI_EvalFunc_MSGOn(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MSGOn_C(gaoUltra);
    return node + 1;
}

tAINode* AI_EvalFunc_MSGSendEx(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tMessage msg;
    int msgID;
    int value;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (!gaoUltra->extended || !gaoUltra->extended->messages)
        return node + 1;

    value = AI_GetIntFromStack();
    msgID = AI_GetIntFromStack();

    memset(&msg, 0, sizeof(msg));
    msg.gao = AI_gpst_CurrentGameObject;
    msg.msgID = msgID;
    msg.f2 = (float)value;
    MSG_l_Push(gaoUltra->extended->messages, &msg);
    return node + 1;
}

struct sMessage* AI_EvalFunc_MSGGetByID_C(struct sGameObjectMain* gao, int msgID)
{
    tGameObjectMessages* messages;
    tMessage swap;
    int found;

    messages = gao->extended->messages;
    found = MSG_l_FindByID(messages, msgID);
    if (found == -1)
        return MSG_l_NoMessage();

    // the match is popped, so it changes places with the last one first
    if (found != messages->count - 1)
    {
        memcpy(&swap, &messages->msgs[found], sizeof(swap));
        memcpy(&messages->msgs[found], &messages->msgs[messages->count - 1], sizeof(swap));
        memcpy(&messages->msgs[messages->count - 1], &swap, sizeof(swap));
        found = messages->count - 1;
    }
    --messages->count;
    return &messages->msgs[found];
}

tAINode* AI_EvalFunc_MSGGetByID(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tVarStackElem var;
    int msgID;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    msgID = AI_GetIntFromStack();
    var.ptr = (char*)AI_EvalFunc_MSGGetByID_C(gaoUltra, msgID);
    var.type = AI_gaw_EnumLink[41];
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalFunc_MSGSendAll(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tMessage msg;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (!gaoUltra->extended || !gaoUltra->extended->messages)
        return node + 1;

    MSG_l_PopCommonFields(&msg);
    msg.gao = (tGameObjectMain*)AI_GetObjectFromStack();
    msg.msgID = AI_GetIntFromStack();
    MSG_l_Push(gaoUltra->extended->messages, &msg);
    return node + 1;
}

int AI_EvalFunc_MSGTestInt1_C(tGameObjectMain* gao, int msgID, int mask)
{
    tGameObjectMessages* messages;
    int i;

    if (!gao->extended || !gao->extended->messages)
        return 0;

    messages = gao->extended->messages;
    for (i = 0; i < messages->count; ++i)
    {
        if (messages->msgs[i].msgID == msgID && (mask & messages->msgs[i].Int1) != 0)
            return 1;
    }
    return 0;
}

tAINode* AI_EvalFunc_MSGTestInt1(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int msgID;
    int mask;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    mask = AI_GetIntFromStack();
    msgID = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_MSGTestInt1_C(gaoUltra, msgID, mask));
    return node + 1;
}

struct sMessage* AI_EvalFunc_MSGRead_C(struct sGameObjectMain* gao, int index)
{
    tGameObjectMessages* messages;

    messages = gao->extended->messages;
    if (index < messages->count)
        return &messages->msgs[index];
    return MSG_l_NoMessage();
}

tAINode* AI_EvalFunc_MSGRead(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tVarStackElem var;
    int index;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    index = AI_GetIntFromStack();
    var.type = AI_gaw_EnumLink[41];
    var.ptr = (char*)AI_EvalFunc_MSGRead_C(gaoUltra, index);
    AI_EvalVar(&var);
    return node + 1;
}

tAINode* AI_EvalFunc_MSGReadByID(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tGameObjectMessages* messages;
    tVarStackElem var;
    int msgID;
    int found;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    msgID = AI_GetIntFromStack();
    messages = gaoUltra->extended->messages;

    // the read leaves the message where it is, unlike MSGGetByID
    found = MSG_l_FindByID(messages, msgID);
    if (found != -1)
        var.ptr = (char*)&messages->msgs[found];
    else
        var.ptr = (char*)MSG_l_NoMessage();

    var.type = AI_gaw_EnumLink[41];
    AI_EvalVar(&var);
    return node + 1;
}

int AI_EvalFunc_MSGGetCount_C(struct sGameObjectMain* gao)
{
    if (!gao->extended || !gao->extended->messages)
        return 0;
    return gao->extended->messages->count;
}

tAINode* AI_EvalFunc_MSGGetCount(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_MSGGetCount_C(gaoUltra));
    return node + 1;
}

tAINode* AI_EvalFunc_MSGSendAllOwner(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    tMessage msg;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;

    if (!MSG_gpo_Owner || !MSG_gpo_Owner->extended || !MSG_gpo_Owner->extended->messages)
        return node + 1;

    MSG_l_PopCommonFields(&msg);
    msg.gao = AI_gpst_CurrentGameObject;
    msg.msgID = AI_GetIntFromStack();
    MSG_l_Push(MSG_gpo_Owner->extended->messages, &msg);
    return node + 1;
}

void AI_EvalFunc_MSGRegOwner_C(struct sGameObjectMain* gao)
{
    MSG_gpo_Owner = gao;
}

tAINode* AI_EvalFunc_MSGRegOwner(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_MSGRegOwner_C(gaoUltra);
    return node + 1;
}
