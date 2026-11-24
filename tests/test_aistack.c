/* An object on and off the evaluator's stack.
 *
 * A reference lives in four bytes everywhere the file formats reach:
 * the variable block it is loaded into, the scratch a keyword copies
 * it to, and the stack slot it is pushed onto. A C variable is the
 * first thing wide enough to hold an address, so there are exactly two
 * places where the four bytes and an address meet --
 * AI_PutObjectToStack on the way in and AI_GetObjectFromStack on the
 * way out -- and those two are what this drives.
 *
 * The round trip is the check that could not pass before: pushing a
 * thing and taking it back gave its address with the top half gone.
 * The rest is the behaviour around it that had to survive -- nought
 * is no object, the two values that name a main actor still name one,
 * and a type that is not a reference is pushed and popped as the bytes
 * it is.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../ai.h"
#include "../gameobject.h"
#include "../handle.h"

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* The two type ids this exercises, as the evaluator numbers them
   rather than as a file does: the table is indexed by the remapped id,
   and AI_gaw_EnumLink is what remaps it. */
static int16_t type_of(int funcid)
{
    int i;
    for (i = 0; i < 23; ++i)
        if (AI_gast_Types[i].funcid == funcid)
            return (int16_t)i;
    printf("  FAIL the type table has no entry for funcid %d\n", funcid);
    ++failures;
    return 0;
}

/* Objects at addresses this build really uses. Never read through --
   only pushed, popped and compared. */
static tGameObjectMain thing1;
static tGameObjectMain thing2;
static tGameObjectMain actor1;
static tGameObjectMain actor2;

static void check_round_trip(int16_t gaoType)
{
    void* in[3];
    void* out[3];
    unsigned i;

    in[0] = &thing1;
    in[1] = &thing2;
    in[2] = &actor1;

    /* Pushed in order, so they come back in the other one. These hold
       the objects' addresses, so they go on through the entry point
       that is for that. */
    for (i = 0; i < 3; ++i)
        AI_PutRefToStack(in[i], gaoType);
    for (i = 0; i < 3; ++i)
        out[2 - i] = AI_GetObjectFromStack();

    for (i = 0; i < 3; ++i)
        if (out[i] != in[i])
        {
            printf("  FAIL object %u went on as %p and came back as %p\n",
                   i, in[i], out[i]);
            ++failures;
        }
}

/* The other kind of push: a value already in the form a slot holds it.
 *
 * A keyword that copies a reference from one place to another has it as
 * four bytes already, and pushing it must not convert it again -- a
 * handle encoded a second time is a handle for wherever that number
 * happens to point, which is nothing. The two entry points exist for
 * this distinction and nothing else, so it is the thing to check.
 */
static void check_stored_form_is_not_encoded_again(int16_t gaoType)
{
    uint32_t stored;
    void* back;

    stored = HDL_ul_Handle(&thing2);
    AI_PutObjectToStack(&stored, gaoType);
    back = AI_GetObjectFromStack();
    if (back != &thing2)
    {
        printf("  FAIL a reference already in a slot's form went on as "
               "%p and came back as %p\n", (void*)&thing2, back);
        ++failures;
    }

    /* And the two forms have to agree about the same object. */
    AI_PutRefToStack(&thing2, gaoType);
    if (AI_GetObjectFromStack() != &thing2)
        fail("the two entry points disagree about the same object");
}

static void check_no_object(int16_t gaoType)
{
    void* nothing = NULL;
    void* back;

    AI_PutRefToStack(nothing, gaoType);
    back = AI_GetObjectFromStack();
    if (back != NULL)
    {
        printf("  FAIL no object came back as %p\n", back);
        ++failures;
    }
}

/* The two values that name an actor rather than referring to one.
 * They arrive in a slot, not from a C variable, so they are put there
 * the way a loaded variable block would have them -- and the accessor
 * is what has to turn them into the actor they name. */
static void check_main_actors(int16_t gaoType)
{
    uint32_t slot;
    uint32_t scratch;

    AI_MainActorArr[0] = &actor1;
    AI_MainActorArr[1] = &actor2;
    AI_MainActor = AI_MainActorArr;

    slot = AI_MAIN_ACTOR_FIRST;
    scratch = 0xDEADu;
    AI_gast_Types[gaoType].funcptr(&slot, &scratch);
    if (HDL_p_Pointer(scratch) != &actor1)
        fail("the first main actor was not named by the value one");

    slot = AI_MAIN_ACTOR_SECOND;
    AI_gast_Types[gaoType].funcptr(&slot, &scratch);
    if (HDL_p_Pointer(scratch) != &actor2)
        fail("the second main actor was not named by the value two");

    /* Whatever the accessor puts in the slot has to be the same kind of
       thing in both cases, or the next copy of it means two different
       things depending on which value it started as. */
    slot = AI_MAIN_ACTOR_FIRST;
    AI_gast_Types[gaoType].funcptr(&slot, &scratch);
    if (scratch == AI_MAIN_ACTOR_FIRST)
        fail("the value one was passed through rather than resolved");

    /* An ordinary reference is copied as it is. */
    slot = HDL_ul_Handle(&thing1);
    AI_gast_Types[gaoType].funcptr(&slot, &scratch);
    if (scratch != slot)
        fail("an ordinary reference was not copied unchanged");
    if (HDL_p_Pointer(scratch) != &thing1)
        fail("an ordinary reference stopped naming its object");
}

/* The same rule once what holds the value is a C variable.
 *
 * A reference is normalised where it is read out of a slot, so this
 * should never see one of the two naming values -- but the keywords
 * that compare two objects apply it anyway rather than resting on that
 * argument, so it has to be right. */
static void check_actor_or_object(void)
{
    AI_MainActorArr[0] = &actor1;
    AI_MainActorArr[1] = &actor2;
    AI_MainActor = AI_MainActorArr;

    if (AI_p_ActorOrObject((void*)(uintptr_t)AI_MAIN_ACTOR_FIRST) != &actor1)
        fail("the value one did not name the first main actor");
    if (AI_p_ActorOrObject((void*)(uintptr_t)AI_MAIN_ACTOR_SECOND) != &actor2)
        fail("the value two did not name the second main actor");
    if (AI_p_ActorOrObject(&thing1) != &thing1)
        fail("an ordinary object was not left alone");
    if (AI_p_ActorOrObject(NULL) != NULL)
        fail("no object was not left as no object");
}

/* A type that is not a reference must be untouched by any of this. */
static void check_vector_untouched(void)
{
    int16_t vecType = type_of(0x7C);
    Vec3 in, out;

    in.x = 1.5f; in.y = -2.25f; in.z = 1024.0f;
    AI_PutObjectToStack(&in, vecType);
    /* Popped the way a vector is: through the type's own accessor,
       since AI_GetObjectFromStack is for references. */
    memset(&out, 0, sizeof(out));
    {
        tVarStackElem var;
        int converted[13];
        AI_GetVarFromStack(converted, &var);
        memcpy(&out, converted, sizeof(out));
    }
    if (out.x != in.x || out.y != in.y || out.z != in.z)
        printf("  FAIL a vector came back as (%f, %f, %f)\n",
               (double)out.x, (double)out.y, (double)out.z);
    if (out.x != in.x || out.y != in.y || out.z != in.z)
        ++failures;
}

int main(void)
{
    int16_t gaoType;

    printf("=== an object on and off the evaluator's stack ===\n");

    gaoType = type_of(0x28);
    if (failures)
        return 1;

    check_round_trip(gaoType);
    check_stored_form_is_not_encoded_again(gaoType);
    check_no_object(gaoType);
    check_main_actors(gaoType);
    check_actor_or_object();
    check_vector_untouched();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
