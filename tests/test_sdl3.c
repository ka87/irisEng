/* The vendored SDL3, and whether it is usable.
 *
 * Nothing in the engine uses SDL yet. This exists so that the thing
 * the next two pieces of work stand on is checked on its own: that the
 * vendored source builds, that it comes up headless, and that the few
 * calls the sound and input backends are going to be written against
 * do what they are expected to do.
 *
 * It runs against SDL's **dummy** audio driver and dummy video driver,
 * which is the whole reason the container build turns every real
 * backend off. There is no mock here and no stand-in: this is the real
 * SDL, with real streams and real events, writing to nothing. A mock
 * of an API nobody has used yet would only encode what I assumed it
 * does, which is the opposite of what is wanted from this file.
 */
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

static int checks;
static int fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okStr(const char* got, const char* want, const char* what)
{
    checks++;
    if (!got || strcmp(got, want) != 0)
    {
        printf("  FAIL %s: got %s want %s\n", what, got ? got : "(null)", want);
        fails++;
    }
}

/* What the sound backend will be built out of: a stream per playing
   sound, with a gain, a frequency ratio for the pitch, and bytes put
   in at one end. */
static void audioStream(void)
{
    SDL_AudioSpec spec;
    SDL_AudioStream* stream;
    short frames[512];
    int   i;

    printf("-- an audio stream, as the sound backend will use one\n");

    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = 44100;

    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                       &spec, NULL, NULL);
    ok(stream != NULL, "a playback stream opens on the dummy device");
    if (!stream)
        return;

    for (i = 0; i < 512; i++)
        frames[i] = (short)(i * 64);

    ok(SDL_PutAudioStreamData(stream, frames, (int)sizeof(frames)),
       "and takes a buffer of frames");

    /* The volume and the pitch, which the engine sets per sound and
       which the two of these are what will carry them. */
    ok(SDL_SetAudioStreamGain(stream, 0.5f), "a gain can be set on it");
    ok(SDL_GetAudioStreamGain(stream) == 0.5f, "and reads back");
    ok(SDL_SetAudioStreamFrequencyRatio(stream, 1.5f),
       "a frequency ratio can be set on it");
    ok(SDL_GetAudioStreamFrequencyRatio(stream) == 1.5f, "and reads back");

    /* How far through a sound is, which the engine asks for. */
    ok(SDL_GetAudioStreamQueued(stream) >= 0, "what is still queued is answered");

    ok(SDL_PauseAudioStreamDevice(stream), "the stream pauses");
    ok(SDL_ResumeAudioStreamDevice(stream), "and resumes");
    ok(SDL_ClearAudioStream(stream), "and can be emptied");

    SDL_DestroyAudioStream(stream);
}

/* What the input and window work will be built out of: a window, and
   the event queue that comes with owning one. */
static void windowAndEvents(void)
{
    SDL_Window* window;
    SDL_Event   ev;
    const bool* keys;
    int         numKeys = 0;
    int         w = 0;
    int         h = 0;

    printf("-- a window and its events, as the platform layer will\n");

    window = SDL_CreateWindow("iriseng", 640, 480, 0);
    ok(window != NULL, "a window is created on the dummy driver");
    if (!window)
        return;

    ok(SDL_GetWindowSize(window, &w, &h), "its size is answered");
    ok(w == 640 && h == 480, "and is the size asked for");

    /* The keyboard as a block of state, which is how the engine reads
       it rather than by event: it asks what is down this frame. */
    keys = SDL_GetKeyboardState(&numKeys);
    ok(keys != NULL, "the keyboard state is a block to read");
    ok(numKeys > SDL_SCANCODE_ESCAPE, "long enough for the keys the game uses");

    /* Pumping is what makes a window's messages arrive, and it is the
       call that replaces the engine's own message loop. */
    SDL_PumpEvents();
    while (SDL_PollEvent(&ev))
        ;
    ok(1, "events pump and drain without a window system");

    SDL_DestroyWindow(window);
}

int main(void)
{
    printf("=== vendored SDL3 ===\n");

    /* The drivers are chosen here rather than left to SDL: the point
       of the headless build is that these two are all there is, and
       asking for them by name fails loudly if that ever changes. */
    SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy");
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");

    ok(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMEPAD),
       "SDL comes up with audio, video and gamepads");
    if (!SDL_WasInit(SDL_INIT_AUDIO))
    {
        printf("  FAIL SDL did not initialise: %s\n", SDL_GetError());
        return 1;
    }

    okStr(SDL_GetCurrentAudioDriver(), "dummy", "the audio driver");
    okStr(SDL_GetCurrentVideoDriver(), "dummy", "the video driver");

    /* The version the source is pinned at, from the headers and from
       the library -- the two disagree when a build picked up an SDL
       from somewhere else, which is worth knowing early. */
    ok(SDL_VERSION == SDL_GetVersion(),
       "the library is the vendored one, not another on the system");
    printf("  SDL %d.%d.%d\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION,
           SDL_MICRO_VERSION);

    audioStream();
    windowAndEvents();

    SDL_Quit();

    printf("  %d checks, %d failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
