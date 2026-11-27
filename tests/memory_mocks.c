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
#include <stdarg.h>
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

/* The renderer's depth list, which the temporary allocator starts
   again whenever it rewinds its buffer -- so these two are reached,
   and counted rather than refused, and the driver checks the count.
   Everything that comes out of that buffer is the renderer's working
   storage, so rewinding it without telling the renderer would leave
   the list pointing into memory about to be handed out again. */
int MOCK_ZListInits;
int MOCK_ZListClears;

void SOFT_ZList_Clear() { ++MOCK_ZListClears; }
void SOFT_ZList_Init() { ++MOCK_ZListInits; }

/* Reached only from the defragmenting, texture-preloading and
   out-of-memory paths, with the engine's own signatures so that a
   mismatch cannot hide here. */
void PreLoadTexReset() { mock_unreachable("PreLoadTexReset"); }
void PLT_Message(const char* title, const char* text, int error)
{
    (void)title; (void)text; (void)error;
}
void AI_EvalFunc_DbgTraceInt_C() { }

/* The allocator says one thing: that it was handed something
   which is not a live block. No case here does that, so a line
   from it is a finding and goes where the runner shows it. */
int mock_reports;

void DBG_OutputDebugString(char* format, ...)
{
    va_list args;

    ++mock_reports;

    printf("  the allocator reported: ");
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
