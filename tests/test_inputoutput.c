#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_mocks.h"
#include "../memory.h"
#include "../time.h"
#include "../ai.h"
#include "../dbg.h"
#include "../win/wininputoutput.h"
#include "../inputoutput.h"

/* The frozen copy and the rewrite in one binary. Half of
   inputoutput.c's functions are not in its header, so they are
   declared here the way both copies define them. */
#define BOTH(ret, name, args) ret BASE_##name args; ret NEW_##name args;

BOTH(void, INO_Joystick_Init, (HWND hwnd))
BOTH(void, INO_SavInitModule, (void))
BOTH(void, INO_KeyboardInit, (void))
BOTH(void, INO_InitModule, (void))
BOTH(int, INO_i_SetCurrentPad, (int pad))
BOTH(int, INO_i_GetCurrentPad, (void))
BOTH(int, INO_SimulateReset, (void))
BOTH(void, INO_Joystick_Close, (void))
BOTH(void, INO_Joystick_Update, (void))
BOTH(int, INO_i_PifPafEnable, (int enable))
BOTH(void, INO_PifSet, (int duration))
BOTH(void, INO_PafSet, (int intensity, int duration))
BOTH(int, INO_b_Joystick_IsButtonDown, (int button))
BOTH(BOOL, INO_b_Joystick_IsButtonJustDown, (int button))
BOTH(BOOL, INO_b_Joystick_IsButtonJustUp, (int button))
BOTH(void, INO_Joystick_Move, (Vec3* move, int stickID))
BOTH(void, INO_JoySimulate, (int m, int b, Vec3* l, Vec3* r))
BOTH(void, INO_SetControlSubmode, (int submode))
BOTH(void, INO_SetControlMode, (int mode))
BOTH(void, INO_SetMouseSensitivity, (float s))
BOTH(void, INO_KeyboardClose, (void))
BOTH(int, INO_SavCloseModule, (void))
BOTH(void, INO_CloseModule, (void))
BOTH(void, INO_Keyboard_Update, (void))
BOTH(void, INO_Reinit, (void))
BOTH(BOOL, INO_b_LanguageIsPresent, (int lang))
BOTH(int, INO_i_SavGetCardStatus, (int slot, int* buttons))
BOTH(int, INO_b_IsKeyPressed, (uint8_t key))
BOTH(BOOL, INO_b_IsKeyJustPressed, (uint8_t key))
BOTH(BOOL, INO_b_IsKeyReleased, (uint8_t key))
BOTH(BOOL, INO_b_IsKeyJustReleased, (uint8_t key))
BOTH(int, INO_i_GetDiscError, (void))
BOTH(int, INO_b_IoButtonPressed, (int button))
BOTH(void, SAV_PhotoFirstInit, (void))
BOTH(void, SAV_PhotoInit, (void))
BOTH(BOOL, SAV_b_IsMoreRecent, (int a, int b))
BOTH(void, SAV_OrderData, (void))
BOTH(void, INO_v_MemoryCardManager, (int slot))
BOTH(void, INO_Update, (void))
BOTH(void, INO_SavSetCardStatus, (int slot, int status))
BOTH(void*, SAV_PhotoReinit, (void))
BOTH(void, SAV_PhotoReinitValidate, (void))
BOTH(void, SAV_PhotoClose, (void))
BOTH(void, SAV_PhotoLastClose, (void))
BOTH(void, SAV_PhotoSet, (int photo, void* src, size_t size))
BOTH(void, SAV_PhotoDel, (int photo))
BOTH(void, SAV_PhotoDecal, (int photo, int count))
BOTH(char*, SAV_PhotoGet, (int photo, int32_t* size))
BOTH(void, SAV_PhotoValidate, (void))
BOTH(void, SAV_PhotoUnvalidate, (void))
BOTH(void, SAV_LoadData, (int forSaving))
BOTH(int, Demo_Geti, (int field))
BOTH(int, Demo_Seti, (int field))
BOTH(int, INO_i_IsMouseBeingUsed, (void))

/* The globals inputoutput.c defines itself. INO_ge_SavState has no
   initialiser, so it is a tentative definition the two copies share
   like the rest; the eight below are initialised, so each copy has its
   own and the driver reaches whichever side is running through SV. */
extern int INO_ge_SavState;

#define SIDED(name) extern int BASE_##name; extern int NEW_##name;
SIDED(INO_ge_SavSetNextState)
SIDED(INO_ge_SavSetNextNextState)
SIDED(INO_ge_SavCommand)
SIDED(INO_gi_LaunchDemo)
SIDED(Photo_gi_FirstInit)
SIDED(Photo_gi_Init)
SIDED(INO_ge_MasterType)
extern int BASE_INO_gai_PresentLanguage[12];
extern int NEW_INO_gai_PresentLanguage[12];

#define SV(name) (*(side ? &NEW_##name : &BASE_##name))
#define SVA(name) (side ? NEW_##name : BASE_##name)

/* ------------------------------------------------------------------ */

static int s_cases;
static int s_fails;
static const char* s_group = "";

#define AI_SAVE_BYTES 256
static uint8_t s_aiSaveBuffer[AI_SAVE_BYTES];

typedef struct
{
    long ret;
    float retF;
    int logLen;
    tIoMockCall log[IOMOCK_LOG_MAX];
    size_t arenaUsed;

    int savState, savNext, savNextNext, savCommand, savStateDummy;
    int lastError, launchDemo, firstInit, photoInit, discError, freeSpace;
    int joyMask, joyDir, joyEnable, padId, pifPafEnable, pafIntensity;
    float pifDuration, pafDuration;
    int masterType, readIdx, demoExit, engExit, engForceEnd;
    int savPhotosSize, defaultSlot;
    int savButton[8];
    char savFileName[128];
    tJoySimulateCurrentStruct asked, current;
    tInoPad currPad[4], prevPad[4];
    tSavIdxElem data[10];
    tSavSlotDesc slotDesc;
    int idxFields[8];
    tPhotoStruct photos, photosValidate;
    long fileDescFp, fileDescBuf;
    int fileDescSize;
    long buffer304, keys, keysPrev, validateBuffer;
    int presentLang[12];
    uint8_t keyBytes[256], keyPrevBytes[256];
    char keybUnk[128];
    uint8_t aiSave[AI_SAVE_BYTES];
    Vec3 outVec;
    int outInts[8];
} tIoState;

static tIoState s_base;
static tIoState s_new;
static unsigned char* s_baseArena;

static Vec3 s_outVec;
static int s_outInts[8];

static void snapshot(tIoState* s, long ret, int side)
{
    memset(s, 0, sizeof(*s));
    s->ret = ret;
    s->logLen = IOMOCK_logLen;
    memcpy(s->log, IOMOCK_log, sizeof(s->log));
    s->arenaUsed = IOMOCK_used;

    s->savState = INO_ge_SavState;
    s->savNext = SV(INO_ge_SavSetNextState);
    s->savNextNext = SV(INO_ge_SavSetNextNextState);
    s->savCommand = SV(INO_ge_SavCommand);
    s->lastError = pcINO_gi_LastError;
    s->launchDemo = SV(INO_gi_LaunchDemo);
    s->firstInit = SV(Photo_gi_FirstInit);
    s->photoInit = SV(Photo_gi_Init);
    s->discError = INO_gi_DiscError;
    s->freeSpace = INO_gi_FreeSpace;
    s->joyMask = INO_l_JoystickMask;
    s->joyDir = INO_l_JoystickDir;
    s->joyEnable = INO_l_Joystick_Enable;
    s->padId = INO_gi_CurrentPadId;
    s->pifPafEnable = INO_gi_PifPafEnable;
    s->pafIntensity = INO_gi_PafIntensity;
    s->pifDuration = INO_gi_PifDuration;
    s->pafDuration = INO_gi_PafDuration;
    s->masterType = SV(INO_ge_MasterType);
    s->readIdx = INO_gb_ReadIdxForSaving;
    s->demoExit = Demo_gb_Exit;
    s->engExit = ENG_gb_ExitApplication;
    s->engForceEnd = ENG_gb_ForceEndEngine;
    s->savPhotosSize = pcINO_SAVPhotosSize;
    s->defaultSlot = SAVE_gi_DefaultSlot;
    memcpy(s->savButton, INO_gi_SavButton, sizeof(s->savButton));
    memcpy(s->savFileName, INO_gsz_SavFileName, sizeof(s->savFileName));
    s->asked = INO_gst_JoySimulateAsked;
    s->current = INO_gst_JoySimulateCurrent;
    memcpy(s->currPad, INO_gst_CurrPad, sizeof(s->currPad));
    memcpy(s->prevPad, INO_gst_PrevPad, sizeof(s->prevPad));
    memcpy(s->data, SAV_gst_Data, sizeof(s->data));
    s->slotDesc = INO_gst_SavSlotDesc;
    s->idxFields[0] = SAV_idx_Field_i2;
    s->idxFields[1] = SAV_idx_Field_i3;
    s->idxFields[2] = SAV_idx_Field_i4;
    s->idxFields[3] = SAV_idx_Field_i5;
    s->idxFields[4] = SAV_idx_Field_i6;
    s->idxFields[5] = SAV_idx_Field_i7;
    s->idxFields[6] = SAV_idx_Field_i8;
    s->idxFields[7] = SAV_idx_Field_i9;
    s->photos = SAV_gst_Photos;
    s->photosValidate = SAV_gst_PhotosValidate;
    s->fileDescFp = (long)(intptr_t)INO_gst_SavFileDesc.fp;
    s->fileDescBuf = IOMOCK_Offset(INO_gst_SavFileDesc.saveBuffer);
    s->fileDescSize = INO_gst_SavFileDesc.size;
    s->buffer304 = IOMOCK_Offset(pc_Buffer_304);
    s->keys = IOMOCK_Offset(INO_Keys);
    s->keysPrev = IOMOCK_Offset(INO_Keys_Prev);
    s->validateBuffer = IOMOCK_Offset(SAV_pc_PhotoValidateBuffer);
    memcpy(s->presentLang, SVA(INO_gai_PresentLanguage), sizeof(s->presentLang));
    if (INO_Keys)
        memcpy(s->keyBytes, INO_Keys, 256);
    if (INO_Keys_Prev)
        memcpy(s->keyPrevBytes, INO_Keys_Prev, 256);
    memcpy(s->keybUnk, INO_keybUnkDataProbablyUnused, sizeof(s->keybUnk));
    memcpy(s->aiSave, s_aiSaveBuffer, AI_SAVE_BYTES);
    s->outVec = s_outVec;
    memcpy(s->outInts, s_outInts, sizeof(s->outInts));
}

static void compare(int k)
{
    int i;
    int bad = 0;

    ++s_cases;
    if (s_base.logLen != s_new.logLen)
    {
        printf("  %s[%d]: call count %d vs %d\n", s_group, k, s_base.logLen, s_new.logLen);
        bad = 1;
    }
    else
    {
        for (i = 0; i < s_base.logLen && i < IOMOCK_LOG_MAX; ++i)
        {
            if (strcmp(s_base.log[i].name, s_new.log[i].name) != 0
                || s_base.log[i].a != s_new.log[i].a
                || s_base.log[i].b != s_new.log[i].b
                || strcmp(s_base.log[i].text, s_new.log[i].text) != 0)
            {
                printf("  %s[%d]: call %d  %s(%ld,%ld)\"%s\" vs %s(%ld,%ld)\"%s\"\n",
                       s_group, k, i,
                       s_base.log[i].name, s_base.log[i].a, s_base.log[i].b, s_base.log[i].text,
                       s_new.log[i].name, s_new.log[i].a, s_new.log[i].b, s_new.log[i].text);
                bad = 1;
                break;
            }
        }
    }

    /* Everything but the log in one go: the struct is laid out so that
       a field added here is compared without anything else changing. */
    {
        size_t off = offsetof(tIoState, arenaUsed);
        if (memcmp((char*)&s_base + off, (char*)&s_new + off, sizeof(tIoState) - off) != 0)
        {
            printf("  %s[%d]: state differs\n", s_group, k);
            bad = 1;
        }
        if (s_base.ret != s_new.ret)
            printf("    return %ld vs %ld\n", s_base.ret, s_new.ret);
        if (s_base.savState != s_new.savState)
            printf("    savState %d vs %d\n", s_base.savState, s_new.savState);
        if (s_base.savNext != s_new.savNext)
            printf("    savNextState %d vs %d\n", s_base.savNext, s_new.savNext);
        if (s_base.savNextNext != s_new.savNextNext)
            printf("    savNextNextState %d vs %d\n", s_base.savNextNext, s_new.savNextNext);
        if (memcmp(&s_base.photos, &s_new.photos, sizeof(tPhotoStruct)) != 0)
            printf("    photo table differs\n");
    }

    if (s_base.arenaUsed == s_new.arenaUsed
        && memcmp(s_baseArena, IOMOCK_arena, s_base.arenaUsed) != 0)
    {
        printf("  %s[%d]: arena bytes differ\n", s_group, k);
        bad = 1;
    }

    if (bad)
        ++s_fails;
}

/* ------------------------------------------------------------------ */
/* One deterministic starting state, rebuilt before each side runs.    */

static unsigned rnd(unsigned* st)
{
    *st = *st * 1103515245u + 12345u;
    return (*st >> 16) & 0x7FFF;
}

static void ioSetup(int side, unsigned seed, int photoReady)
{
    unsigned st = seed * 2654435761u + 1;
    int i, j;

    IOMOCK_Reset();
    IOMOCK_async = 0;
    IOMOCK_mouseInUse = 1;
    IOMOCK_setPadResult = (int)(seed % 5);
    IOMOCK_joyMoveX = (float)((int)rnd(&st) - 16384) * 0.001f;
    IOMOCK_joyMoveY = (float)((int)rnd(&st) - 16384) * 0.001f;
    IOMOCK_joyMoveZ = (float)((int)rnd(&st) - 16384) * 0.001f;
    IOMOCK_now = 1000000L + (long)seed * 54321L;
    IOMOCK_retNum = 0;

    memset(&s_outVec, 0, sizeof(s_outVec));
    memset(s_outInts, 0, sizeof(s_outInts));

    INO_ge_SavState = 0;
    SV(INO_ge_SavSetNextState) = -1;
    SV(INO_ge_SavSetNextNextState) = -1;
    SV(INO_ge_SavCommand) = -1;
    pcINO_gi_LastError = 0;
    SV(INO_gi_LaunchDemo) = -1;
    INO_gi_DiscError = (int)(seed % 3);
    INO_gi_FreeSpace = (int)(seed * 17 % 100000);
    INO_l_JoystickMask = -1;
    INO_l_JoystickDir = (int)(seed % 4);
    INO_l_Joystick_Enable = 1;
    INO_gi_CurrentPadId = (int)(seed % 4);
    INO_gi_PifPafEnable = 1;
    INO_gi_PafIntensity = 0;
    INO_gi_PifDuration = 0.0f;
    INO_gi_PafDuration = 0.0f;
    SV(INO_ge_MasterType) = (int)(seed % 3);
    INO_gb_ReadIdxForSaving = 0;
    Demo_gb_Exit = 0;
    ENG_gb_ExitApplication = 0;
    ENG_gb_ForceEndEngine = 0;
    pcINO_SAVPhotosSize = 0;
    SAVE_gi_DefaultSlot = (int)(seed % 4);
    TIM_gf_dt = (float)(1 + rnd(&st) % 500) * 0.0001f;

    for (i = 0; i < 8; ++i)
        INO_gi_SavButton[i] = (int)(seed * 3 + i);
    memset(INO_gsz_SavFileName, 0, sizeof(INO_gsz_SavFileName));
    strcpy(INO_gsz_SavFileName, "start.idx");

    memset(&INO_gst_JoySimulateAsked, 0, sizeof(INO_gst_JoySimulateAsked));
    memset(&INO_gst_JoySimulateCurrent, 0, sizeof(INO_gst_JoySimulateCurrent));
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 16; ++j)
        {
            INO_gst_CurrPad[i].padData[j] = (uint8_t)rnd(&st);
            INO_gst_PrevPad[i].padData[j] = (uint8_t)rnd(&st);
        }

    for (i = 0; i < 10; ++i)
    {
        SAV_gst_Data[i].superiorAtOrdering = (char)(rnd(&st) % 2);
        SAV_gst_Data[i].sec = (char)(rnd(&st) % 60);
        SAV_gst_Data[i].min = (char)(rnd(&st) % 60);
        SAV_gst_Data[i].hour = (char)(rnd(&st) % 24);
        SAV_gst_Data[i].mday = (char)(1 + rnd(&st) % 28);
        SAV_gst_Data[i].month = (char)(rnd(&st) % 12);
        SAV_gst_Data[i].year = (int16_t)(2000 + rnd(&st) % 4);
        SAV_gst_Data[i].s1 = (int16_t)rnd(&st);
        SAV_gst_Data[i].s2 = (int16_t)rnd(&st);
        SAV_gst_Data[i].s3 = 0;
        SAV_gst_Data[i].s4 = 0;
        SAV_gst_Data[i].i1 = (int)rnd(&st);
        SAV_gst_Data[i].i9 = (int)rnd(&st);
    }
    INO_gst_SavSlotDesc.id = (int)(seed % 12);
    INO_gst_SavSlotDesc.i2 = (int)rnd(&st);
    INO_gst_SavSlotDesc.i3 = (int)rnd(&st);
    INO_gst_SavSlotDesc.i4 = (int)rnd(&st);
    SAV_idx_Field_i2 = (int)rnd(&st);
    SAV_idx_Field_i3 = (int)rnd(&st);
    SAV_idx_Field_i4 = (int)rnd(&st);
    SAV_idx_Field_i5 = (int)rnd(&st);
    SAV_idx_Field_i6 = (int)rnd(&st);
    SAV_idx_Field_i7 = (int)rnd(&st);
    SAV_idx_Field_i8 = (int)rnd(&st);
    SAV_idx_Field_i9 = (int)rnd(&st);

    /* The slot file's scratch buffer has to exist: three of the card
       states copy through it without checking. */
    INO_gst_SavFileDesc.fp = (FILE*)(intptr_t)0x1234;
    INO_gst_SavFileDesc.saveBuffer = (char*)MEM_p_Alloc(AI_SAVE_BYTES);
    INO_gst_SavFileDesc.size = AI_SAVE_BYTES;
    for (i = 0; i < AI_SAVE_BYTES; ++i)
        INO_gst_SavFileDesc.saveBuffer[i] = (char)(seed * 3 + i * 5);
    pc_Buffer_304 = 0;

    AI_gi_SaveBufferSize = AI_SAVE_BYTES;
    AI_gpc_SaveBuffer = s_aiSaveBuffer;
    for (i = 0; i < AI_SAVE_BYTES; ++i)
        s_aiSaveBuffer[i] = (uint8_t)(seed + i * 7);

    memset(pcINO_keyboardDataBackup, 0, sizeof(pcINO_keyboardDataBackup));
    memset(INO_keybUnkDataProbablyUnused, 0x55, sizeof(INO_keybUnkDataProbablyUnused));

    /* The two keyboard buffers, which most entry points expect to
       exist already. */
    INO_Keys = (uint8_t*)MEM_p_Alloc(256u);
    INO_Keys_Prev = (uint8_t*)MEM_p_Alloc(256u);
    for (i = 0; i < 256; ++i)
    {
        INO_Keys[i] = (uint8_t)rnd(&st);
        INO_Keys_Prev[i] = (uint8_t)rnd(&st);
    }

    SV(Photo_gi_FirstInit) = 0;
    SV(Photo_gi_Init) = 0;
    SAV_pc_PhotoValidateBuffer = 0;
    memset(&SAV_gst_Photos, 0, sizeof(SAV_gst_Photos));
    memset(&SAV_gst_PhotosValidate, 0, sizeof(SAV_gst_PhotosValidate));

    if (photoReady)
    {
        int at = 0;
        SAV_pc_PhotoValidateBuffer = (char*)MEM_p_Alloc(SAV_PHOTO_BUFFER_SIZE);
        SAV_gst_Photos.photoBuf = (char*)MEM_p_Alloc(SAV_PHOTO_BUFFER_SIZE);
        SAV_gst_Photos.photoBufSize = SAV_PHOTO_BUFFER_SIZE;
        SAV_gst_PhotosValidate.photoBuf = SAV_pc_PhotoValidateBuffer;
        SAV_gst_PhotosValidate.photoBufSize = SAV_PHOTO_BUFFER_SIZE;
        SV(Photo_gi_FirstInit) = 1;
        SV(Photo_gi_Init) = 1;
        for (i = 0; i < 100; ++i)
        {
            int sz = (i % 4 == 3) ? 0 : (int)(16 + rnd(&st) % 64);
            SAV_gst_Photos.elems[i].startOffset = at;
            SAV_gst_Photos.elems[i].photoSize = sz;
            for (j = 0; j < sz; ++j)
                SAV_gst_Photos.photoBuf[at + j] = (char)(i * 11 + j);
            at += sz;
        }
        SAV_gst_Photos.usedSize = at;
    }

    /* The card-manager table, built by the side under test. */
    if (side)
        NEW_INO_SavInitModule();
    else
        BASE_INO_SavInitModule();

    IOMOCK_logLen = 0;
    IOMOCK_retIdx = 0;
}

static void afterSide(int side, long ret)
{
    if (side)
    {
        snapshot(&s_new, ret, side);
    }
    else
    {
        snapshot(&s_base, ret, side);
        memcpy(s_baseArena, IOMOCK_arena, IOMOCK_used);
    }
}

/* ------------------------------------------------------------------ */

static void groupJoystick(void)
{
    int k, side;
    long ret;

    s_group = "joystick";

    /* Init, close, pad select and the simple getters. */
    for (k = 0; k < 8; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 100u + k, 0);
            if (side)
            {
                NEW_INO_Joystick_Init((HWND)(intptr_t)(0x900 + k));
                s_outInts[0] = NEW_INO_i_SetCurrentPad(k - 2);
                s_outInts[1] = NEW_INO_i_GetCurrentPad();
                s_outInts[2] = NEW_INO_SimulateReset();
                s_outInts[3] = NEW_INO_i_IsMouseBeingUsed();
                NEW_INO_Joystick_Close();
                ret = 0;
            }
            else
            {
                BASE_INO_Joystick_Init((HWND)(intptr_t)(0x900 + k));
                s_outInts[0] = BASE_INO_i_SetCurrentPad(k - 2);
                s_outInts[1] = BASE_INO_i_GetCurrentPad();
                s_outInts[2] = BASE_INO_SimulateReset();
                s_outInts[3] = BASE_INO_i_IsMouseBeingUsed();
                BASE_INO_Joystick_Close();
                ret = 0;
            }
            afterSide(side, ret);
        }
        compare(k);
    }

    /* INO_Joystick_Update over the mask, the simulation and the two
       rumble clocks, including the step that takes a clock to zero. */
    for (k = 0; k < 160; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 200u + k, 0);
            INO_l_Joystick_Enable = (k % 7 != 6);
            INO_gi_CurrentPadId = (k % 11 == 10) ? -1 : (k % 4);
            INO_l_JoystickMask = (k * 37) & 0xFFF;
            INO_gst_JoySimulateAsked.controlsMask = (k % 3 == 0) ? 0 : (k * 97) & 0x0FFF0FFF;
            INO_gst_JoySimulateAsked.buttons = (k * 53) & 0xFFF;
            INO_gst_JoySimulateAsked.amountXLeft = (float)(k % 17) * 0.0625f;
            INO_gst_JoySimulateAsked.amountYLeft = (float)(k % 13) * 0.125f;
            INO_gst_JoySimulateAsked.amountXRight = (float)(k % 11) * 0.25f;
            INO_gst_JoySimulateAsked.amountYRight = (float)(k % 7) * 0.5f;
            Demo_gb_Exit = (k % 5 == 4);
            IOMOCK_mouseInUse = (k % 4 != 3);
            TIM_gf_dt = 0.016f;
            /* a clock that ends exactly on this step, one that does
               not, and one that is already zero */
            INO_gi_PifDuration = (k % 3 == 0) ? 0.0f : (k % 3 == 1) ? 0.016f : 0.5f;
            INO_gi_PafDuration = (k % 3 == 0) ? 0.032f : (k % 3 == 1) ? 0.0f : 0.016f;
            if (side)
                NEW_INO_Joystick_Update();
            else
                BASE_INO_Joystick_Update();
            afterSide(side, 0);
        }
        compare(1000 + k);
    }

    /* The pad-button predicates, over every button index of every pad
       and the no-pad case. */
    for (k = 0; k < 96; ++k)
    {
        int button = k % 16;
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 400u + k, 0);
            INO_gi_CurrentPadId = (k % 6 == 5) ? -1 : (k % 4);
            if (side)
            {
                s_outInts[0] = NEW_INO_b_Joystick_IsButtonDown(button);
                s_outInts[1] = NEW_INO_b_Joystick_IsButtonJustDown(button);
                s_outInts[2] = NEW_INO_b_Joystick_IsButtonJustUp(button);
                s_outInts[3] = NEW_INO_b_IoButtonPressed(button);
            }
            else
            {
                s_outInts[0] = BASE_INO_b_Joystick_IsButtonDown(button);
                s_outInts[1] = BASE_INO_b_Joystick_IsButtonJustDown(button);
                s_outInts[2] = BASE_INO_b_Joystick_IsButtonJustUp(button);
                s_outInts[3] = BASE_INO_b_IoButtonPressed(button);
            }
            afterSide(side, 0);
        }
        compare(2000 + k);
    }

    /* INO_Joystick_Move: both sticks, the simulated path, the masked
       path and the back-end path, under all four direction flips. */
    for (k = 0; k < 64; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            int stick = k & 1;
            ioSetup(side, 500u + k, 0);
            INO_l_Joystick_Enable = (k % 9 != 8);
            INO_gi_CurrentPadId = (k % 7 == 6) ? -1 : 0;
            INO_l_JoystickDir = (k >> 1) & 3;
            INO_l_JoystickMask = ((k >> 3) & 1) ? -1 : 0;
            INO_gst_JoySimulateCurrent.controlsMask = ((k >> 4) & 1) ? (1 << (stick + 24)) : 0;
            INO_gst_JoySimulateCurrent.amountXLeft = 0.375f;
            INO_gst_JoySimulateCurrent.amountYLeft = -0.625f;
            INO_gst_JoySimulateCurrent.amountXRight = -0.875f;
            INO_gst_JoySimulateCurrent.amountYRight = 0.0625f;
            s_outVec.x = 123.0f;
            s_outVec.y = 456.0f;
            s_outVec.z = 789.0f;
            if (side)
                NEW_INO_Joystick_Move(&s_outVec, stick);
            else
                BASE_INO_Joystick_Move(&s_outVec, stick);
            afterSide(side, 0);
        }
        compare(3000 + k);
    }

    /* JoySimulate, with and without each stick. */
    for (k = 0; k < 8; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            Vec3 l, r;
            ioSetup(side, 600u + k, 0);
            l.x = 1.5f; l.y = -2.5f; l.z = 3.5f;
            r.x = -4.5f; r.y = 5.5f; r.z = -6.5f;
            if (side)
                NEW_INO_JoySimulate(k * 3, k * 5, (k & 1) ? &l : 0, (k & 2) ? &r : 0);
            else
                BASE_INO_JoySimulate(k * 3, k * 5, (k & 1) ? &l : 0, (k & 2) ? &r : 0);
            afterSide(side, 0);
        }
        compare(4000 + k);
    }

    /* Rumble: the enable gate, both setters and their clamps. */
    for (k = 0; k < 80; ++k)
    {
        static const int amounts[8] = { 0, 1, -1, 7, -7, 255, 256, -300 };
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 700u + k, 0);
            INO_gi_PifPafEnable = (k % 5 != 4);
            if (side)
            {
                s_outInts[0] = NEW_INO_i_PifPafEnable((k % 4) - 1);
                NEW_INO_PifSet(amounts[k % 8]);
                NEW_INO_PafSet(amounts[k % 8], amounts[(k + 3) % 8]);
                s_outInts[1] = NEW_INO_i_GetDiscError();
            }
            else
            {
                s_outInts[0] = BASE_INO_i_PifPafEnable((k % 4) - 1);
                BASE_INO_PifSet(amounts[k % 8]);
                BASE_INO_PafSet(amounts[k % 8], amounts[(k + 3) % 8]);
                s_outInts[1] = BASE_INO_i_GetDiscError();
            }
            afterSide(side, 0);
        }
        compare(5000 + k);
    }

    /* The control-mode pass-throughs and the module lifecycle. */
    for (k = 0; k < 12; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 800u + k, 0);
            if (side)
            {
                NEW_INO_SetControlMode(k - 2);
                NEW_INO_SetControlSubmode(k * 3 - 4);
                NEW_INO_SetMouseSensitivity((float)k * 0.125f);
                NEW_INO_Reinit();
            }
            else
            {
                BASE_INO_SetControlMode(k - 2);
                BASE_INO_SetControlSubmode(k * 3 - 4);
                BASE_INO_SetMouseSensitivity((float)k * 0.125f);
                BASE_INO_Reinit();
            }
            afterSide(side, 0);
        }
        compare(6000 + k);
    }
}

static void groupKeyboard(void)
{
    int k, side;

    s_group = "keyboard";

    /* The buffer swap and the function-key mapping, with and without
       the modifier that gates it. */
    for (k = 0; k < 48; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            int i;
            ioSetup(side, 900u + k, 0);
            pcINO_keyboardDataBackup[157] = (char)((k % 4 == 3) ? 0 : 0x80);
            for (i = 0; i < 10; ++i)
                pcINO_keyboardDataBackup[59 + i] = (char)(((k >> (i % 5)) & 1) ? 0x80 : 0);
            if (side)
                NEW_INO_Keyboard_Update();
            else
                BASE_INO_Keyboard_Update();
            afterSide(side, 0);
        }
        compare(k);
    }

    /* The four key predicates over every code. */
    for (k = 0; k < 256; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 1100u + k, 0);
            INO_Keys[k] = (uint8_t)((k & 1) ? 0x80 : 0x7F);
            INO_Keys_Prev[k] = (uint8_t)((k & 2) ? 0x80 : 0x01);
            if (side)
            {
                s_outInts[0] = NEW_INO_b_IsKeyPressed((uint8_t)k);
                s_outInts[1] = NEW_INO_b_IsKeyJustPressed((uint8_t)k);
                s_outInts[2] = NEW_INO_b_IsKeyReleased((uint8_t)k);
                s_outInts[3] = NEW_INO_b_IsKeyJustReleased((uint8_t)k);
            }
            else
            {
                s_outInts[0] = BASE_INO_b_IsKeyPressed((uint8_t)k);
                s_outInts[1] = BASE_INO_b_IsKeyJustPressed((uint8_t)k);
                s_outInts[2] = BASE_INO_b_IsKeyReleased((uint8_t)k);
                s_outInts[3] = BASE_INO_b_IsKeyJustReleased((uint8_t)k);
            }
            afterSide(side, 0);
        }
        compare(1000 + k);
    }

    /* Init and close of the whole module. */
    for (k = 0; k < 4; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 1400u + k, 0);
            IOMOCK_rets[0] = k;
            IOMOCK_retNum = 1;
            if (side)
            {
                NEW_INO_InitModule();
                NEW_INO_CloseModule();
            }
            else
            {
                BASE_INO_InitModule();
                BASE_INO_CloseModule();
            }
            afterSide(side, 0);
        }
        compare(2000 + k);
    }
}

static void groupLanguage(void)
{
    int k, side;

    s_group = "language";

    for (k = 0; k < 60; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 1500u + k, 0);
            SV(INO_ge_MasterType) = (k / 15) % 4;
            SVA(INO_gai_PresentLanguage)[(k % 12)] = (k % 2);
            if (side)
                s_outInts[0] = NEW_INO_b_LanguageIsPresent((k % 15) - 2);
            else
                s_outInts[0] = BASE_INO_b_LanguageIsPresent((k % 15) - 2);
            afterSide(side, 0);
        }
        compare(k);
    }
}

static void groupDemo(void)
{
    int k, side;

    s_group = "demo";

    for (k = 0; k < 40; ++k)
    {
        static const int fields[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 199, 200, 201, 202,
                                        203, 204, 665, 666, 667, -1, 1000, 100 };
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 1600u + k, 0);
            Demo_gb_Exit = k % 3;
            if (side)
            {
                s_outInts[0] = NEW_Demo_Geti(fields[k % 20]);
                s_outInts[1] = NEW_Demo_Seti(fields[k % 20]);
                s_outInts[2] = NEW_Demo_Geti(6);
            }
            else
            {
                s_outInts[0] = BASE_Demo_Geti(fields[k % 20]);
                s_outInts[1] = BASE_Demo_Seti(fields[k % 20]);
                s_outInts[2] = BASE_Demo_Geti(6);
            }
            afterSide(side, 0);
        }
        compare(k);
    }
}

static void groupSaveOrder(void)
{
    int k, side;

    s_group = "saveorder";

    /* Every pair of slots through the recency test, over states whose
       timestamps agree in each field in turn. */
    for (k = 0; k < 200; ++k)
    {
        int a = k % 10;
        int b = (k / 10) % 10;
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 1700u + k, 0);
            if (k >= 100)
            {
                /* make the two entries agree down to a chosen field,
                   so each arm of the chain is the one that decides */
                int depth = k % 7;
                SAV_gst_Data[b].superiorAtOrdering = SAV_gst_Data[a].superiorAtOrdering;
                if (depth > 0) SAV_gst_Data[b].year = SAV_gst_Data[a].year;
                if (depth > 1) SAV_gst_Data[b].month = SAV_gst_Data[a].month;
                if (depth > 2) SAV_gst_Data[b].mday = SAV_gst_Data[a].mday;
                if (depth > 3) SAV_gst_Data[b].hour = SAV_gst_Data[a].hour;
                if (depth > 4) SAV_gst_Data[b].min = SAV_gst_Data[a].min;
                if (depth > 5) SAV_gst_Data[b].sec = SAV_gst_Data[a].sec;
            }
            if (side)
            {
                s_outInts[0] = NEW_SAV_b_IsMoreRecent(a, b);
                s_outInts[1] = NEW_SAV_b_IsMoreRecent(b, a);
                NEW_SAV_OrderData();
            }
            else
            {
                s_outInts[0] = BASE_SAV_b_IsMoreRecent(a, b);
                s_outInts[1] = BASE_SAV_b_IsMoreRecent(b, a);
                BASE_SAV_OrderData();
            }
            afterSide(side, 0);
        }
        compare(k);
    }

    /* All ten slots carrying the same timestamp, which is the one case
       the index-order tie-break decides on its own. */
    for (k = 0; k < 4; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            int i;
            ioSetup(side, 1900u + k, 0);
            for (i = 0; i < 10; ++i)
            {
                SAV_gst_Data[i].superiorAtOrdering = (char)(k & 1);
                SAV_gst_Data[i].year = 2003;
                SAV_gst_Data[i].month = 5;
                SAV_gst_Data[i].mday = 9;
                SAV_gst_Data[i].hour = 13;
                SAV_gst_Data[i].min = 7;
                SAV_gst_Data[i].sec = (char)((k & 2) ? 0 : i);
            }
            if (side)
                NEW_SAV_OrderData();
            else
                BASE_SAV_OrderData();
            afterSide(side, 0);
        }
        compare(1000 + k);
    }
}

static void groupCardManager(void)
{
    int k, side;

    s_group = "cardmanager";

    /* Every state the machine can be in, driven with a scripted reply
       from the handler it dispatches to, with and without an open
       file, with and without an async operation in flight. */
    for (k = 0; k < 36 * 8; ++k)
    {
        int state = k % 36;
        int variant = k / 36;
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 2000u + k, 1);
            INO_ge_SavState = state;
            INO_gst_SavFileDesc.fp = (variant & 1) ? 0 : (FILE*)(intptr_t)0x1234;
            IOMOCK_async = (variant & 2) ? 1 : 0;
            SAV_gst_PhotosValidate.usedSize = (variant & 4)
                ? 0
                : ((state == 15) ? (int)(k * 7919) % (SAV_PHOTO_BUFFER_SIZE * 2) - 1000 : 4096);
            INO_gst_SavSlotDesc.id = (variant & 4) ? 11 : (k % 10);
            IOMOCK_rets[0] = 22;
            IOMOCK_rets[1] = 0;
            IOMOCK_rets[2] = 9;
            IOMOCK_retNum = 3;
            if (side)
                NEW_INO_v_MemoryCardManager(k % 5);
            else
                BASE_INO_v_MemoryCardManager(k % 5);
            afterSide(side, 0);
        }
        compare(k);
    }

    /* INO_SavSetCardStatus over every request value, from both a busy
       machine and an idle one. */
    for (k = 0; k < 48 * 4; ++k)
    {
        int status = (k % 48) - 2;
        int variant = k / 48;
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 3000u + k, 1);
            INO_ge_SavState = (variant == 0) ? 0 : (variant == 1) ? 27
                            : (variant == 2) ? 5 : status;
            INO_gst_SavSlotDesc.id = (k % 13);
            if (side)
                NEW_INO_SavSetCardStatus(k % 4, status);
            else
                BASE_INO_SavSetCardStatus(k % 4, status);
            afterSide(side, 0);
        }
        compare(1000 + k);
    }

    /* The status query, which decides between an error code, -1 and a
       pending state change. */
    for (k = 0; k < 64; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 4000u + k, 0);
            INO_ge_SavState = (k % 4 == 0) ? 0 : (k % 4 == 1) ? 27 : (k % 4 == 2) ? 3 : 20;
            SV(INO_ge_SavSetNextNextState) = ((k >> 2) & 1) ? -1 : (k % 30);
            pcINO_gi_LastError = ((k >> 3) & 1) ? 0 : 405;
            if (side)
                s_outInts[0] = NEW_INO_i_SavGetCardStatus(k % 4,
                                                          ((k >> 4) & 1) ? s_outInts + 1 : 0);
            else
                s_outInts[0] = BASE_INO_i_SavGetCardStatus(k % 4,
                                                           ((k >> 4) & 1) ? s_outInts + 1 : 0);
            afterSide(side, 0);
        }
        compare(2000 + k);
    }

    /* And the per-frame entry point, which runs all three. */
    for (k = 0; k < 36; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 5000u + k, 1);
            INO_ge_SavState = k;
            IOMOCK_rets[0] = 20;
            IOMOCK_rets[1] = 0;
            IOMOCK_retNum = 2;
            if (side)
                NEW_INO_Update();
            else
                BASE_INO_Update();
            afterSide(side, 0);
        }
        compare(3000 + k);
    }

    /* SAV_LoadData, which is a request in disguise. */
    for (k = 0; k < 6; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 5200u + k, 0);
            INO_ge_SavState = (k % 3 == 0) ? 0 : (k % 3 == 1) ? 27 : 4;
            if (side)
                NEW_SAV_LoadData(k & 1);
            else
                BASE_SAV_LoadData(k & 1);
            afterSide(side, 0);
        }
        compare(4000 + k);
    }
}

static void groupPhotos(void)
{
    int k, side;
    static char src[256];

    s_group = "photos";

    for (k = 0; k < 256; ++k)
        src[k] = (char)(k * 13 + 1);

    /* The two init entry points from an uninitialised state. */
    for (k = 0; k < 6; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 6000u + k, 0);
            if (k & 1)
            {
                SV(Photo_gi_FirstInit) = 1;
                SAV_pc_PhotoValidateBuffer = (char*)MEM_p_Alloc(SAV_PHOTO_BUFFER_SIZE);
                IOMOCK_logLen = 0;
            }
            if (side)
            {
                NEW_SAV_PhotoFirstInit();
                NEW_SAV_PhotoInit();
                NEW_SAV_PhotoInit();
            }
            else
            {
                BASE_SAV_PhotoFirstInit();
                BASE_SAV_PhotoInit();
                BASE_SAV_PhotoInit();
            }
            afterSide(side, 0);
        }
        compare(k);
    }

    /* SAV_PhotoSet over the index range, growing and shrinking. Index
       99 is driven only at its current size: the next entry's start
       offset is read past the array there, and a size change makes
       both copies walk off the buffer. */
    for (k = 0; k < 200; ++k)
    {
        int photo = (k % 103) - 2;
        int grow = k / 103;
        for (side = 0; side < 2; ++side)
        {
            size_t size;
            ioSetup(side, 7000u + k, 1);
            size = (size_t)SAV_gst_Photos.elems[photo >= 0 && photo < 100 ? photo : 0].photoSize;
            if (photo != 99)
                size = grow ? size + 17 : (size > 8 ? size - 8 : 0);
            if (side)
                NEW_SAV_PhotoSet(photo, src, size);
            else
                BASE_SAV_PhotoSet(photo, src, size);
            afterSide(side, 0);
        }
        compare(1000 + k);
    }

    /* The over-capacity refusal. */
    for (k = 0; k < 6; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 7300u + k, 1);
            SAV_gst_Photos.photoBufSize = (unsigned int)(SAV_gst_Photos.usedSize + k * 8);
            if (side)
                NEW_SAV_PhotoSet(10, src, (size_t)(SAV_gst_Photos.elems[10].photoSize + 32));
            else
                BASE_SAV_PhotoSet(10, src, (size_t)(SAV_gst_Photos.elems[10].photoSize + 32));
            afterSide(side, 0);
        }
        compare(2000 + k);
    }

    /* Both halves of SAV_PhotoDel: the compacting form below 1000 and
       the in-place form above it. */
    for (k = 0; k < 220; ++k)
    {
        int photo = (k % 110) - 2;
        if (k >= 110)
            photo = 1000 + ((k % 110) - 2);
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 8000u + k, 1);
            if (k % 7 == 3)
                SAV_gst_Photos.elems[(k % 100)].photoSize = 0;
            if (side)
                NEW_SAV_PhotoDel(photo);
            else
                BASE_SAV_PhotoDel(photo);
            afterSide(side, 0);
        }
        compare(3000 + k);
    }

    /* SAV_PhotoDecal, whose count must stay inside the table. */
    for (k = 0; k < 120; ++k)
    {
        int first = k % 12;
        int count = (k / 12) % 10;
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 9000u + k, 1);
            if (k % 5 == 2)
            {
                SAV_gst_Photos.elems[first].photoSize = 0;
                SAV_gst_Photos.elems[first + 2].photoSize = 0;
            }
            if (side)
                NEW_SAV_PhotoDecal(1000 + first, count);
            else
                BASE_SAV_PhotoDecal(1000 + first, count);
            afterSide(side, 0);
        }
        compare(4000 + k);
    }
    /* and the two refusals: a first photo below the 1000 offset, and
       a first or a count outside the hundred entries */
    for (k = 0; k < 10; ++k)
    {
        static const int first[10] = { 0, 50, 100, 999, 1105, 1005, 1005, 1099, 1100, 1000 };
        static const int count[10] = { 0, 1, 2, 3, 3, 150, -2, 1, 1, 100 };
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 9200u + k, 1);
            if (side)
                NEW_SAV_PhotoDecal(first[k], count[k]);
            else
                BASE_SAV_PhotoDecal(first[k], count[k]);
            afterSide(side, 0);
        }
        compare(5000 + k);
    }

    /* SAV_PhotoGet across the bounds. */
    for (k = 0; k < 104; ++k)
    {
        int photo = k - 2;
        for (side = 0; side < 2; ++side)
        {
            char* got;
            int32_t size = -7;
            ioSetup(side, 9400u + k, 1);
            got = side ? NEW_SAV_PhotoGet(photo, &size) : BASE_SAV_PhotoGet(photo, &size);
            s_outInts[0] = size;
            afterSide(side, IOMOCK_Offset(got));
        }
        compare(6000 + k);
    }

    /* Validate, unvalidate, reinit and the two closes. */
    for (k = 0; k < 32; ++k)
    {
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 9600u + k, (k & 1));
            if (k & 8)
                SAV_gst_Photos.usedSize = 0;
            if (side)
            {
                NEW_SAV_PhotoValidate();
                NEW_SAV_PhotoUnvalidate();
                NEW_SAV_PhotoReinitValidate();
                s_outInts[0] = (int)IOMOCK_Offset(NEW_SAV_PhotoReinit());
                if (k & 2)
                    NEW_SAV_PhotoClose();
                if (k & 4)
                    NEW_SAV_PhotoLastClose();
            }
            else
            {
                BASE_SAV_PhotoValidate();
                BASE_SAV_PhotoUnvalidate();
                BASE_SAV_PhotoReinitValidate();
                s_outInts[0] = (int)IOMOCK_Offset(BASE_SAV_PhotoReinit());
                if (k & 2)
                    BASE_SAV_PhotoClose();
                if (k & 4)
                    BASE_SAV_PhotoLastClose();
            }
            afterSide(side, 0);
        }
        compare(7000 + k);
    }
}

/* Two things the sweeps above cannot reach, driven on purpose. They
   take a seed base above everything else in the file so a case
   inserted earlier cannot shift them off the branch they exist for. */
static void groupTargeted(void)
{
    int k, side;

    s_group = "targeted";

    /* The demo's exit chord. The sweep fills the pad with random
       bytes, where both of its two buttons are down in all but one
       draw in 128, so "both" and "either" agree almost everywhere.
       All four combinations on each of the four pads here. */
    for (k = 0; k < 64; ++k)
    {
        int pad = k % 4;
        int combo = (k / 4) % 4;
        for (side = 0; side < 2; ++side)
        {
            int i;
            ioSetup(side, 30000u + k, 0);
            INO_l_Joystick_Enable = 1;
            INO_gi_CurrentPadId = 0;
            INO_l_JoystickMask = -1;
            INO_gst_JoySimulateAsked.controlsMask = 0;
            Demo_gb_Exit = (k / 16) & 1;
            IOMOCK_mouseInUse = 0;
            for (i = 0; i < 4; ++i)
            {
                INO_gst_CurrPad[i].padData[8] = 0;
                INO_gst_CurrPad[i].padData[9] = 0;
            }
            INO_gst_CurrPad[pad].padData[8] = (uint8_t)((combo & 1) ? 0x80 : 0);
            INO_gst_CurrPad[pad].padData[9] = (uint8_t)((combo & 2) ? 0x80 : 0);
            if (side)
                NEW_INO_Joystick_Update();
            else
                BASE_INO_Joystick_Update();
            afterSide(side, 0);
        }
        compare(k);
    }

    /* The photo block's clamp in card state 15. The sweep's sizes all
       land inside the buffer, so neither arm of the clamp is entered;
       these are the two ends and the values either side of them. */
    for (k = 0; k < 16; ++k)
    {
        static const int sizes[8] = { -1000000, -5, -1, 0, 1,
                                      SAV_PHOTO_BUFFER_SIZE,
                                      SAV_PHOTO_BUFFER_SIZE + 1,
                                      SAV_PHOTO_BUFFER_SIZE * 2 };
        for (side = 0; side < 2; ++side)
        {
            ioSetup(side, 31000u + k, 1);
            INO_ge_SavState = 15;
            SAV_gst_PhotosValidate.usedSize = sizes[k % 8];
            pc_Buffer_304 = (k & 8) ? SAV_pc_PhotoValidateBuffer : 0;
            IOMOCK_rets[0] = 28;
            IOMOCK_retNum = 1;
            if (side)
                NEW_INO_v_MemoryCardManager(0);
            else
                BASE_INO_v_MemoryCardManager(0);
            afterSide(side, 0);
        }
        compare(1000 + k);
    }
}

int main(void)
{
    s_baseArena = (unsigned char*)malloc(IOMOCK_ARENA_SIZE);
    if (!s_baseArena)
        return 2;

    printf("=== inputoutput differential test ===\n");

    groupJoystick();
    groupKeyboard();
    groupLanguage();
    groupDemo();
    groupSaveOrder();
    groupCardManager();
    groupPhotos();
    groupTargeted();

    printf("\n%d cases, %d differing\n", s_cases, s_fails);
    if (s_fails)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
