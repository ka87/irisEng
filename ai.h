#pragma once
#include "common.h"
#include "gameobject.h"
#include "textget.h"
#include "math.h"

typedef struct sAINode
{
    int32_t val;
    int16_t val2;
    char flags;
    char category;
} tAINode;

typedef struct sAILocal
{
    int offset;
    int type;
    char name[30];
    int16_t _pad;
} tAILocal;

typedef struct sVarStackElem
{
    __int16 type;
    byte _gap[2];
    char* ptr;
} tVarStackElem;

typedef struct sOfc_scriptengine_functions_types
{
    int16_t funcid;
    int16_t dataTypeSize;
    void (* funcptr)(void *, void *);
} tOfc_scriptengine_functions_types;

typedef struct sOfc_scriptengine_functions
{
    int16_t funcid;
    int16_t dataTypeSize;
    struct sAINode* (*funcptr)(struct sAINode *);
} tOfc_scriptengine_functions;

typedef struct AIVarElem
{
    int varOffset;
    unsigned int variableArrayCount;
    __int16 variableType;
    __int16 flags;
} AIVarElem;

typedef struct AIVarNameElem
{
    char varName[30];
} AIVarNameElem;

typedef struct sAIFunction
{
    struct sAINode* nodes;
    int stackOffsetNum; // function local stack size
    char* localStringConstsData; // string consts used in the function or an offset if ainode.val2 = 38; basically it's used for string format specifiers
    __int16 usageCount;
    byte pad[2];
    int(*funcPtr)();
    int varsResolved;
    uint32_t key;   // the behaviour's key, for diagnostics
} tAIFunction;

typedef struct sAIVars
{
    byte* varData;
    unsigned int varFullSizeToAlloc;
    AIVarElem* variables;
    unsigned int varNum;
    AIVarNameElem* varNames;
    struct sAIFunction* callBacks[5];
    int instanceFuncNum;
    struct sAIFunction** instanceFuncs;
} tAIVars;

typedef struct sAIModel
{
    struct sAIVars* vars;
    struct sAIFunction** funcs;
    unsigned int funcNum;
    int16_t instanceCnt;
    byte isResolved;
    byte isDisabled;
} tAIModel;

typedef struct sAiCBElemStruct
{
    struct sAIFunction* functionData;
    int id;
    struct sGameObjectMain* gao;
} tAiCBElemStruct;

typedef struct sAI
{
    struct sAIModel* model;
    struct sAINode* instanceJumpNodes[5];
    struct sAIFunction* instanceCallBacks[5];
    float instanceCallBackTimeRemaining[5];
    byte* instanceVarDataInitial;
    byte* instanceVarDataCurrent;
    int instanceAICBListNum;
    struct sAiCBElemStruct* instanceAICBList;
    struct sAIVars* varsInstance;   
    uint16_t end;
    uint16_t start;
} tAI;

typedef struct sAIVarUnk
{
    byte _gap1[4];
    int32_t i1;
    int32_t i2;
    byte _gap2[8];
} tAIVarUnk;


unsigned int AI_gi_SaveBufferSize;
byte* AI_gpc_SaveBuffer;

int AI_C_Callback;

struct sVarStackElem AI_gast_GlobalVarStruct[10240];
char AI_gac_GlobalStack[10240];
int AI_gul_SizeGlobalStack;
int AI_gul_SizeGlobalVarStack;
int AI_gul_SizeUserStack;
int AI_gul_SizeUserVarStack;
char AI_gast_LocalVarStack[10240];
int AI_gul_SizeLocalStack;

struct sVarStackElem AI_gast_UserVarStack[1024];
char AI_gac_UserStack[1024];

extern tOfc_scriptengine_functions_types AI_gast_Types[23];
extern tOfc_scriptengine_functions AI_gast_Categs[16];
extern tOfc_scriptengine_functions AI_gast_Keywords[74];
extern tOfc_scriptengine_functions AI_gast_Functions[848];
extern tOfc_scriptengine_functions AI_gast_Fields[27];

extern int AI_CompiledAIEnabled;
extern int AIUnivIsRunning;
extern float AI_gf_Epsilon;

int16_t AI_gaw_EnumLink[15815]; // 0x3dc7 => legmagasabb Evalfunction ID!!!

struct sAI* AI_gpst_CurrentInstance;
struct sAI* AI_gpst_CurrentInstanceUltra;
struct sGameObjectMain* AI_gpst_CurrentUltra;
struct sGameObjectMain* AI_gpst_CurrentGameObject;
struct sAINode** AI_gppst_CurrentJumpNode;
struct sAIFunction* AI_gpst_CurrentFunction;
int AI_gul_CurrentTrack;
int AI_gi_RunFctLevel;

int AI_GetIntFromStack();
int AI_PutIntToStack(int a1);
float AI_GetFloatFromStack();
int AI_PutFloatToStack(float a1);
Vec3* AI_GetVecFromStack(Vec3* a1);
void AI_PutVecToStack(Vec3* a1);
void AI_GetVarFromStack(void* dest, tVarStackElem* a2);
char* AI_PushUserStack(void* a1, int16_t a2);
void AI_PopUserStack(void* a1, tVarStackElem* a2);
void AI_PutObjectToStack(void* a1, __int16 type);
void AI_EvalVar(tVarStackElem* a1);
void AI_EvalVarArray(tAINode* a1, char* a2);
void AI_EvalVarArray2(tAINode* a1, char* a2);
void AI_EvalVarArray3(tAINode* a1, char* a2);
void AI_InitVarArray(tAINode* a1, char* a2);
char* AI_EvalVarArray_C3(char* a1, int a2);
char* AI_EvalVarArray_C7(char* a1, int a2);
char* AI_EvalVarArray2_C(int a1, char* a2, int a3, int a4);
char* AI_EvalVarArray_C2(char* a1, int a2);
char* AI_EvalVarArray_C(int a1, char* a2, int num);
void AI_RunNode(tAINode* a1);
void AI_FreeInstance(tAI* a1);
BOOL AI_SpecialWParam(char a1);
BOOL AI_b_IsARef(int a1);
int AI_ul_ResolveOneRef(int a1, int a2);
void AI_ResolveInstanceRef(tAI* a1);
void AI_ReinitInstance(tAI* a1);
void AI_DuplicateInstance(struct sGameObjectMain* a1, struct sGameObjectMain* a2);
void AI_ExecCallback(struct sGameObjectMain* a1, int id);
void AI_ExecCallbackAll(struct sWorld* a1, int a2);
void AI_MainCall(struct sWorld* a1);
void AI_Reset();
int AI_RunInstance(struct sAI* a1);
void AI_ReinitUniverse();
struct sAI* AI_pst_LoadInstance(int key);
struct sAI* AI_ul_CallbackLoadInstance(uint32_t Offset);
void AI_InitModule();
tAINode* AI_pst_RunFunction(tAIFunction* funcPtr, tAINode** jumpNode, int stackDecrease);

//for compiled functions
unsigned int AI_ul_RealResolveOneRef(int type, unsigned int key);
unsigned int AI_ul_RealResolveOneRefObject(unsigned int a1);
unsigned int AI_ul_RealResolveOneRefFunction(unsigned int key);
BOOL AI_EvalFunc_AIIsModel_C(struct sGameObjectMain* a1, struct sAIModel* a2);
void AI2C_ResolveBuffer(uint32_t* a1, void* a2, int a3);
void AI2C_ResolveBufferByName(uint32_t* varPtr, void* gaoOrGaoIdxOrAIPtr, char* name);
void* AI2C_ResolveBufferPtr(void* gaoOrGaoIdxOrAIPtr);
struct sGameObjectMain* AI2C_CheckActorGao(struct sGameObjectMain* gao);
void* AI2C_VarGetFromAI(struct sAI* aiPtr, char* name);
struct sGameObjectMain* AI2C_GaoGetFromAI(struct sAI* aiPtr, char* name);
void AI2C_IntSetFromAI(struct sAI* aiPtr, char* name, int val);
void AI2C_FloatSetFromAI(struct sAI* aiPtr, char* name, float val);
void AI2C_VecSetFromAI(struct sAI* aiPtr, char* name, Vec3* val);
void AI2C_GaoSetFromAI(struct sAI* aiPtr, char* name, struct sGameObjectMain* val);
void AI2C_TextGetSetFromAI(struct sAI* aiPtr, char* name, struct sTextGet* val);
void* AI2C_VarArrayGetFromAI(struct sAI* aiPtr, char* name, int idx);
void* AI2C_VarArrayC3GetFromAI(struct sAI* aiPtr, char* name, int idx);
void AI2C_IntArraySetFromAI(struct sAI* aiPtr, char* name, int idx, int val);
void AI2C_FloatArraySetFromAI(struct sAI* aiPtr, char* name, int idx, float val);
struct sAI* AI2C_GetAIModelFromGao(void* gaoOrGaoIdxOrAIPtr);
int AI_EvalKeyword_During_C();
void AI2C_PushInt(int a1);
char* AI2C_PushFloat(float a1);
char* AI2C_PushVector(Vec3* a1);
Vec3* AI2C_PopVector(Vec3* a1);
char* AI2C_PushGameObject(struct sGameObjectMain* a1);
struct sGameObjectMain* AI2C_PopGameObject();
int AI2C_PopInt();
float AI2C_PopFloat();
BOOL AI2C_VecEqNull(Vec3* a1);
struct sGameObjectMain* AI2C_GetMainActor(void* a1);
void AI2C_AssignVector(Vec3* a1, Vec3* a2);
void AI2C_MultiplyVectorWithScalar(Vec3* a1, Vec3* a2, float a3);
void AI2C_AddVector(Vec3* a1, Vec3* a2, Vec3* a3);
float AI2C_VectorLen(Vec3* a1);
void AI2C_SubstractVector(Vec3* dest, Vec3* a2, Vec3* a3);
void AI2C_NormalizeEqVector(Vec3* a1);
float AI2C_DotProductVector(Vec3* a1, Vec3* a2);
void AI2C_NegateVector(Vec3* a1, Vec3* a2);
BOOL AI2C_DistOfTwoPointsIsZero(Vec3* a2, Vec3* a3);

tAINode* AI_dum(tAINode* a1);
char* get_a_string_from_globalstack_sub_4D2040();

// FIXME, TODO: TEMP, should me moved to somewhere else
void AI_EvalType_GetGAO(struct sGameObjectMain** a1, struct sGameObjectMain** a2);

#define GAOKEY2AI(key) AI2C_GetAIModelFromGao((void*)AI_ul_RealResolveOneRefObject(key))
#define GAORESOLVE(key) (struct sGameObjectMain*)AI_ul_RealResolveOneRefObject(key)

#define AI2IG *(int*)AI2C_VarGetFromAI
#define AI2IGPtr (int*)AI2C_VarGetFromAI
#define AI2FG *(float*)AI2C_VarGetFromAI
#define AI2VG (Vec3*)AI2C_VarGetFromAI
#define AI2GG AI2C_GaoGetFromAI
#define AI2TG (tTextGet*)AI2C_VarGetFromAI
#define AI2IAG *(int*)AI2C_VarArrayGetFromAI
#define AI2FAG *(float*)AI2C_VarArrayGetFromAI
#define AI2VAG (Vec3*)AI2C_VarArrayGetFromAI
#define AI2VAGC3 (Vec3*)AI2C_VarArrayC3GetFromAI
#define AI2IS AI2C_IntSetFromAI
#define AI2FS AI2C_FloatSetFromAI
#define AI2VS AI2C_VecSetFromAI
#define AI2GS AI2C_GaoSetFromAI
#define AI2TS AI2C_TextGetSetFromAI
#define AI2IAS AI2C_IntArraySetFromAI
#define AI2FAS AI2C_FloatArraySetFromAI