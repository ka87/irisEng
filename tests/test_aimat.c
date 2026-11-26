/* The AI material and colour builtins, across the four-byte AI stack.
 *
 * The AI language passes everything through a stack of four-byte slots,
 * so a builtin that hands a script a reference to something the engine
 * owns has to put a handle there and resolve one coming back. Several
 * of these builtins do that -- a procedural texture, an animated
 * texture, a game object -- and they were casting the address straight
 * into an int instead, which truncates.
 *
 * Nothing in ai/ had a test of any kind before this: there is no
 * frozen copy of any of those files and no runner built one, which is
 * the fourth group in this work to turn out that way.
 *
 * The defect it was written for is not the handles, though. The colour
 * builtins take a colour off the stack and write it through the
 * address MAT_GetColorPtr hands back, and MATSetAmbient declared that
 * address as an int** where its own getter three functions below has
 * always had int*. Writing through an int** writes eight bytes, and
 * the ambient colour is a four-byte field with the diffuse colour
 * directly after it -- so on a wide build setting the ambient colour
 * destroyed the diffuse one. check_the_ambient_setter below is that,
 * and it fails against the int** spelling.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../handle.h"
#include "../textures.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geomaterial.h"
#include "../gameobject.h"
#include "../ai.h"
#include "aimat_mocks.h"

/* Declared in ai/ai_materialandcolorfunctions.h, but the builtins that
   carry their whole body in the wrapper are not. */
tAINode* AI_EvalFunc_MATSetAmbient(tAINode* node);
tAINode* AI_EvalFunc_MATGetAmbient(tAINode* node);
tAINode* AI_EvalFunc_MATSetDiffuse(tAINode* node);
tAINode* AI_EvalFunc_MATGetDiffuse(tAINode* node);
tAINode* AI_EvalFunc_MATSetConstant(tAINode* node);
tAINode* AI_EvalFunc_MATGetConstant(tAINode* node);
tAINode* AI_EvalFunc_MATGetPro(tAINode* node);
tAINode* AI_EvalFunc_MATProSeti(tAINode* node);
tGEOMAT* MAT_ValidateSUBMAT_C(tGameObjectMain* gao, unsigned int subMatID);
void MAT_GetColorPtr(tGEOMAT* material, void** colorPtr, int multiMatId,
                     int which);

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static void failx(const char* what, unsigned long got, unsigned long want)
{
    printf("  FAIL %s: got 0x%lx, want 0x%lx\n", what, got, want);
    ++failures;
}

/* ------------------------------------------------------- the material */

static tGroIntf intf3;
static tGroIntf intf5;
static tProceduralIntf proIntf;
static tMultiTextureElemStruct level;
static char mpegRoom[512];
static tGEOMAT mat;
static tGameObjectVisualObject viso;
static tGameObjectBasic basic;
static tGameObjectMain gao;
static tAINode nodes[4];

/* A type 3 material on a game object, which is the shape
   MAT_ValidateSUBMAT_C walks without needing anything resolved. */
static void world_init(void)
{
    memset(&intf3, 0, sizeof(intf3));
    intf3.id = 3;
    memset(&intf5, 0, sizeof(intf5));
    intf5.id = 5;
    memset(&proIntf, 0, sizeof(proIntf));
    proIntf.id = 3;         /* the kind whose field 11 takes another */
    memset(&level, 0, sizeof(level));
    level.texElemIdx = 7;

    memset(&mat, 0, sizeof(mat));
    mat.intf = &intf3;

    memset(&viso, 0, sizeof(viso));
    viso.GROMAT = (struct sGroGeneric*)&mat;

    memset(&basic, 0, sizeof(basic));
    basic.actorObjectOrVisualObject = (void*)&viso;

    memset(&gao, 0, sizeof(gao));
    gao.flags = 0x4000;
    gao.basic = &basic;

    memset(nodes, 0, sizeof(nodes));
}

/* The three colours, as the engine stores them. */
/* Every one of these builtins reads the object it works on out of
   AI_gpst_CurrentUltra and then replaces it with
   AI_gpst_CurrentGameObject, so both have to be the driver's object
   before each call. */
static void arm(void)
{
    AI_gpst_CurrentUltra = &gao;
    AI_gpst_CurrentGameObject = &gao;
    AI_gpst_CurrentInstance = NULL;
    AI_gpst_CurrentInstanceUltra = NULL;
}

static void set_colours(unsigned int ambient, unsigned int diffuse,
                        unsigned int constant)
{
    mat.multiMaterialNumIfType4ThenAmbiantColor = (int)ambient;
    mat.multiMaterialPtrIfType4ThenDiffuseColor =
        (tGEOMAT**)(uintptr_t)diffuse;
    mat.solidVertexColor = (int)constant;
}

static unsigned int ambient_of(void)
{
    return (unsigned int)mat.multiMaterialNumIfType4ThenAmbiantColor;
}

static unsigned int diffuse_of(void)
{
    return (unsigned int)(uintptr_t)mat.multiMaterialPtrIfType4ThenDiffuseColor;
}

static unsigned int constant_of(void)
{
    return (unsigned int)mat.solidVertexColor;
}

/* The check on every colour check here: the three really are distinct
   fields in the order this file assumes, so a write that runs over one
   of them lands in the next. */
static void check_the_three_colours_are_where_we_think(void)
{
    printf("-- where the three colours live\n");

    if ((char*)&mat.multiMaterialPtrIfType4ThenDiffuseColor
        != (char*)&mat.multiMaterialNumIfType4ThenAmbiantColor + 4)
        fail("the diffuse colour does not sit directly after the ambient "
             "one, so an over-wide write to the ambient would not show "
             "up in the diffuse and the check below proves less than it "
             "says");
    if (sizeof(mat.multiMaterialNumIfType4ThenAmbiantColor) != 4)
        fail("the ambient colour is not a four-byte field");

    /* And the address the builtins write through is that field. */
    {
        void* slot = NULL;

        MAT_GetColorPtr(&mat, &slot, 0, 2);
        if (slot != (void*)&mat.multiMaterialNumIfType4ThenAmbiantColor)
            fail("the ambient colour's address is not the ambient field");
        MAT_GetColorPtr(&mat, &slot, 0, 0);
        if (slot != (void*)&mat.multiMaterialPtrIfType4ThenDiffuseColor)
            fail("the diffuse colour's address is not the diffuse field");
        MAT_GetColorPtr(&mat, &slot, 0, 1);
        if (slot != (void*)&mat.solidVertexColor)
            fail("the constant colour's address is not that field");
    }
}

/* The defect. Setting the ambient colour must write four bytes and
   leave the diffuse colour alone. */
static void check_the_ambient_setter(void)
{
    static const unsigned int colours[] = {
        0x11223344u, 0x00000000u, 0xFFFFFFFFu, 0x80000000u, 0x7F7F7F7Fu
    };
    int i;

    printf("-- setting the ambient colour\n");

    for (i = 0; i < (int)(sizeof(colours) / sizeof(colours[0])); i++)
    {
        AIMATMOCK_Reset();
        arm();
        set_colours(0xAAAAAAAAu, 0xBBBBBBBBu, 0xCCCCCCCCu);

        AIMATMOCK_Push((int)colours[i]);        /* the colour   */
        AIMATMOCK_Push(0);                      /* the subMatID */
        AI_EvalFunc_MATSetAmbient(nodes);

        if (ambient_of() != colours[i])
            failx("the ambient colour", ambient_of(), colours[i]);
        if (diffuse_of() != 0xBBBBBBBBu)
            failx("setting the ambient colour wrote over the diffuse one",
                  diffuse_of(), 0xBBBBBBBBu);
        if (constant_of() != 0xCCCCCCCCu)
            failx("setting the ambient colour wrote over the constant one",
                  constant_of(), 0xCCCCCCCCu);
        if (AIMATMOCK_underflows)
            fail("the builtin took more off the stack than it was given");
    }
}

/* And the other two setters, which were already right, so this is the
   pair that says the check above is not simply failing everything. */
static void check_the_other_setters(void)
{
    printf("-- setting the diffuse and constant colours\n");

    AIMATMOCK_Reset();
    arm();
    set_colours(0xAAAAAAAAu, 0xBBBBBBBBu, 0xCCCCCCCCu);
    AIMATMOCK_Push((int)0x12345678u);
    AIMATMOCK_Push(0);
    AI_EvalFunc_MATSetDiffuse(nodes);
    if (diffuse_of() != 0x12345678u)
        failx("the diffuse colour", diffuse_of(), 0x12345678u);
    if (ambient_of() != 0xAAAAAAAAu)
        fail("setting the diffuse colour wrote over the ambient one");
    if (constant_of() != 0xCCCCCCCCu)
        fail("setting the diffuse colour wrote over the constant one");

    AIMATMOCK_Reset();
    arm();
    set_colours(0xAAAAAAAAu, 0xBBBBBBBBu, 0xCCCCCCCCu);
    AIMATMOCK_Push((int)0x87654321u);
    AIMATMOCK_Push(0);
    AI_EvalFunc_MATSetConstant(nodes);
    if (constant_of() != 0x87654321u)
        failx("the constant colour", constant_of(), 0x87654321u);
    if (ambient_of() != 0xAAAAAAAAu)
        fail("setting the constant colour wrote over the ambient one");
    if (diffuse_of() != 0xBBBBBBBBu)
        fail("setting the constant colour wrote over the diffuse one");
}

/* Each getter answers with the colour its own setter wrote, which is
   what says the two halves agree about where the colour is and how
   wide it is. */
static void check_the_getters(void)
{
    printf("-- reading the three colours back\n");

    AIMATMOCK_Reset();
    arm();
    set_colours(0x01020304u, 0x05060708u, 0x090A0B0Cu);

    AIMATMOCK_Push(0);
    AI_EvalFunc_MATGetAmbient(nodes);
    if (AIMATMOCK_stackNum != 1
        || (unsigned int)AIMATMOCK_stack[0] != 0x01020304u)
        failx("the ambient colour came back",
              AIMATMOCK_stackNum ? (unsigned int)AIMATMOCK_stack[0] : 0,
              0x01020304u);

    AIMATMOCK_Reset();
    arm();
    AIMATMOCK_Push(0);
    AI_EvalFunc_MATGetDiffuse(nodes);
    if (AIMATMOCK_stackNum != 1
        || (unsigned int)AIMATMOCK_stack[0] != 0x05060708u)
        failx("the diffuse colour came back",
              AIMATMOCK_stackNum ? (unsigned int)AIMATMOCK_stack[0] : 0,
              0x05060708u);

    AIMATMOCK_Reset();
    arm();
    AIMATMOCK_Push(0);
    AI_EvalFunc_MATGetConstant(nodes);
    if (AIMATMOCK_stackNum != 1
        || (unsigned int)AIMATMOCK_stack[0] != 0x090A0B0Cu)
        failx("the constant colour came back",
              AIMATMOCK_stackNum ? (unsigned int)AIMATMOCK_stack[0] : 0,
              0x090A0B0Cu);
}

/* A reference across the stack. The builtin that hands a script a
   procedural texture has to put something there that resolves back to
   it, and the one that takes it back has to resolve it. */
static void check_a_reference_across_the_stack(void)
{
    struct sTexProceduralStruct* pro;
    int slotValue;

    printf("-- a reference handed to a script and taken back\n");

    /* A block the layer can issue a handle for, out on the heap, so a
       truncated address would not survive the trip. */
    pro = (struct sTexProceduralStruct*)malloc(256);
    memset(pro, 0, 256);
    pro->proceduralIntf = &proIntf;
    pro->proceduralTexPtr = mpegRoom;       /* what field 11 works on */
    AIMATMOCK_procedural = pro;
    AIMATMOCK_anim = NULL;

    /* MAT_GetMtxLvlPtr only answers for a multi-texture material, and
       it reads the chain head out of the slot the material keeps it
       in -- so this is also the one check that reaches that slot from
       the AI side. */
    mat.intf = &intf5;
    mat.idxOrPtr = (intptr_t)&level;

    if (!((uintptr_t)pro >> 32))
        printf("    NOTE the heap is below thirty-two bits here, so a "
               "truncated reference would survive and the checks below "
               "prove less\n");

    AIMATMOCK_Reset();
    arm();
    AIMATMOCK_Push(0);              /* multiMatElemId */
    AIMATMOCK_Push(0);              /* subMatID       */
    AI_EvalFunc_MATGetPro(nodes);

    if (AIMATMOCK_stackNum != 1)
    {
        fail("the builtin did not answer with one slot");
        free(pro);
        return;
    }
    slotValue = AIMATMOCK_stack[0];

    if (HDL_p_Pointer((uint32_t)slotValue) != (void*)pro)
        fail("what the builtin put on the stack does not resolve to the "
             "procedural texture it was given");
    if (slotValue == (int)(uintptr_t)pro && ((uintptr_t)pro >> 32))
        fail("the builtin put a truncated address on the stack");

    /* And back the other way: the same slot value, handed to a builtin
       that takes a reference, has to reach the engine whole. */
    AIMATMOCK_Reset();
    arm();
    AIMATMOCK_Push(slotValue);      /* param1: another procedural */
    AIMATMOCK_Push(11);             /* field 11 updates with it   */
    AIMATMOCK_Push(slotValue);      /* the one to update          */
    AI_EvalFunc_MATProSeti(nodes);

    if (AIMATMOCK_updateWithNum != 1)
        fail("the update builtin did not reach the engine once");
    else if (AIMATMOCK_updateWith != (void*)pro)
        fail("the reference the script passed back did not arrive whole");

    free(pro);
    AIMATMOCK_procedural = NULL;
}

int main(void)
{
    printf("=== the AI material and colour builtins ===\n");

    world_init();

    check_the_three_colours_are_where_we_think();
    check_the_ambient_setter();
    check_the_other_setters();
    check_the_getters();
    check_a_reference_across_the_stack();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
