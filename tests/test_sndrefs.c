/* A voice and a sound buffer reached through the translation layer.
 *
 * sndbackend.c keeps three key/value lists whose keys are addresses:
 * the sound-buffer list, and the two raster lists that record how many
 * bytes a voice is costing and which file it came from. A key is four
 * bytes, so what goes in there is a handle.
 *
 * The differential beside this one cannot see that. It compares the
 * rewritten sndbackend.c against a frozen copy that stores truncated
 * addresses, and it declares 84 differences as deliberate -- so a
 * handle in a key would add 24 more by construction and bury the
 * count it exists to protect. It builds with the layer in its identity
 * form instead, which under the -no-pie it already requires is
 * lossless, and keeps its comparison exactly as it was.
 *
 * This is the path it gives up. Everything here builds with the layer
 * on, so a handle is a small number that is nothing like the address
 * it stands for, and the first check is that it is.
 *
 * What is driven is the two directions that matter: a key put in and
 * taken back out again -- the raster pair, where the value stored
 * beside the key has to come back with it -- and a key read back as a
 * pointer and used, which is what the render-mode walk does.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "../sndplatform.h"
#include "../sound.h"
#include "../basic_array.h"
#include "../platform/sndbackend.h"
#include "../handle.h"
#include "sndbackend_mocks.h"

tSplVoice* SPL_p_VoiceCreate(int channels, int freq, int bytes);

#if !HDL_TRANSLATES
#error "this test is the translating path; build it without HDL_TRANSLATES=0"
#endif

void pcSND_AddToSNDRaster(struct sSplVoice* buffer, int bytes);
void pcSND_SNDRasterRelease(struct sSplVoice* buffer);
int pcSND_i_ChangeRenderMode(int mode);
void pcSND_SB_Stop(tSndSoundBufferElem* sbe);
int pcSND_Stream_GetStatus(tSndSoundBufferElem* sndBufferElem, int* status);
int pcSND_ref_SB_GetStatus(tSndSoundBufferElem* ref, int* status);
void pcSND_ref_SB_SetVolume(tSndSoundBufferElem* ref, int att);
void pcSND_ref_StreamGetMonoToStereo(tSndSoundBufferElem** track,
                                     tSndSoundBufferElem** resource);
void pcSND_ref_SB_SetFrequency(tSndSoundBufferElem* ref, int freq);
void pcSND_ref_SB_Stop(tSndSoundBufferElem* ref);
extern int SND_gi_StreamListSize;

extern int SND_gi_RasterCurrentFileId;

#define SENTINEL 0x5EED

static int failures;

/* How many different object ids the log mentions after `what`. The
   mock writes an object as oN, so "att(o" counts the voices the
   attenuation was set on. */
static int count_distinct(const char* log, const char* what)
{
    int seen[256];
    int n = 0;
    const char* at = log;
    size_t len = strlen(what);

    memset(seen, 0, sizeof(seen));
    while ((at = strstr(at, what)) != NULL)
    {
        int id = atoi(at + len);
        at += len;
        if (id >= 0 && id < 256 && !seen[id])
        {
            seen[id] = 1;
            ++n;
        }
    }
    return n;
}

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* Voices and buffer elements at addresses this build really uses.
   A voice is opaque to everything outside the sound platform, so the
   driver only needs distinct addresses of the right alignment; the
   mocks identify one by its address and never read through it. */
#define REFS 12

/* The three lists, which the mock's reset does not reach: it rewinds
   the bump allocator under them, so a list left pointing at its old
   key/value array is pointing at memory about to be handed out again.
   Every check starts by forgetting them, which is what the engine's own
   teardown does with the storage still there. */
static void forget_lists(void)
{
    memset(&dsndSBList, 0, sizeof(dsndSBList));
    memset(&rasterbas1, 0, sizeof(rasterbas1));
    memset(&rasterbas2, 0, sizeof(rasterbas2));
}

static struct sSplVoice* voices[REFS];
static tSndSoundBufferElem buffers[REFS];

/* Real mock voices, because the mocks name a voice by recognising its
   address as one of their own objects and answer -2 for anything else
   -- so twelve addresses of the driver's own would all look like the
   same voice in the log. */
static void make_voices(void)
{
    int i;
    for (i = 0; i < REFS; i++)
    {
        voices[i] = SPL_p_VoiceCreate(1, 22050, 4096);
        if (!voices[i])
        {
            fail("the mock would not make a voice");
            return;
        }
    }
}

static struct sSplVoice* voice_at(int i)
{
    return voices[i];
}

/* The check that keeps every other check here honest.
 *
 * With the layer on, a handle is an index and a generation and is
 * nothing like the address it stands for. If the two ever coincided --
 * a build where the layer is the identity, a value that never went
 * through it -- then a lookup would succeed whether or not anything
 * was translated, and nothing below would mean anything. */
static void check_a_handle_is_not_an_address(void)
{
    int i;

    for (i = 0; i < REFS; i++)
    {
        uint32_t h = HDL_ul_Handle(voice_at(i));

        if (!h)
        {
            fail("the layer gave out no handle at all");
            return;
        }
        if (h == (uint32_t)(uintptr_t)voice_at(i))
        {
            fail("a handle is the address itself, so this test cannot "
                 "tell a translated key from an untranslated one");
            return;
        }
        if (HDL_p_Pointer(h) != voice_at(i))
        {
            fail("a handle does not resolve to what it was made from");
            return;
        }
    }
}

/* The raster lists: a key in, and the value stored beside it back out.
 *
 * Adding a voice records its byte count in one list and the current
 * file id in the other; releasing it takes both back and undoes its
 * share of the statistics. So releasing everything that was added has
 * to leave the statistics exactly where they started -- which only
 * happens if each release found its own voice's two entries. */
static void check_raster_round_trip(int order)
{
    static const int fileIds[4] = { 0x2000, 8, 1, 0x80000 };
    tSndRasterStatStruct before;
    int sizes[REFS];
    int i;

    forget_lists();
    SND_gst_Params.soundEnabled = 1;
    memset(&sndRasterStat, 0, sizeof(sndRasterStat));
    before = sndRasterStat;

    for (i = 0; i < REFS; i++)
    {
        sizes[i] = 1000 + i * 37;
        SND_gi_RasterCurrentFileId = fileIds[i % 4];
        pcSND_AddToSNDRaster(voice_at(i), sizes[i]);
    }

    if (sndRasterStat.totalBytes <= 0)
    {
        fail("adding voices to the raster recorded nothing");
        return;
    }

    /* Released in a different order each time, because the list keeps
       its keys sorted and handles sort differently from addresses --
       so the order a key is found in is not the order it went in. */
    for (i = 0; i < REFS; i++)
    {
        int k = (order == 0) ? i
              : (order == 1) ? REFS - 1 - i
              : (i * 5 + 3) % REFS;
        pcSND_SNDRasterRelease(voice_at(k));
    }

    if (sndRasterStat.totalBytes != before.totalBytes
        || sndRasterStat.trackBytes != before.trackBytes
        || sndRasterStat.loadingSoundBytes != before.loadingSoundBytes
        || sndRasterStat.soundBytes != before.soundBytes)
    {
        printf("  FAIL releasing every voice left the raster statistics "
               "dirty (order %d): total %d, track %d, loading %d, "
               "sound %d\n", order, sndRasterStat.totalBytes,
               sndRasterStat.trackBytes, sndRasterStat.loadingSoundBytes,
               sndRasterStat.soundBytes);
        ++failures;
    }

    /* And a voice that was never added has nothing to take back, so
       releasing it must leave the statistics alone rather than find
       somebody else's entry. */
    pcSND_SNDRasterRelease(voice_at(0));
    if (sndRasterStat.totalBytes != before.totalBytes)
        fail("releasing a voice that was never added changed the "
             "raster statistics");
}

/* The other direction: a key read back out of the list as a pointer.
 *
 * The render-mode walk reads every key in the sound-buffer list,
 * resolves it, and sets the pan and the attenuation on that element's
 * voice. The mock names a voice by its address, so the log says how
 * many distinct voices were reached -- and a key that resolved to
 * anything but the element it was made from cannot produce that
 * element's voice.
 *
 * What this does not pin is the value set, only that the right voice
 * was reached once each. Reading the key without resolving it does not
 * survive that far anyway: the element pointer is then a small number
 * and the walk reads a voice out of it. */
static void check_render_mode_walk(void)
{
    int i;
    int reached = 0;

    forget_lists();
    SND_gst_Params.soundEnabled = 1;

    /* The list stands in for the play path, which is what fills it in
       the engine: the key and the value are both the element. */
    for (i = 0; i < REFS; i++)
    {
        memset(&buffers[i], 0, sizeof(buffers[i]));
        buffers[i].voice = voice_at(i);
        buffers[i].unk2SetTo64InCreate = 64;
        buffers[i].unknownFiledinsub49db40ToSetVolume = 0x5EED;
        BAS_binsert(HDL_ul_Handle(&buffers[i]),
                    (int)HDL_ul_Handle(&buffers[i]), &dsndSBList);
    }

    if (dsndSBList.actElemNum != REFS)
    {
        fail("the sound-buffer list did not take every element");
        return;
    }

    wsnd_log_restart();
    pcSND_i_ChangeRenderMode(1);

    /* One pan and one attenuation per element, and each naming a
       different voice. */
    reached = count_distinct(wsnd_log_text(), "att(o");
    if (reached != REFS)
    {
        printf("  FAIL the walk set the attenuation on %d distinct voices "
               "and the list held %d elements, so a key did not resolve to "
               "the element it was made from\n", reached, REFS);
        ++failures;
    }
    reached = count_distinct(wsnd_log_text(), "pan(o");
    if (reached != REFS)
    {
        printf("  FAIL the walk set the pan on %d distinct voices and the "
               "list held %d elements\n", reached, REFS);
        ++failures;
    }
}

/* And the delete side, driven through the engine's own function.
 *
 * Stopping a buffer takes it out of the sound-buffer list by key. The
 * driver fills the list the way the play path does -- the element's
 * handle as both key and value -- so every stop has to find its own
 * entry and the list has to end up empty. A delete that looked up the
 * address instead would find nothing and leave all twelve behind. */
static void check_stop_removes_its_own_entry(void)
{
    int i;

    forget_lists();
    SND_gst_Params.soundEnabled = 1;

    for (i = 0; i < REFS; i++)
    {
        memset(&buffers[i], 0, sizeof(buffers[i]));
        buffers[i].voice = voice_at(i);
        buffers[i].format = 1;
        BAS_binsert(HDL_ul_Handle(&buffers[i]),
                    (int)HDL_ul_Handle(&buffers[i]), &dsndSBList);
    }
    if (dsndSBList.actElemNum != REFS)
    {
        fail("the sound-buffer list did not take every element");
        return;
    }

    for (i = 0; i < REFS; i++)
        pcSND_SB_Stop(&buffers[i]);

    if (dsndSBList.actElemNum != 0)
    {
        printf("  FAIL stopping all %d buffers left %d in the list, so a "
               "delete did not find its own key\n",
               REFS, dsndSBList.actElemNum);
        ++failures;
    }
}

/* The slot itself: a buffer, an index, and the two not confused.
 *
 * A sound's buffer slot holds a buffer on the plain path, a one-based
 * index on the stream path, whichever of those a track was built from,
 * and nought or -1 for no sound. It is read back as a 32-bit key, so a
 * buffer goes in as a handle and an index goes in tagged.
 *
 * The tag is the part worth checking. Without it the obvious design is
 * to put everything through the handle layer, which works -- the same
 * input gives the same key -- while handing out handles for small
 * integers as though they were addresses. The checks below are what
 * tell the two designs apart: an index must not resolve to a buffer and
 * a buffer must not read as an index, and no amount of consistency
 * gives you that.
 */
static void check_slot_encoding(void)
{
    int i;

    /* A buffer goes in and comes back. */
    for (i = 0; i < REFS; i++)
    {
        tSndRef ref = SND_ref_FromBuffer(&buffers[i]);

        if (SND_ul_RefBits(ref) == (uint32_t)(uintptr_t)&buffers[i])
        {
            fail("a slot holds the buffer's own address, so nothing here "
                 "proves anything");
            return;
        }
        if (SND_p_BufferFromRef(ref) != &buffers[i])
            fail("a buffer did not come back out of its slot");
        if (SND_b_RefIsIndex(ref))
            fail("a slot holding a buffer reads as an index");
    }

    /* An index goes in and comes back, and is not a buffer. */
    for (i = 1; i <= 64; i++)
    {
        tSndRef ref = SND_ref_FromIndex(i);

        if (SND_i_IndexFromRef(ref) != i)
            fail("an index did not come back out of its slot");
        if (!SND_b_RefIsIndex(ref))
            fail("a slot holding an index does not read as one");
        /* The resolver hands back what it did not issue, so an index
           slot resolves to its own bits and never to a live buffer. */
        if (SND_p_BufferFromRef(ref) != (void*)(uintptr_t)SND_ul_RefBits(ref))
            fail("a slot holding an index resolved to something");
    }

    /* And the two encodings do not collide: no buffer's slot is any
       index's slot, which is the property the tag is for. */
    for (i = 0; i < REFS; i++)
    {
        uint32_t bits = SND_ul_RefBits(SND_ref_FromBuffer(&buffers[i]));
        int k;

        for (k = 1; k <= 64; k++)
            if (bits == SND_ul_RefBits(SND_ref_FromIndex(k)))
            {
                fail("a buffer's slot is also an index's slot");
                return;
            }
        /* A buffer's slot read as an index is out of any list's range,
           so the stream family rejects it rather than acting on it. */
        if (SND_i_IndexFromRef(SND_ref_FromBuffer(&buffers[i])) <= 64)
            fail("a buffer's slot reads as a small index");
    }

    /* No sound at all, both spellings, unchanged either way. */
    if (SND_ul_RefBits(SND_ref_FromIndex(0)) != 0)
        fail("no sound became something");
    if (SND_p_BufferFromRef((tSndRef)0) != NULL)
        fail("an empty slot resolved to something");
    if (SND_i_IndexFromRef((tSndRef)(uintptr_t)HDL_NOT_FOUND) != -1)
        fail("the not-found value stopped being itself");
}

/* The stream family, which reads the slot as an index into its list.
 *
 * Its entries answer 0 for an index inside the list and -1 for anything
 * else, so a slot built from a buffer has to be refused -- the one
 * thing a design without the tag cannot promise. */
static void check_stream_family_reads_an_index(void)
{
    int status;
    int i;

    SND_gi_StreamListSize = 8;

    for (i = 1; i <= 8; i++)
        if (pcSND_Stream_GetStatus(SND_ref_FromIndex(i), &status) != 0)
        {
            printf("  FAIL the stream family refused index %d, which is "
                   "inside its list\n", i);
            ++failures;
        }
    if (pcSND_Stream_GetStatus(SND_ref_FromIndex(9), &status) != -1)
        fail("the stream family accepted an index past the end of its list");
    if (pcSND_Stream_GetStatus((tSndRef)0, &status) != -1)
        fail("the stream family accepted an empty slot");

    for (i = 0; i < REFS; i++)
        if (pcSND_Stream_GetStatus(SND_ref_FromBuffer(&buffers[i]),
                                   &status) != -1)
        {
            fail("the stream family took a slot holding a buffer for one "
                 "of its own indices");
            break;
        }
}

/* The plain family, which reads the slot as a buffer.
 *
 * These are the entries the interface table points at, and each one
 * resolves the slot before handing it to the function that does the
 * work. An entry that stopped resolving would look identical to every
 * caller that hands it a raw address, so what is driven here is a slot
 * built the way the engine builds one -- a handle, which is nothing
 * like the buffer it stands for. */
/* A getter that finds nothing leaves the caller's slots as they were.
 *
 * The streaming entry is an empty stub, and before these wrappers
 * existed the interface was wired to it directly: a function that
 * writes nothing left the two slots the caller passed exactly as they
 * were. The wrapper converts a pointer into a slot on the way out, and
 * converting unconditionally turned that stub into something that
 * writes a nought into both of them.
 *
 * One of the two is a track resource list entry, and the next track to
 * take that resource is handed it as its buffer -- so the engine went
 * looking for the current position of a track with no buffer at all,
 * and the stub was what had emptied it. */
static void check_a_getter_that_finds_nothing_leaves_the_slots(void)
{
    tSndSoundBufferElem* track;
    tSndSoundBufferElem* resource;

    printf("-- a getter that hands back nothing leaves the slots alone\n");

    memset(&buffers[0], 0, sizeof(buffers[0]));
    memset(&buffers[1], 0, sizeof(buffers[1]));
    track = (tSndSoundBufferElem*)SND_ref_FromBuffer(&buffers[0]);
    resource = (tSndSoundBufferElem*)SND_ref_FromBuffer(&buffers[1]);

    if (!track || !resource)
        fail("the layer gave out no slot to start the case with");

    pcSND_ref_StreamGetMonoToStereo(&track, &resource);

    if (SND_p_BufferFromRef(track) != &buffers[0])
        fail("a getter that found nothing emptied the track's slot");
    if (SND_p_BufferFromRef(resource) != &buffers[1])
        fail("a getter that found nothing emptied the resource's slot");
}

static void check_plain_family_reads_a_buffer(void)
{
    int status;
    int i;

    forget_lists();
    SND_gst_Params.soundEnabled = 1;

    SND_gst_Params.soundRenderMode = 2;
    for (i = 0; i < REFS; i++)
    {
        memset(&buffers[i], 0, sizeof(buffers[i]));
        buffers[i].voice = voice_at(i);
        /* A different balance each, so the volume each one works out
           is its own and a write to the wrong buffer shows. */
        buffers[i].unk2SetTo64InCreate = 4 + 10 * i;
        buffers[i].unknownFiledinsub49db40ToSetVolume = SENTINEL;
    }

    /* Setting the volume works a value out from the buffer's own
       balance and writes it on the buffer. Nothing else can reach that
       field, so finding it changed is the slot having been resolved. */
    for (i = 0; i < REFS; i++)
        pcSND_ref_SB_SetVolume(SND_ref_FromBuffer(&buffers[i]), -1000);

    for (i = 0; i < REFS; i++)
        if (buffers[i].unknownFiledinsub49db40ToSetVolume == SENTINEL)
        {
            printf("  FAIL the plain family left buffer %d untouched, so "
                   "its slot was not resolved\n", i);
            ++failures;
        }

    /* And each got its own value rather than one buffer getting every
       call, which is what a slot resolving to the wrong thing looks
       like. */
    {
        int distinct = 0;
        int k;
        for (i = 0; i < REFS; i++)
        {
            for (k = 0; k < i; k++)
                if (buffers[k].unknownFiledinsub49db40ToSetVolume
                        == buffers[i].unknownFiledinsub49db40ToSetVolume)
                    break;
            if (k == i)
                ++distinct;
        }
        /* Not all twelve: the volume is clamped at both ends, so some
           balances work out to the same value. Several distinct ones is
           enough to say the calls did not all land on one buffer. */
        if (distinct < 4)
        {
            printf("  FAIL only %d of %d buffers got a volume of their "
                   "own, so the calls are landing on the same one\n",
                   distinct, REFS);
            ++failures;
        }
    }

    /* Reading the status reads through the slot as well, and the mock
       answers from the voice it finds there. */
    for (i = 0; i < REFS; i++)
    {
        status = -1;
        pcSND_ref_SB_GetStatus(SND_ref_FromBuffer(&buffers[i]), &status);
        if (status == -1)
        {
            fail("the plain family read no status through a slot");
            break;
        }
    }

    /* An empty slot is no buffer, and has to be let alone rather than
       followed. */
    pcSND_ref_SB_Stop((tSndRef)0);
    pcSND_ref_SB_SetVolume((tSndRef)0, -500);
}

int main(void)
{
    int order;

    printf("=== a voice and a buffer through the translation layer ===\n");

    wsnd_mock_reset();
    make_voices();

    check_a_handle_is_not_an_address();
    check_slot_encoding();
    check_stream_family_reads_an_index();
    wsnd_mock_reset();
    make_voices();
    check_plain_family_reads_a_buffer();
    check_a_getter_that_finds_nothing_leaves_the_slots();
    for (order = 0; order < 3; order++)
    {
        wsnd_mock_reset();
        make_voices();
        check_raster_round_trip(order);
    }
    wsnd_mock_reset();
    make_voices();
    check_render_mode_walk();
    wsnd_mock_reset();
    make_voices();
    check_stop_removes_its_own_entry();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
