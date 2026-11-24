/* Everything memory.c refers to and the allocator does not need.
 *
 * It is a short list on purpose: the allocator carves up one block it
 * takes from the C library and asks nothing else of the engine. The
 * semaphore is real enough to be correct for one thread, which is all
 * the driver uses; the other five are reached only from paths the
 * driver does not take, and say so if they ever are.
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_mutex.h>

static int mock_unreachable(const char* what)
{
    printf("  FAIL a mock the driver should never reach was called: %s\n", what);
    exit(1);
}

/* A counting semaphore with one holder and one thread: the count is
   all that is observable, and the driver would deadlock rather than
   race if the allocator ever took it twice. */
struct SDL_Semaphore
{
    int count;
};

SDL_Semaphore* SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_Semaphore* s = (SDL_Semaphore*)calloc(1, sizeof(*s));
    if (s)
        s->count = (int)initial_value;
    return s;
}

void SDL_DestroySemaphore(SDL_Semaphore* sem)
{
    free(sem);
}

void SDL_WaitSemaphore(SDL_Semaphore* sem)
{
    if (!sem)
        return;
    if (sem->count <= 0)
    {
        printf("  FAIL the allocator took its own lock twice\n");
        exit(1);
    }
    --sem->count;
}

void SDL_SignalSemaphore(SDL_Semaphore* sem)
{
    if (sem)
        ++sem->count;
}

/* Reached only from the defragmenting, texture-preloading and
   out-of-memory paths, with the engine's own signatures so that a
   mismatch cannot hide here. */
void SOFT_ZList_Clear() { mock_unreachable("SOFT_ZList_Clear"); }
void SOFT_ZList_Init() { mock_unreachable("SOFT_ZList_Init"); }
void PreLoadTexReset() { mock_unreachable("PreLoadTexReset"); }
void PLT_Message(const char* title, const char* text, int error)
{
    (void)title; (void)text; (void)error;
}
void AI_EvalFunc_DbgTraceInt_C() { }
