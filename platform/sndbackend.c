#include "../common.h"
// Do not include these! dsound.h defines IKsPropertySet and differently!
//#include <ks.h>
//#include <ksproxy.h>
#include "adpcm.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../time.h"
#include "../dbg.h"
#include "../platform.h"
#include "sndbackend.h"
#include "../sndplatform.h"

/* Tracing the sound thread. Off, and one line to turn on: it prints
   what the thread is doing once a second, the play list and the state
   of every stream on it, and a line at each refill, each removal and
   each stream's beginning and end.

   It sits here, above everything that reads it, and that is the whole
   reason this comment is long. An identifier that is not defined is
   nought in a `#if`, so a `#if PCSND_TRACE` written above the define
   compiles out whichever value the define carries, and says nothing
   about it. That is how seven of these went silently missing while
   the two below the define printed -- three runs of the game produced
   no refill, no removal and no state, and all of it was read as the
   code not reaching those points.

   The sweep in tests/warncheck.sh fails on an undefined identifier in
   a `#if` now, so a trace that cannot print is a failed build rather
   than a quiet lie. */
#define PCSND_TRACE 0


int gpcSNDVolumeAdjustmentArr[] = {
                 0,          0, 0xFFFFFFF6, 0xFFFFFFF6, 0xFFFFFFF6, 
        0xFFFFFFEC, 0xFFFFFFEC, 0xFFFFFFEC, 0xFFFFFFE2, 0xFFFFFFE2, 
        0xFFFFFFD8, 0xFFFFFFD8, 0xFFFFFFD8, 0xFFFFFFCE, 0xFFFFFFCE,
        0xFFFFFFCE, 0xFFFFFFC4, 0xFFFFFFC4, 0xFFFFFFBA, 0xFFFFFFBA, 
        0xFFFFFFBA, 0xFFFFFFB0,0xFFFFFFB0,  0xFFFFFFA6, 0xFFFFFFA6, 
        0xFFFFFF9C, 0xFFFFFF9C, 0xFFFFFF9C, 0xFFFFFF92, 0xFFFFFF92,
        0xFFFFFF88, 0xFFFFFF88, 0xFFFFFF7E, 0xFFFFFF7E, 0xFFFFFF74, 
        0xFFFFFF74, 0xFFFFFF74, 0xFFFFFF6A, 0xFFFFFF6A, 0xFFFFFF60, 
        0xFFFFFF60, 0xFFFFFF56, 0xFFFFFF56, 0xFFFFFF4C, 0xFFFFFF4C, 
        0xFFFFFF42, 0xFFFFFF38, 0xFFFFFF38, 0xFFFFFF2E, 0xFFFFFF2E,
        0xFFFFFF24, 0xFFFFFF24, 0xFFFFFF1A, 0xFFFFFF1A, 0xFFFFFF10, 
        0xFFFFFF06, 0xFFFFFF06, 0xFFFFFEFC, 0xFFFFFEFC, 0xFFFFFEF2, 
        0xFFFFFEE8, 0xFFFFFEE8, 0xFFFFFEDE, 0xFFFFFED4,
        0xFFFFFED4, 0xFFFFFECA, 0xFFFFFEC0, 0xFFFFFEB6, 0xFFFFFEB6,
        0xFFFFFEAC, 0xFFFFFEA2, 0xFFFFFE98, 0xFFFFFE98, 0xFFFFFE8E,
        0xFFFFFE84, 0xFFFFFE7A, 0xFFFFFE70, 0xFFFFFE70, 0xFFFFFE66,
        0xFFFFFE5C, 0xFFFFFE52, 0xFFFFFE48, 0xFFFFFE3E, 0xFFFFFE34,
        0xFFFFFE2A, 0xFFFFFE20, 0xFFFFFE16, 0xFFFFFE0C, 0xFFFFFE02,
        0xFFFFFDF8, 0xFFFFFDE4, 0xFFFFFDDA, 0xFFFFFDD0, 0xFFFFFDC6,
        0xFFFFFDB2, 0xFFFFFDA8, 0xFFFFFD9E, 0xFFFFFD8A, 0xFFFFFD80,
        0xFFFFFD6C, 0xFFFFFD62, 0xFFFFFD4E, 0xFFFFFD3A, 0xFFFFFD30,
        0xFFFFFD1C, 0xFFFFFD08, 0xFFFFFCF4, 0xFFFFFCE0, 0xFFFFFCC2,
        0xFFFFFCAE, 0xFFFFFC9A, 0xFFFFFC7C, 0xFFFFFC5E, 0xFFFFFC40,
        0xFFFFFC22, 0xFFFFFC04, 0xFFFFFBDC, 0xFFFFFBB4, 0xFFFFFB82,
        0xFFFFFB50, 0xFFFFFB14, 0xFFFFFACE, 0xFFFFFA88, 0xFFFFFA24,
        0xFFFFF9A2, 0xFFFFF8F8, 0xFFFFF7CC, 0xFFFFDCB0
};

/* Two registry reads and a hand-loaded EAXDirectSoundCreate8 used to
   stand here: the driver GUID the device was opened on, and whether
   the user had turned the effects on. Nothing opens a named driver
   any more and nothing does effects, so all three are gone. */

static int pcSND_DecodeADPCMBlocks(char* src, int16_t* dest, int blockNum, int channelNum)
{
    pcSND_ADPCMToPCM_Convert(
        (uint8_t*)src,
        dest,
        60 * blockNum,
        channelNum,
        channelNum != 1 ? 72 : 36,
        60u);
    return 1;
}

/* Waiting for the sound thread to say it has done what it was asked.
   The engine hands the thread a command through one of the four
   control events and then waits here for the acknowledgement, with the
   five-second bound it has always had. That bound used to be silent,
   which is the worst way for it to be: five seconds of a stopped game
   and nothing said. If it is ever reached the thread is wedged, so say
   which command it was. */
/* The four commands the engine hands the sound thread. They were the
   indices of four events it waited on, and the thread's switch still
   reads them that way. */
#define PCSND_CMD_ADD    0
#define PCSND_CMD_REMOVE 1
#define PCSND_CMD_EXIT   2
#define PCSND_CMD_STOP   3

static void pcSND_l_Command(int command, const char* what)
{
    if (!SPL_b_Command(command, 5000))
        DBG_OutputDebugString("SND: the sound thread did not answer '%s' in five seconds\n", what);
}

void pcSND_AddSoundToThePlayList(BOOL notifyThread)
{
    int slot;
    size_t bytes;
    HANDLE* handles;

    for (slot = 0; slot < sndEventsNum; ++slot)
    {
        if (!sndPlayList[slot].playListElemInUse)
            break;
    }
    if (slot >= sndEventsNum)
    {
        bytes = 8 * (sndEventsNum + 200);
        if (sndEventsNum)
        {
            sndPlayList = (tPcSndEventRelated*)MEM_p_Realloc(sndPlayList, bytes);
            // preserved: the handle array is grown from sndPlayList, not from sndEventHandles
            handles = (HANDLE*)MEM_p_Realloc(sndPlayList, bytes);
        }
        else
        {
            sndPlayList = (tPcSndEventRelated*)MEM_p_Alloc(bytes);
            handles = (HANDLE*)MEM_p_Alloc(bytes);
        }
        sndEventHandles = handles;
        memset(&sndPlayList[sndEventsNum], 0, sizeof(tPcSndEventRelated) * 200);
        memset(&sndEventHandles[2 * sndEventsNum], 0, 3200u); //TODO ez hogy jön ki? 2x4x200 az csak 1600
        slot = sndEventsNum;
        sndEventsNum += 200;
    }
    sndPlayList[slot].playListElemInUse = 1;
    sndPlayList[slot].soundElem = sndCurrSoundBufferElem;
    sndEventHandles[2 * slot] = sndCurrSoundBufferElem->miscData->eventHandle1;
    sndEventHandles[2 * slot + 1] = sndCurrSoundBufferElem->miscData->eventHandle2;
    sndEventHandleNum += 2;
#if PCSND_TRACE
    DBG_OutputDebugString("SND add: slot %d sbe=%p misc=%p voice=%p handles=%d\n",
                          slot, (void*)sndCurrSoundBufferElem,
                          (void*)(sndCurrSoundBufferElem ? sndCurrSoundBufferElem->miscData : 0),
                          (void*)(sndCurrSoundBufferElem ? sndCurrSoundBufferElem->voice : 0),
                          sndEventHandleNum);
#endif
    if (notifyThread)
        SPL_CommandDone();
}

void pcSND_RemoveSoundFromPlayList(tSndSoundBufferElem* sbe, int fireEvent)
{
    int slot;
    int next;
    int count;

    if (sbe == (tSndSoundBufferElem*)-1)
    {
        sndEventHandleNum = 0;
        if (fireEvent)
            SPL_CommandDone();
        return;
    }
    if ((sndEventHandleNum & 0xFFFFFFFE) != 0)
    {
        count = sndEventHandleNum >> 1;
        for (slot = 0; slot < count; ++slot)
        {
            if (sndPlayList[slot].soundElem == sbe)
                break;
        }
        if (slot < count)
        {
            for (next = slot + 1; next < count; ++next)
            {
                sndPlayList[slot].soundElem = sndPlayList[next].soundElem;
                sndEventHandles[2 * slot] = sndEventHandles[2 * next];
                sndEventHandles[2 * slot + 1] = sndEventHandles[2 * next + 1];
                ++slot;
            }
            sndPlayList[slot].soundElem = 0;
            sndPlayList[slot].playListElemInUse = 0;
            sndEventHandles[2 * slot] = 0;
            sndEventHandles[2 * slot + 1] = 0;
            sndEventHandleNum -= 2;
        }
    }
#if PCSND_TRACE
    DBG_OutputDebugString("SND rm: sbe=%p misc=%p handles=%d\n",
                          (void*)sbe, (void*)sbe->miscData, sndEventHandleNum);
#endif
    /* Cleared rather than reset: these are the tokens described in
       pcSND_StartSoundThread, and nought is what the engine reads as
       "this sound is not on the list". */
    sbe->miscData->eventHandle1 = 0;
    sbe->miscData->eventHandle2 = 0;
    if (fireEvent)
        SPL_CommandDone();
}

void pcSND_StopSoundAndRemoveFromThePlayList(tSndSoundBufferElem* sbe, int fireEvent)
{
    SPL_VoiceStop(sbe->voice);
    pcSND_RemoveSoundFromPlayList(sbe, fireEvent);
}

void pcSND_updateCurrentPlayPositionAndSize(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    int advanced;
    int playPos;

    playPos = SPL_i_VoiceCursor(sbe->voice);
    misc = sbe->miscData;
    if (playPos >= misc->currentPlayPosition)
        advanced = playPos - misc->currentPlayPosition;
    else
        advanced = playPos + misc->soundBufferSize - misc->currentPlayPosition;
    misc->currSize += advanced;
    misc->currentPlayPosition = playPos;
}

unsigned int pcSND_CalcSizeToReadByFormat(unsigned int byteCount, unsigned int channels)
{
    unsigned int frames;

    frames = (byteCount >> 1) / channels;
    if (channels == 1)
        return 36 * (frames / 60);
    return 72 * (frames / 60);
}

unsigned int pcSND_calcBlockNumForDecompress(int size, int channels)
{
    if (channels == 1)
        return size / 36;
    return size / 72;
}

unsigned int pcSND_calcMemoryNeededForTheSound(unsigned int fullsoundlen, int channels)
{
    if (channels == 1)
        return 120 * (fullsoundlen / 36);
    return 120 * channels * (fullsoundlen / 72);
}

void pcSND_ReadAndDecompressSoundData(unsigned int wanted, tSndSoundBufferElem* sbe, int useFirstBuffer, uint32_t* decodedSize, int16_t* dest)
{
    tSbeMiscData* misc;
    unsigned int readSize;
    int blockNum;
    DWORD decoded;
    int16_t* src;
    int16_t* mixDest;
    DWORD done;
    int left;
    int right;
    unsigned int mixed;
    int channels;
    void* buffer;

    if (useFirstBuffer)
        buffer = dsndReadBuffer1;
    else
        buffer = dsndReadBuffer2;
    misc = sbe->miscData;
    if (misc->fullSize - misc->sumSize < wanted)
        wanted = misc->fullSize - misc->sumSize;
    readSize = pcSND_CalcSizeToReadByFormat(wanted, sbe->format);
    memset(buffer, 0, 307200u);
    *decodedSize = (uint32_t)SPL_i_FileRead(sbe->miscData->fileHandle, buffer, (int)readSize);
    channels = sbe->format;
    blockNum = pcSND_calcBlockNumForDecompress(readSize, channels);
    pcSND_DecodeADPCMBlocks((char*)buffer, dest, blockNum, channels);
    decoded = pcSND_calcMemoryNeededForTheSound(*decodedSize, sbe->format);
    *decodedSize = decoded;
    sbe->miscData->sumSize += decoded;
    if (sbe->format == 2 && SND_gst_Params.soundRenderMode == 1)// mono-ra alakítás?
    {
        src = dest;
        mixDest = dest;
        for (done = 0; done < *decodedSize; done += 4)
        {
            left = *src;
            right = src[1];
            src += 2;
            mixed = (unsigned int)(((double)left + (double)right) * 0.5);
            *mixDest = mixed;
            mixDest[1] = mixed;
            mixDest += 2;
        }
    }
}

/* The staging buffer, grown to what is asked of it. Never clamped:
   see the note in sndbackend.h. */
static char* pcSND_l_p_UploadBuffer(int bytes)
{
    void* grown;

    if (bytes <= 0)
        return 0;
    if ((size_t)bytes > dsndUploadBufferSize)
    {
        grown = realloc(dsndUploadBuffer, (size_t)bytes);
        if (!grown)
            return 0;
        dsndUploadBuffer = (uint8_t*)grown;
        dsndUploadBufferSize = (size_t)bytes;
    }
    return (char*)dsndUploadBuffer;
}

/* Where in the big file a sound begins.
 *
 * The offset is held as an int and was handed to SetFilePointer as
 * the low half of a 64-bit distance, with the high half nought --
 * and in that call the low half is **unsigned**. So an offset past
 * two gigabytes is a large positive number there and a negative one
 * to anything that widens the int by its sign, which is what reading
 * it straight into a 64-bit seek does.
 *
 * A negative seek is refused, and a refused seek leaves the file
 * where it was: the next read takes whatever is under the pointer
 * and the decoder makes noise of it. Hence the cast, which is the
 * arithmetic the original did. */
#define PCSND_FILE_OFFSET(x) ((long long)(unsigned int)(x))

/* Which block of the buffer the cursor is in. The buffer is a whole
   number of sizeToUpload blocks and the device used to fire an event
   as the cursor entered each one. */
static int pcSND_l_PlayedBlock(const tSbeMiscData* misc)
{
    if (misc->sizeToUpload <= 0)
        return 0;
    return misc->currentPlayPosition / misc->sizeToUpload;
}

/* Whether a second has gone by, which is how often the state below
   goes out. One clock for the list and for every stream on it, so a
   report is one moment and not several. */
#if PCSND_TRACE
static int pcSND_l_stateDue;
#endif

/* One streaming sound, looked at rather than waited on.
 *
 * This is the body of what the position notifications used to drive.
 * Two of them mattered: one fired as the cursor entered each block of
 * the buffer and meant "refill the block behind you", and one fired
 * when the buffer stopped and meant "take this off the list". SDL has
 * neither, so the thread asks the voice where its cursor is and works
 * both out: a block it has not seen the cursor in before is the
 * first, and a voice that is no longer playing is the second. */
static void pcSND_l_ServiceStream(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    unsigned int audioBytes1;
    char* audioPtr1;
    DWORD produced;
    DWORD totalSizeAfterDecompress;
    int block;
    int stopped;

    misc = sbe->miscData;

    /* Everything about one stream, once a second, before any way out
       of this function.
       
       The lines below print when something happens, and a stream that
       has gone quiet is the case where nothing does -- no refill, no
       removal, and the thread's own summary still counting it as a
       stream. That is three different faults with one appearance, and
       only the state tells them apart: whether the voice still reads
       as playing, whether the play cursor is moving, whether the
       device is still draining what it was given, and which block the
       refill is waiting for. */
#if PCSND_TRACE
    if (pcSND_l_stateDue)
    {
        if (!misc)
            DBG_OutputDebugString("SND %p: no miscData, so it is skipped here and never removed\n",
                                  (void*)sbe);
        else
            DBG_OutputDebugString(
                "SND %p: vst=%d pos=%d wr=%d q=%d blk=%d/%d up=%d buf=%d off=%d sz=%u/%u raf=%d rem=%d stop=%d\n",
                (void*)sbe,
                SPL_i_VoiceStatus(sbe->voice),
                SPL_i_VoiceCursor(sbe->voice),
                SPL_i_VoiceWriteCursor(sbe->voice),
                SPL_i_VoiceQueued(sbe->voice),
                pcSND_l_PlayedBlock(misc), misc->lastPlayedBlock,
                misc->sizeToUpload, misc->soundBufferSize, misc->currOffset,
                misc->currSize, misc->fullSize,
                misc->RemoveAfterPlayFinished,
                misc->shouldBeRemovedFromPlayList,
                misc->Stopped_SetIN49e270);
    }
#endif

    if (!misc || misc->Stopped_SetIN49e270 == 1)
        return;

    stopped = (SPL_i_VoiceStatus(sbe->voice) & SPL_PLAYING) == 0;
    pcSND_updateCurrentPlayPositionAndSize(sbe);
    if (misc->RemoveAfterPlayFinished && misc->currSize >= misc->fullSize)
        misc->shouldBeRemovedFromPlayList = 1;
    if (misc->shouldBeRemovedFromPlayList == 1 || stopped)
    {
#if PCSND_TRACE
        DBG_OutputDebugString("SND drop %p: %s (curr %u full %u raf %d off %d)\n",
                              (void*)sbe,
                              misc->shouldBeRemovedFromPlayList == 1 ? "asked for" : "voice stopped",
                              misc->currSize, misc->fullSize,
                              misc->RemoveAfterPlayFinished, misc->currOffset);
#endif
        pcSND_StopSoundAndRemoveFromThePlayList(sbe, 0);
        return;
    }

    block = pcSND_l_PlayedBlock(misc);
    if (block == misc->lastPlayedBlock)
        return;
    misc->lastPlayedBlock = block;

    if (misc->RemoveAfterPlayFinished)
    {
        SPL_b_VoiceSilence(sbe->voice, misc->currOffset, misc->sizeToUpload);
        misc->currOffset += misc->sizeToUpload;
        if (misc->currOffset >= misc->soundBufferSize)
            misc->currOffset -= misc->soundBufferSize;
        return;
    }

    /* Decoded into a staging buffer and then handed over, where it
       used to be decoded straight into a locked stretch of the
       buffer. The voice does its own wrapping, so there is no second
       pointer to carry on into and no short lock to account for. */
    audioBytes1 = misc->sizeToUpload;
    audioPtr1 = pcSND_l_p_UploadBuffer((int)audioBytes1);
    if (!audioPtr1)
        return;
    totalSizeAfterDecompress = 0;
    pcSND_ReadAndDecompressSoundData(audioBytes1, sbe, 1, &totalSizeAfterDecompress, (int16_t*)audioPtr1);
    produced = totalSizeAfterDecompress;
    if (totalSizeAfterDecompress < audioBytes1)
    {
        if (misc->readThroughWithMultipleLoop)
        {
            do
            {
                SPL_b_FileSeek(misc->fileHandle, PCSND_FILE_OFFSET(misc->distanceToMove));
                misc->sumSize = 0;
                pcSND_ReadAndDecompressSoundData(audioBytes1 - produced, sbe, 1, &totalSizeAfterDecompress,
                                                 (int16_t*)&audioPtr1[produced]);
                /* A pass that seeks back to the start and still decodes
                   nothing will never decode anything, and this loop had
                   no way out of that: it is the one place the engine
                   reads a sound round and round rather than to its end.
                   It could not be reached before because the refill was
                   gated on locking a stretch of the buffer first, and a
                   lock that failed left nothing to fill; the voice's
                   write cannot fail, so the gate has gone and the loop
                   is reachable. Treated as the end of the sound, which
                   is what the arm below does when a single read falls
                   short. */
                if (totalSizeAfterDecompress == 0)
                {
                    misc->RemoveAfterPlayFinished = 1;
                    memset(&audioPtr1[produced], 0, audioBytes1 - produced);
                    break;
                }
                produced += totalSizeAfterDecompress;
            } while (produced < audioBytes1);
        }
        else
        {
            misc->RemoveAfterPlayFinished = 1;
            memset(&audioPtr1[totalSizeAfterDecompress], 0, audioBytes1 - totalSizeAfterDecompress);
        }
    }
#if PCSND_TRACE
    DBG_OutputDebugString("SND fill %p: block %d off %d want %u got %u%s (curr %u full %u)\n",
                          (void*)sbe, block, misc->currOffset, audioBytes1, produced,
                          misc->RemoveAfterPlayFinished ? " LAST" : "",
                          misc->currSize, misc->fullSize);
#endif
    SPL_b_VoiceWrite(sbe->voice, misc->currOffset, audioPtr1, (int)audioBytes1);
    misc->currOffset += audioBytes1;
    if (misc->currOffset >= misc->soundBufferSize)
        misc->currOffset -= misc->soundBufferSize;
}

/* Every streaming sound on the list, once. */
static void pcSND_l_ServiceStreams(void)
{
    int slot;

    /* The live length of the list, which is what the rest of the file
       reads out of sndEventHandleNum: two handles went on it per
       sound, and the first two slots are the control events' with no
       sound of their own. */
    int count = sndEventHandleNum >> 1;

    if (count > sndEventsNum)
        count = sndEventsNum;

    /* The list as the loop sees it, once a second, ahead of the
       guards below.
       
       A stream the thread's own summary counts and that never prints a
       line of its own is being skipped here, and this says by which
       guard: not in use, no sound, or the -1 the engine uses to mean
       all of them. If it passes all three it is serviced, and then it
       says so itself -- including when its miscData has gone, which
       the state line used to return before reaching. */
#if PCSND_TRACE
    {
        static DWORD lastList;
        DWORD nowList = GetTickCount();

        pcSND_l_stateDue = (nowList - lastList >= 1000);
        if (pcSND_l_stateDue)
        {
            int i;
            lastList = nowList;
            DBG_OutputDebugString("SND list: handles=%d slots=%d count=%d\n",
                                  sndEventHandleNum, sndEventsNum, count);
            for (i = 0; i < count && i < 8; ++i)
                DBG_OutputDebugString("SND   [%d] inUse=%d elem=%p misc=%p voice=%p\n",
                                      i, sndPlayList[i].playListElemInUse,
                                      (void*)sndPlayList[i].soundElem,
                                      sndPlayList[i].soundElem
                                          ? (void*)sndPlayList[i].soundElem->miscData : (void*)0,
                                      sndPlayList[i].soundElem
                                          ? (void*)sndPlayList[i].soundElem->voice : (void*)0);
        }
    }
#endif

    for (slot = 0; slot < count; ++slot)
        if (sndPlayList[slot].playListElemInUse && sndPlayList[slot].soundElem
            && sndPlayList[slot].soundElem != (tSndSoundBufferElem*)-1)
        {
            pcSND_l_ServiceStream(sndPlayList[slot].soundElem);
        }
}

static unsigned int pcSND_l_feeds;
static unsigned int pcSND_l_ticks;
static unsigned int pcSND_l_wakes;

static void pcSND_l_Feed(void)
{
    ++pcSND_l_feeds;
    SPL_Feed();
}

/* What the thread has been doing, once a second. A thread that is
   being woken by control events faster than its own timeout shows up
   here as wakes far outnumbering ticks. */
static void pcSND_l_ThreadStats(int wasTick)
{
#if PCSND_TRACE
    static DWORD last;
    DWORD now;

    if (wasTick)
        ++pcSND_l_ticks;
    else
        ++pcSND_l_wakes;
    now = GetTickCount();
    if (now - last < 1000)
        return;
    last = now;
    DBG_OutputDebugString("SND thread: %u ticks, %u event wakes, %u feeds, %d streams\n",
                          pcSND_l_ticks, pcSND_l_wakes, pcSND_l_feeds,
                          (sndEventHandleNum >> 1) - 2);
    pcSND_l_ticks = 0;
    pcSND_l_wakes = 0;
    pcSND_l_feeds = 0;
#else
    (void)wasTick;
#endif
}

/* How long the thread sleeps when nothing has asked it for anything.
   Short against a block of sound -- the buffer is four blocks and a
   block is tens of milliseconds -- so a refill is never late, and
   long enough that the thread is asleep almost all of the time. */
#define PCSND_SERVICE_MS  5

int pcSND_SoundThread(void* parameter)
{
    int signalled;

    while (1)
    {
        /* One command or a timeout, where it used to be four events
           and one per refill point of every streaming sound. The
           timeout is what the refill points were: the thread wakes,
           looks at every stream's cursor and does what the device's
           own notification would have made it do.
           
           Only ever one command is outstanding -- the engine waits for
           the answer before it returns -- so what was a wait on four
           events is a wait on one mailbox, and the command is the
           index of the event that used to carry it. */
        signalled = SPL_i_TakeCommand(PCSND_SERVICE_MS);
        /* A wait that failed is waited again and is not a tick, which
           is what the loop around it used to say. */
        if (signalled == SPL_CMD_ERROR)
            continue;

        /* The device is fed whatever woke the thread.
           
           It used to be fed only on the timeout, on the reasoning that
           the timeout is what the refill notifications were. That is
           true of the refills and false of the feed: a control event
           returns from the wait at once, so a run of them -- which is
           what creating sounds looks like, each one an event and an
           acknowledgement -- costs a feed every time, and the device
           goes hungry exactly when the engine is busiest. The queue is
           four short chunks, so a few missed feeds are audible. */
        pcSND_l_Feed();

        if (signalled == SPL_CMD_NONE)
        {
            /* The refill points: every streaming sound's cursor looked
               at, and whatever the device's own notification would have
               made the thread do. */
            pcSND_l_ServiceStreams();
            /* Again, because servicing is what puts sound into the
               buffers the feed reads out of. */
            pcSND_l_Feed();
            pcSND_l_ThreadStats(1);
            continue;
        }
        pcSND_l_ThreadStats(0);

        switch (signalled)
        {
        case PCSND_CMD_ADD:
            pcSND_AddSoundToThePlayList(1);
            break;
        case PCSND_CMD_REMOVE:
            pcSND_RemoveSoundFromPlayList(sndCurrSoundBufferElem, 1);
            break;
        case PCSND_CMD_EXIT:
            /* Returning is what ExitThread was, and the only way out
               of this loop. */
            SPL_CommandDone();
            return 0;
        case PCSND_CMD_STOP:
            pcSND_StopSoundAndRemoveFromThePlayList(sndCurrSoundBufferElem, 1);
            break;
        default:
            break;
        }
    }
    return 0;
}

static void pcSND_StartSoundThread(void)
{
    unsigned int i;

    if (!SND_gc_NoSound)
    {
        pcSNDThreadId = 0;
        pcSNDhObject = NULL;
        sndCurrSoundBufferElem = 0;
        trackEventsArrayNextIdx = 0;
        dsndReadBuffer1 = malloc(307200u);
        dsndReadBuffer2 = malloc(307200u);
        /* Twenty-three events were made here and four of them were
           ever waited on. The four and their answer are the mailbox
           now; the rest were handed out two per streaming sound, and
           no one has signalled or waited on one of those since the
           refill notifications went. What is left of them is a value
           the engine tells apart from nought and from -1, so that is
           all they are -- a token, and no object behind it. */
        for (i = 0; i < 23; ++i)
            dsndEvents[i] = (HANDLE)(size_t)(i + 1);
        sndEventsNum = 23;
        sndPlayList = (tPcSndEventRelated*)MEM_p_Alloc(sizeof(tPcSndEventRelated) * 23);
        memset(sndPlayList, 0, 8 * sndEventsNum);
        sndEventHandles = (HANDLE*)MEM_p_Alloc(8 * sndEventsNum);
        memset(sndEventHandles, 0, 8 * sndEventsNum);
        sndEventHandleNum = 4;
        for (i = 0; i < 2; ++i)
        {
            sndEventHandles[2 * i] = dsndEvents[2 * i];
            sndEventHandles[2 * i + 1] = dsndEvents[2 * i + 1];
            sndPlayList[i].playListElemInUse = 1;
            sndPlayList[i].soundElem = 0;
        }
        pcSNDhObject = SPL_p_ThreadStart(pcSND_SoundThread, 0, "BGE sound");
    }
}

/* Opening the device.
   
   What used to be here was DirectSound's own ceremony: a primary
   buffer set to 48 kHz stereo, a 3D listener queried off it, and a
   10 kHz mono buffer made for no other purpose than asking the EAX
   listener property set whether it was there. None of it has a
   successor. The device is 48 kHz stereo because that is what the
   primary buffer was set to, and the voices convert to it; there is
   no listener, and there is no EAX. */
int pcSND_l_Init(tSndHandles* sndh)
{
    BAS_binit(&dsndSBList, 48);
    memset(&gDoSoundGlobal, 0, sizeof(gDoSoundGlobal));
    pcSNDhObject = NULL;
    gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 = 0;
    sndh->MainWindowHandle = MAI_gh_MainWindow;
    /* No COM to initialise, no registry to read for a driver GUID or
       an EAX setting, and no cooperative level: the window does not
       own the device any more. */
    if (!SPL_b_Open())
        return 0;
    sndh->deviceOpen = 1;
    pcSND_StartSoundThread();
    return 1;
}

int pcSND_i_ChangeRenderMode(int mode)
{
    int i;
    tSndSoundBufferElem* sbe;
    int balance;
    int vol;
    int plain;
    int level;
    int pan;

    SND_gst_Params.soundRenderMode = mode;
    for (i = 0; i < dsndSBList.actElemNum; ++i)
    {
        sbe = (tSndSoundBufferElem*)dsndSBList.keyValPtr[i].key;
        if (sbe != (tSndSoundBufferElem*)-1)
        {
            (vol = SPL_i_VoiceAttenuation(sbe->voice));
            balance = sbe->unk2SetTo64InCreate;
            if (SND_gst_Params.soundRenderMode == 1)
            {
                if (balance <= 64)
                    plain = vol - gpcSNDVolumeAdjustmentArr[balance];
                else
                    plain = vol - gpcSNDVolumeAdjustmentArr[127 - balance];
                pan = 0;
                vol = gpcSNDVolumeAdjustmentArr[64] + plain;
            }
            else
            {
                vol -= gpcSNDVolumeAdjustmentArr[64];
                if (balance < 64)
                {
                    level = gpcSNDVolumeAdjustmentArr[balance];
                    pan = gpcSNDVolumeAdjustmentArr[127 - balance] - level;
                }
                else if (balance == 64)
                {
                    level = gpcSNDVolumeAdjustmentArr[64];
                    pan = 0;
                }
                else
                {
                    level = gpcSNDVolumeAdjustmentArr[127 - balance];
                    pan = level - gpcSNDVolumeAdjustmentArr[balance];
                }
                vol += level;
            }
            SPL_VoiceSetPan(sbe->voice, pan);
            SPL_VoiceSetAttenuation(sbe->voice, vol);
        }
    }
    return 0;
}

/* A 3D buffer used to be made here and parked at a fixed position
   with fixed distances. That was not spatialisation: the engine works
   out its own volume and pan, and the 3D buffer existed because the
   EAX per-buffer reverb needed one to hang off. There is nothing to
   make now, and nothing that wanted it. */
int pcSND_SetDefaultSoundPositionAndDistance(tSndSoundBufferElem* sbe)
{
    (void)sbe;
    return 0;
}

/* The room reverb went through the buffer's property set, which was
   EAX's. There is none, so this answers what it answered on a machine
   whose driver had no EAX: it cannot be set. Every caller already
   coped with that, because plenty of cards did not have it. */
int pcSND_SNDPropertySetRoomReverb(tSndSoundBufferElem* sbe, int enable)
{
    (void)sbe;
    (void)enable;
    return -1;
}

/* The status word the engine's own tests read. Bit 0 is playing and
   bit 2 is looping, which are the values sound.c compares against --
   see the "(status & 4) == 0 && (status & 1) != 0" it uses to pick a
   voice to steal. Bit 4 is the engine's own mark for a streamed
   sound and is added by its caller. */
#define PCSND_PLAYING  1
#define PCSND_LOOPING  4

static int pcSND_l_VoiceStatus(struct sSplVoice* voice)
{
    int spl = SPL_i_VoiceStatus(voice);
    int status = 0;

    if (spl & SPL_PLAYING)
        status |= PCSND_PLAYING;
    if (spl & SPL_LOOPING)
        status |= PCSND_LOOPING;
    return status;
}

void pcSND_DisableReverbAndRestartPlaying(tSoundFxParams* params, tSndSoundBufferElem* sbe)
{
    unsigned int status;

    if (pcSNDFXEnabled_dword_B30400)
    {
        status = pcSND_l_VoiceStatus(sbe->voice);
        if ((status & 1) != 0)
            SPL_VoiceStop(sbe->voice);
        pcSND_SNDPropertySetRoomReverb(sbe, 0);
        if ((status & 1) != 0)
        {
            SPL_VoicePlay(sbe->voice, (status >> 2) & 1);
        }
    }
}

int pcSND_DisableReverbIf_i7_not_match_with_idx(tSoundFxParams* params, tSndSoundBufferElem* sbe)
{
    int count;
    int idx;

    count = params->voiceListCount;
    for (idx = 0; idx < count; ++idx)
    {
        if (params->voiceList[idx] == sbe)
            break;
    }
    if (idx != count)
    {
        pcSND_DisableReverbAndRestartPlaying(params, sbe);
        params->voiceList[idx] = NULL;
    }
    return 0;
}

void pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(tSndSoundBufferElem* sbe)
{
    if (sbe->miscData->eventHandle2 != (HANDLE)-1)
    {
        sndCurrSoundBufferElem = sbe;
        pcSND_l_Command(PCSND_CMD_STOP, "stop");
    }
}

void pcSND_SB_Stop(tSndSoundBufferElem* sbe)
{
    int wasListed;

    if (sbe)
    {
        BAS_bdeletegetval((int)sbe, &dsndSBList, &wasListed);
        if (sbe->miscData)
        {
            if (wasListed)
                gDoSoundGlobal.dword_B303C0 -= sbe->format;
            pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(sbe);
        }
        else
        {
            if (wasListed)
                gDoSoundGlobal.sndConcurrentlyPlayedNum -= sbe->format;
            pcSND_DisableReverbIf_i7_not_match_with_idx(&SND_gst_FxDefaultParam, sbe);
            SPL_VoiceStop(sbe->voice);
        }
    }
}

void pcSND_stopSoundIfReadFinished(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;

    misc = sbe->miscData;
    if (misc && !misc->readThroughWithMultipleLoop && misc->currSize >= misc->fullSize)
        pcSND_SB_Stop(sbe);
}

int pcSND_SB_GetStatus(tSndSoundBufferElem* sbe, int* status)
{
    int result;

    if (!sbe)
        return 0;
    if (sbe->miscData)
        pcSND_stopSoundIfReadFinished(sbe);
    *status = pcSND_l_VoiceStatus(sbe->voice);
    result = 0;
    if (sbe->miscData && (*status & 1) != 0)
        *status |= 0x10u;
    return result;
}

int pcSND_SB_Play(tSndSoundBufferElem* sbe, int flags)
{
    int playing;

    if (!sbe)
        return -1;
    if (sbe->miscData)
        return -1;
    playing = gDoSoundGlobal.sndConcurrentlyPlayedNum + sbe->format;
    if (playing > 44)
        return -1;
    gDoSoundGlobal.sndConcurrentlyPlayedNum += sbe->format;
    if (playing > gDoSoundGlobal.sndConcurrentlyPlayedPeakNum)
        gDoSoundGlobal.sndConcurrentlyPlayedPeakNum = playing;
    SPL_VoicePlay(sbe->voice, flags & 1);
    /* The old play reported whether the card had given the buffer a
       hardware voice. Every voice is mixed in software now, so there is
       one answer and the other message is gone. */
    OutputDebugStringA("HARDWARE voice\n");
    BAS_binsert((unsigned int)sbe, (int)sbe, &dsndSBList);
    return 0;
}

static int pcSND_RasterBudgetLeft(void)
{
    return 6000000 - sndRasterStat.totalBytes;
}

void pcSND_AddToSNDRaster(struct sSplVoice* buffer, int bytes)
{
    int fileId;

    if (SND_gst_Params.soundEnabled)
    {
        BAS_binsert((unsigned int)buffer, bytes, &rasterbas1);
        BAS_binsert((unsigned int)buffer, SND_gi_RasterCurrentFileId, &rasterbas2);
        fileId = SND_gi_RasterCurrentFileId;
        if (fileId == 0x2000 || fileId == 0x4000 || fileId == 0x80000)
            sndRasterStat.trackBytes += bytes;
        else if (fileId == 8)
            sndRasterStat.loadingSoundBytes += bytes;
        else
            sndRasterStat.soundBytes += bytes;
        sndRasterStat.totalBytes += bytes;
        sndRasterStat.budgetLeft = pcSND_RasterBudgetLeft();
    }
}

tSndSoundBufferElem* pcSND_pst_SB_Create(tSndHandles* handles, tSndWavHdrStruct* sndHdr, int sndStartPos)
{
    char* sndData;
    char* cursor;
    char* uncompressedSnd;
    unsigned int uncompressedSize;
    int packedSize;
    unsigned int decodedSize;
    unsigned int blockAlign;
    DWORD samplesPerSec;
    tSndSoundBufferElem* sndSNDBuf;

    if (!handles->deviceOpen)
        return 0;
    if (sndHdr->wfex.wFormatTag != 2)
        return 0;
    sndData = 0;
    if (sndStartPos)
    {
        sndData = (char*)MEM_p_Alloc(sndHdr->fullSoundSize);
        if (!sndData)
            return 0;
        if (!LOA_gb_SpeedMode || !isSpeedModeAndBinFile())
        {
            BIG_ReadNoSeek(sndStartPos, sndData, sndHdr->fullSoundSize);
        }
        else if (sndHdr->wfex.nChannels == 1)
        {
            cursor = LOA_FetchBuffer(sndHdr->fullSoundSize);
            LOA_ReadCharArray(&cursor, sndData, sndHdr->fullSoundSize, 1);
        }
        else
        {
            packedSize = sndHdr->fullSoundSize;
            sndHdr->wfex.cbSize >>= 1;
            sndHdr->fullSoundSize = packedSize >> 1;
            cursor = LOA_FetchBuffer(packedSize >> 1);
            LOA_ReadCharArray(&cursor, sndData, sndHdr->fullSoundSize, 1);
            cursor = LOA_FetchBuffer(sndHdr->fullSoundSize);
            LOA_ReadCharArray(&cursor, &sndData[sndHdr->fullSoundSize], sndHdr->fullSoundSize, 1);
        }
    }
    if (sndHdr->wfex.wFormatTag != 2)
    {
        if (sndData)
            MEM_Free(sndData);
        return 0;
    }
    if (sndStartPos)
    {
        uncompressedSize = pcSND_calcMemoryNeededForTheSound(sndHdr->fullSoundSize, sndHdr->wfex.nChannels);
        uncompressedSnd = (char*)MEM_p_Alloc(uncompressedSize);
        memset(uncompressedSnd, 0, uncompressedSize);
        pcSND_DecodeADPCMBlocks(sndData, (int16_t*)uncompressedSnd, sndHdr->fullSoundSize / sndHdr->wfex.nBlockAlign, sndHdr->wfex.nChannels);
        MEM_Free(sndData);
        sndData = uncompressedSnd;
    }
    decodedSize = pcSND_calcMemoryNeededForTheSound(sndHdr->fullSoundSize, sndHdr->wfex.nChannels);
    samplesPerSec = sndHdr->wfex.nSamplesPerSec;
    sndHdr->fullSoundSize = decodedSize;
    blockAlign = 2 * sndHdr->wfex.nChannels;
    sndHdr->wfex.nBlockAlign = blockAlign;
    sndHdr->wfex.wBitsPerSample = 16;
    sndHdr->wfex.wFormatTag = 1;
    sndHdr->wfex.cbSize = 0;
    sndHdr->wfex.nAvgBytesPerSec = (uint16_t)blockAlign * samplesPerSec;
    sndSNDBuf = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(sndSNDBuf, 0, sizeof(tSndSoundBufferElem));
    sndSNDBuf->unk2SetTo64InCreate = 64;
    /* A buffer description used to be filled in here -- its size, its
       capability flags, a 3D algorithm GUID -- and handed over. The
       voice takes the three things the description was carrying, and
       they all come from the wav header the caller already has. */
    sndSNDBuf->voice = SPL_p_VoiceCreate(sndHdr->wfex.nChannels,
                                         sndHdr->wfex.nSamplesPerSec,
                                         sndHdr->fullSoundSize);
    if (!sndSNDBuf->voice)
    {
        if (sndData)
            MEM_Free(sndData);
        MEM_Free(sndSNDBuf);
        return 0;
    }
    sndSNDBuf->format = sndHdr->wfex.nChannels;
    pcSND_AddToSNDRaster(sndSNDBuf->voice, sndHdr->fullSoundSize);
    if (!sndData)
        return sndSNDBuf;
    /* The whole sound, in one write. It used to be a lock that could
       hand back two stretches of a circular buffer and a copy into
       each; the voice does its own wrapping, so there is one call and
       nothing to stitch. */
    if (!SPL_b_VoiceWrite(sndSNDBuf->voice, 0, sndData, sndHdr->fullSoundSize))
    {
        SPL_VoiceDestroy(sndSNDBuf->voice);
        MEM_Free(sndSNDBuf);
        MEM_Free(sndData);
        return NULL;
    }
    pcSND_SetDefaultSoundPositionAndDistance(sndSNDBuf);
    MEM_Free(sndData);
    return sndSNDBuf;
}

/* The 3D listener, the buffer EAX was probed on, its 3D buffer and
   its property set were released here. None of the four is made any
   more, so there is nothing to give back. */
int pcSND_DSoundBuffersRelease()
{
    return 0;
}

void pcSND_Cleanup()
{
    int i;

    if (!SND_gc_NoSound)
    {
        sndCurrSoundBufferElem = (tSndSoundBufferElem*)-1;
        pcSND_l_Command(PCSND_CMD_EXIT, "exit");
        if (!SPL_b_ThreadWait(pcSNDhObject, 5000))
            DBG_OutputDebugString("SND: the sound thread did not exit in five seconds\n");
        sndCurrSoundBufferElem = NULL;
        pcSNDhObject = 0;
        pcSNDThreadId = 0;
        if (sndEventHandles)
            MEM_Free(sndEventHandles);
        sndEventHandles = NULL;
        if (sndPlayList)
            MEM_Free(sndPlayList);
        sndPlayList = NULL;
        sndEventHandleNum = 0;
        sndEventsNum = 0;
        trackEventsArrayNextIdx = 0;
        /* Tokens, with nothing behind them to close. */
        for (i = 0; i < 23; ++i)
            dsndEvents[i] = 0;
        if (dsndReadBuffer1)
            free(dsndReadBuffer1);
        dsndReadBuffer1 = NULL;
        if (dsndReadBuffer2)
            free(dsndReadBuffer2);
        dsndReadBuffer2 = NULL;
    }
}

void pcSND_Close(tSndHandles* handles)
{
    pcSND_Cleanup();
    BAS_bfree(&dsndSBList);
    handles->deviceOpen = 0;
    SPL_Close();
}

void pcSND_SB_Duplicate(tSndHandles* handles, tSndSoundBufferElem* src, tSndSoundBufferElem** dest)
{
    tSndSoundBufferElem* copy;

    *dest = NULL;
    if (!src->miscData)
    {
        copy = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
        *dest = copy;
        if (copy)
        {
            memset(copy, 0, sizeof(tSndSoundBufferElem));
            (*dest)->voice = SPL_p_VoiceDuplicate(src->voice);
            (*dest)->format = src->format;
            (*dest)->unk2SetTo64InCreate = 64;
            pcSND_SetDefaultSoundPositionAndDistance(*dest);
        }
    }
}

void pcSND_SB_GetFrequency(tSndSoundBufferElem* sbe, int* freq)
{
    if (sbe)
        (*(freq) = SPL_i_VoiceFrequency(sbe->voice));
}

void pcSND_SB_GetPan(tSndSoundBufferElem* sbe, int* pan, int* front)
{
    int balance;
    float level;

    if (sbe)
    {
        *front = 0;
        *pan = 0;
        if (SND_gst_Params.soundRenderMode == 2)
        {
            balance = sbe->unk2SetTo64InCreate;
            if (balance < 64)
            {
                level = (float)((double)balance * 0.015625);
                *pan = SND_l_GetAttFromVol(level);
            }
            else if (balance > 64)
            {
                level = (float)((127.0 - (double)balance) * 0.015873017);
                *pan = SND_l_GetAttFromVol(level);
            }
            else
            {
                *pan = 0;
            }
        }
    }
}

/* Where a sound sits between the two speakers.
   
   This used to put the 3D buffer on a unit circle: the pan became an
   angle, hard right at 0 degrees through straight ahead at 90 to hard
   left at 180, and the position followed from it. The circle was the
   only use the engine ever made of 3D, and the pan is what it was
   expressing -- so the pan is what is set now, in the same hundredths
   of a decibel and with the same sign. What is lost is the equal-power
   curve of going round a circle; what is kept is which speaker the
   sound comes out of and how much. */
int pcSND_SoundBufferSetPan(tSndSoundBufferElem* sbe, int pan)
{
    if (sbe)
        SPL_VoiceSetPan(sbe->voice, pan);
    return 0;
}

void pcSND_SB_SetPan(tSndSoundBufferElem* sbe, int pan, int front) // ehhez nagyon hasonló ez: pcSND_UnknownSoundPanAndVolumeSetByUnknown
{
    int balance;
    int stripped;
    int vol;
    unsigned int scaled;
    //int v6a;
    int level;
    int side;

    if (!sbe)
        return;
    (vol = SPL_i_VoiceAttenuation(sbe->voice));
    balance = sbe->unk2SetTo64InCreate;
    if (balance <= 64)
        stripped = vol - gpcSNDVolumeAdjustmentArr[balance];
    else
        stripped = vol - gpcSNDVolumeAdjustmentArr[127 - balance];
    if (pan == 0)
    {
        sbe->unk2SetTo64InCreate = 64;
        level = gpcSNDVolumeAdjustmentArr[64];
        side = 0;
    }
    else if (pan < 0)
    {
        scaled = (unsigned int)(SND_f_GetVolFromAtt(pan) * 64.0);
        if ((scaled & 0x80000000) == 0)
        {
            if (scaled > 127)
                scaled = 127;
        }
        else
        {
            scaled = 0;
        }
        sbe->unk2SetTo64InCreate = scaled;
        level = gpcSNDVolumeAdjustmentArr[scaled];
        side = gpcSNDVolumeAdjustmentArr[127 - scaled] - level;
    }
    else
    {
        scaled = (unsigned int)(127.0 - SND_f_GetVolFromAtt(-pan) * 63.0);
        if ((scaled & 0x80000000) == 0)
        {
            if (scaled > 127)
                scaled = 127;
        }
        else
        {
            scaled = 0;
        }
        sbe->unk2SetTo64InCreate = scaled;
        // TODO FIXME!!!!!!!!!
        level = gpcSNDVolumeAdjustmentArr[127 - scaled]; // v6a = gpcSNDVolumeAdjustmentArr[127 - v6]
        side = level - gpcSNDVolumeAdjustmentArr[scaled]; //v8 = v6a - gpcSNDVolumeAdjustmentArr[v6];
    }
    vol = stripped + level;
    if (SND_gst_Params.soundRenderMode == 1)
    {
        SPL_VoiceSetPan(sbe->voice, 0);
    }
    else
    {
        if (vol > 0)
            vol = 0;
        else if (vol < -10000)
            vol = -10000;
        if (side > 10000)
            side = 10000;
        else if (side < -10000)
            side = -10000;
        pcSND_SoundBufferSetPan(sbe, side);
        if (sbe->unknownFiledinsub49db40ToSetVolume != vol)
            SPL_VoiceSetAttenuation(sbe->voice, vol);
    }
}

void pcSND_SB_SetCurrPos(tSndSoundBufferElem* sbe, int pos)
{
    if (sbe)
        SPL_VoiceSetCursor(sbe->voice, pos);
}

void pcSND_SB_SetVolume(tSndSoundBufferElem* sbe, int att)
{
    int base;
    int balance;
    int vol;

    if (!sbe)
        return;
    base = gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 + att;
    balance = sbe->unk2SetTo64InCreate;
    if (SND_gst_Params.soundRenderMode != 2)
        vol = gpcSNDVolumeAdjustmentArr[64] + base;
    else if (balance > 64)
        vol = gpcSNDVolumeAdjustmentArr[127 - balance] + base;
    else
        vol = gpcSNDVolumeAdjustmentArr[balance] + base;
    if (vol > 0)
        vol = 0;
    else if (vol < -10000)
        vol = -10000;
    if (sbe->unknownFiledinsub49db40ToSetVolume != vol)
    {
        sbe->unknownFiledinsub49db40ToSetVolume = vol;
        SPL_VoiceSetAttenuation(sbe->voice, vol);
    }
}

void pcSND_SNDRasterRelease(struct sSplVoice* buffer)
{
    int fileId;
    int bytes;

    if (SND_gst_Params.soundEnabled)
    {
        BAS_bdeletegetval((int)buffer, &rasterbas2, &fileId);
        BAS_bdeletegetval((int)buffer, &rasterbas1, &bytes);
        if (fileId != -1 && bytes != -1)
        {
            if (fileId == 0x2000 || fileId == 0x4000 || fileId == 0x80000)
                sndRasterStat.trackBytes -= bytes;
            else if (fileId == 8)
                sndRasterStat.loadingSoundBytes -= bytes;
            else
                sndRasterStat.soundBytes -= bytes;
            sndRasterStat.totalBytes -= bytes;
            sndRasterStat.budgetLeft = pcSND_RasterBudgetLeft();
        }
    }
}

void pcSND_MiscDataRelease(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;

    if (sbe)
    {
        misc = sbe->miscData;
        if (misc)
        {
            pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(sbe);
            if (misc->fileHandle != SPL_FILE_NONE)
                SPL_FileClose(misc->fileHandle);
            misc->fileHandle = SPL_FILE_NONE;
            /* The notify interface the refill positions were set on
               used to be released here. There is none. */
            misc->vmilenEventHandlptr_Sub49e950 = 0;
            if (sbe->voice)
            {
                pcSND_SNDRasterRelease(sbe->voice);
                SPL_VoiceDestroy(sbe->voice);
            }
            sbe->voice = 0;
            MEM_Free(sbe->miscData);
            sbe->miscData = 0;
        }
    }
}

void pcSND_ReleaseSoundBuffer(tSndSoundBufferElem* sbe)
{
    int wasListed;

    if (sbe)
    {
        BAS_bdeletegetval((int)sbe, &dsndSBList, &wasListed);
        if (sbe->miscData)
        {
            if (wasListed)
                gDoSoundGlobal.dword_B303C0 -= sbe->format;
            pcSND_MiscDataRelease(sbe);
            MEM_Free(sbe);
        }
        else
        {
            if (wasListed)
                gDoSoundGlobal.sndConcurrentlyPlayedNum -= sbe->format;
            /*
            // TODO FIXME: ez lehet megint valami bent felejtett kód, mivel 0x54 nincs is a tSndSoundBufferElem struktúrában, és  gap1, gap sem állítódik
            if (*(_DWORD*)v1->gap1)
                ((void(__stdcall*)(dsndSoundBuffer**, _DWORD, _DWORD, _DWORD))(*v1->voice)[1].QueryInterface)(
                    v1->voice,
                    0,
                    0,
                    0);
            if (*(_DWORD*)v1->gap)
                MEM_Free(*(void**)v1->gap);
            */
            pcSND_SNDRasterRelease(sbe->voice);
            SPL_VoiceDestroy(sbe->voice);
            MEM_Free(sbe);
        }
    }
}

int pcSND_Return160()
{
    return 160;
}

tSndSoundBufferElem* pcSND_SoundReloadToDSound(tSndHandles* handles, tSndWavHdrStruct* wavHdr, int offset)
{
    char* sndData;
    char* cursor;
    char* uncompressedSnd;
    unsigned int uncompressedSize;
    int packedSize;
    unsigned int decodedSize;
    unsigned int blockAlign;
    DWORD samplesPerSec;
    tSndSoundBufferElem* sndSNDBuf;

    if (!handles->deviceOpen)
        return 0;
    if (wavHdr->wfex.wFormatTag != 2)
        return 0;
    sndData = 0;
    if (offset)
    {
        sndData = (char*)MEM_p_Alloc(wavHdr->fullSoundSize);
        if (!sndData)
            return 0;
        if (!LOA_gb_SpeedMode || !isSpeedModeAndBinFile())
        {
            BIG_ReadNoSeek(offset, sndData, wavHdr->fullSoundSize);
        }
        else if (wavHdr->wfex.nChannels == 1)
        {
            cursor = LOA_FetchBuffer(wavHdr->fullSoundSize);
            LOA_ReadCharArray(&cursor, sndData, wavHdr->fullSoundSize, 1);
        }
        else
        {
            packedSize = wavHdr->fullSoundSize;
            wavHdr->wfex.cbSize >>= 1;
            wavHdr->fullSoundSize = packedSize >> 1;
            cursor = LOA_FetchBuffer(packedSize >> 1);
            LOA_ReadCharArray(&cursor, sndData, wavHdr->fullSoundSize, 1);
            cursor = LOA_FetchBuffer(wavHdr->fullSoundSize);
            LOA_ReadCharArray(&cursor, &sndData[wavHdr->fullSoundSize], wavHdr->fullSoundSize, 1);
        }
    }
    if (wavHdr->wfex.wFormatTag != 2)
    {
        if (sndData)
            MEM_Free(sndData);
        return 0;
    }
    if (offset)                                 // itt mvi fv van ami nem látszik sub_4A0590, 
// kioptimalizálja az ida, mert vna egy vizsgálat != 2-re ami return 0-val zárul ellenkezõ esetben ellenõrzõdik 
// hogy == 0x69-el ami nyilván nem teljesül.
//  Az a kódrész akkor futna le.
    {
        uncompressedSize = pcSND_calcMemoryNeededForTheSound(wavHdr->fullSoundSize, wavHdr->wfex.nChannels);
        uncompressedSnd = (char*)MEM_p_Alloc(uncompressedSize);
        memset(uncompressedSnd, 0, uncompressedSize);
        pcSND_DecodeADPCMBlocks(sndData, (int16_t*)uncompressedSnd, wavHdr->fullSoundSize / (int)wavHdr->wfex.nBlockAlign, wavHdr->wfex.nChannels);
        MEM_Free(sndData);
        sndData = uncompressedSnd;
    }
    decodedSize = pcSND_calcMemoryNeededForTheSound(wavHdr->fullSoundSize, wavHdr->wfex.nChannels);
    samplesPerSec = wavHdr->wfex.nSamplesPerSec;
    wavHdr->fullSoundSize = decodedSize;
    blockAlign = 2 * wavHdr->wfex.nChannels;
    wavHdr->wfex.nBlockAlign = blockAlign;
    wavHdr->wfex.wBitsPerSample = 16;
    wavHdr->wfex.wFormatTag = 1;
    wavHdr->wfex.cbSize = 0;
    wavHdr->wfex.nAvgBytesPerSec = (uint16_t)blockAlign * samplesPerSec;
    sndSNDBuf = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(sndSNDBuf, 0, sizeof(tSndSoundBufferElem));
    sndSNDBuf->unk2SetTo64InCreate = 64;
    sndSNDBuf->voice = SPL_p_VoiceCreate(wavHdr->wfex.nChannels,
                                         wavHdr->wfex.nSamplesPerSec,
                                         wavHdr->fullSoundSize);
    if (!sndSNDBuf->voice)
    {
        if (sndData)
            MEM_Free(sndData);
        MEM_Free(sndSNDBuf);
        return 0;
    }
    sndSNDBuf->format = wavHdr->wfex.nChannels;
    pcSND_AddToSNDRaster(sndSNDBuf->voice, wavHdr->fullSoundSize);
    if (!sndData)
        return sndSNDBuf;
    if (!SPL_b_VoiceWrite(sndSNDBuf->voice, 0, sndData, wavHdr->fullSoundSize))
    {
        SPL_VoiceDestroy(sndSNDBuf->voice);
        MEM_Free(sndSNDBuf);
        MEM_Free(sndData);
        return NULL;
    }
    MEM_Free(sndData);
    return sndSNDBuf;
}

tSndSoundBufferElem* pcSND_SB_Reload(tSndHandles* handles, tSndSoundBufferElem* sndBufferElem, tSndWavHdrStruct* wavHdr, int offset)
{
    tSndSoundBufferElem* created; // esi

    created = pcSND_SoundReloadToDSound(handles, wavHdr, offset);
    if (created)
        pcSND_ReleaseSoundBuffer(sndBufferElem);
    return created;
}

void pcSND_StreamPrefetchArray(int* positions, int* sizes) //// @SND_StreamResolveSyncPrefetch
{
    if (positions)
        memset(positions, 0, 32u);
}

void pcSND_setCurrentSoundBuffElemAndSetAnEvent(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;

    misc = sbe->miscData;
    if ((misc->eventHandle1 == (HANDLE)-1 || !misc->eventHandle1) && (misc->eventHandle2 == (HANDLE)-1 || !misc->eventHandle2))
    {
        misc->eventHandle1 = dsndEvents[misc->dsndEventsArrayIdx + 6];
        sbe->miscData->eventHandle2 = dsndEvents[sbe->miscData->dsndEventsArrayIdx + 15];
        sndCurrSoundBufferElem = sbe;
        pcSND_l_Command(PCSND_CMD_ADD, "add");
    }
}

/* Five notification positions used to be set here: four at the block
   boundaries of the buffer, all firing the refill event, and one at
   the stop, firing the other. Nothing says when a cursor reaches a
   position now, so there is nothing to set -- the thread looks
   instead, and what it compares against is the block the cursor is
   in, which starts as the one the first fill left it in. */
void pcSND_SetPositionNotify(tSndSoundBufferElem* sbe)
{
    pcSND_setCurrentSoundBuffElemAndSetAnEvent(sbe);
    sbe->miscData->lastPlayedBlock = pcSND_l_PlayedBlock(sbe->miscData);
#if PCSND_TRACE
    /* Where a stream's refilling begins, and the sizes it begins with.
       A block far larger than a second of sound is a stream the cursor
       cannot cross a boundary of in any reasonable time, which looks
       from outside exactly like a refill that never comes. */
    DBG_OutputDebugString("SND start: sbe=%p misc=%p voice=%p up=%d buf=%d full=%u blk=%d h1=%p h2=%p\n",
                          (void*)sbe, (void*)sbe->miscData, (void*)sbe->voice,
                          sbe->miscData->sizeToUpload, sbe->miscData->soundBufferSize,
                          sbe->miscData->fullSize, sbe->miscData->lastPlayedBlock,
                          (void*)sbe->miscData->eventHandle1,
                          (void*)sbe->miscData->eventHandle2);
#endif
}

void pcSND_fillSoundBuffer(int unused, tSbeMiscData* miscArg, tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    int skipSize;
    int produced;
    int decoded;
    int audioBytes1;
    char* audioPtr1;

    misc = sbe->miscData;
    if (misc)
    {
        audioBytes1 = misc->soundBufferSize;
        audioPtr1 = pcSND_l_p_UploadBuffer(audioBytes1);
        if (!audioPtr1)
            return;
        skipSize = pcSND_CalcSizeToReadByFormat(misc->startDecodedOffset, sbe->format);
        SPL_b_FileSeek(misc->fileHandle, PCSND_FILE_OFFSET(misc->distanceToMove + skipSize));
        misc->sumSize = misc->startDecodedOffset;
        pcSND_ReadAndDecompressSoundData(audioBytes1, sbe, 0, &decoded, (int16_t*)audioPtr1);
        produced = decoded;
        if (decoded < audioBytes1)
        {
            if (misc->readThroughWithMultipleLoop)
            {
                do
                {
                    SPL_b_FileSeek(misc->fileHandle, PCSND_FILE_OFFSET(misc->distanceToMove));
                    misc->sumSize = 0;
                    pcSND_ReadAndDecompressSoundData(audioBytes1 - produced, sbe, 0, &decoded, (int16_t*)(&audioPtr1[produced]));
                    produced += decoded;
                } while (produced < audioBytes1);
            }
            else
            {
                if (decoded < misc->sizeToUpload)
                {
                    misc->RemoveAfterPlayFinished = 1;
                    produced = decoded;
                }
                memset(&audioPtr1[produced], 0, audioBytes1 - produced);
            }
        }
        SPL_b_VoiceWrite(sbe->voice, 0, audioPtr1, (int)audioBytes1);
        misc->currOffset += audioBytes1;
        if (misc->currOffset >= misc->soundBufferSize)
            misc->currOffset -= misc->soundBufferSize;
        misc->currSize = misc->startDecodedOffset;
        misc->currentPlayPosition = 0;
    }
}

void pcSND_SB_SetFrequency(tSndSoundBufferElem* sbe, int freq)
{
    if (sbe)
    {
        if (freq > 200000)
            freq = 200000;
        else if (freq < 100)
            freq = 100;
        SPL_VoiceSetFrequency(sbe->voice, freq);
    }
}

void pcSND_UnknownSoundPanAndVolumeSetByUnknown(tSndSoundBufferElem* sbe, int pan)
{
    int balance;
    int stripped;
    int vol;
    unsigned int scaled;
    //unsigned int v6b;
    int level;
    LONG side;

    if (!sbe)
        return;
    (vol = SPL_i_VoiceAttenuation(sbe->voice));
    balance = sbe->unk2SetTo64InCreate;
    if (balance <= 64)
        stripped = vol - gpcSNDVolumeAdjustmentArr[balance];
    else
        stripped = vol - gpcSNDVolumeAdjustmentArr[-balance + 127];
    if (pan == 0)
    {
        sbe->unk2SetTo64InCreate = 64;
        level = gpcSNDVolumeAdjustmentArr[64];
        side = 0;
    }
    else if (pan < 0)
    {
        scaled = (unsigned int)(SND_f_GetVolFromAtt(pan) * 64.0);
        if ((scaled & 0x80000000) == 0)
        {
            if (scaled > 127)
                scaled = 127;
        }
        else
        {
            scaled = 0;
        }
        sbe->unk2SetTo64InCreate = scaled;
        level = gpcSNDVolumeAdjustmentArr[scaled];
        side = gpcSNDVolumeAdjustmentArr[127 - scaled] - level;
    }
    else
    {
        scaled = (unsigned int)(127.0 - SND_f_GetVolFromAtt(-pan) * 63.0);
        if ((scaled & 0x80000000) == 0)
        {
            if (scaled > 127)
                scaled = 127;
        }
        else
        {
            scaled = 0;
        }
        sbe->unk2SetTo64InCreate = scaled;
        // TODO FIXME!!!!!!!!!
        level = gpcSNDVolumeAdjustmentArr[127 - scaled]; // v6b = gpcSNDVolumeAdjustmentArr[127 - v6a]
        side = level - gpcSNDVolumeAdjustmentArr[scaled]; //v8 = v6b - gpcSNDVolumeAdjustmentArr[v6a];
    }
    vol = stripped + level;
    if (SND_gst_Params.soundRenderMode == 1)
    {
        SPL_VoiceSetPan(sbe->voice, 0);
    }
    else
    {
        if (vol > 0)
            vol = 0;
        else if (vol < -10000)
            vol = -10000;
        if (side > 10000)
            side = 10000;
        else if (side < -10000)
            side = -10000;
        SPL_VoiceSetPan(sbe->voice, side);
        if (sbe->unknownFiledinsub49db40ToSetVolume != vol)
            SPL_VoiceSetAttenuation(sbe->voice, vol);
    }
}

void pcSND_StreamReinitAndPlay(tSndSoundBufferElem* sbe, int len, int startPos, int from, int to, char flags, int freq, int pan, int att)
{
    tSbeMiscData* misc;
    int readSize;
    int startSize;
    int channels;
    int streaming;
    int fullSize;

    if (sbe)
    {
        misc = sbe->miscData;
        fullSize = pcSND_calcMemoryNeededForTheSound(len, sbe->format);
        pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(sbe);
        readSize = pcSND_CalcSizeToReadByFormat(from, sbe->format);
        /* The start position is rounded down to a whole compressed
           block, so that the seek below lands on a block boundary. The
           block is 36 bytes per channel -- a fixed property of the
           format, not of any struct, which is what it used to be taken
           from. */
        startSize = pcSND_calcMemoryNeededForTheSound(SND_ADPCM_BLOCK_BYTES * sbe->format * (readSize / (SND_ADPCM_BLOCK_BYTES * sbe->format)), sbe->format);
        if (startSize < 0 || startSize > fullSize)
            startSize = 0;
        misc->RemoveAfterPlayFinished = 0;
        misc->shouldBeRemovedFromPlayList = 0;
        misc->Stopped_SetIN49e270 = 0;
        misc->readThroughWithMultipleLoop = flags & 1;
        misc->currOffset = 0;
        misc->currentPlayPosition = 0;
        misc->currSize = 0;
        misc->startDecodedOffset = startSize;
        misc->fullSize = fullSize;
        misc->distanceToMove = startPos;
        misc->sumSize = 0;
        channels = sbe->format;
        streaming = gDoSoundGlobal.dword_B303C0 + channels;
        if (streaming <= 4)
        {
            gDoSoundGlobal.dword_B303C0 += channels;
            if (streaming > gDoSoundGlobal.dword_B303C4)
                gDoSoundGlobal.dword_B303C4 = streaming;
            BAS_binsert((unsigned int)sbe, (int)sbe, &dsndSBList);
            pcSND_fillSoundBuffer(0, misc, sbe);
            pcSND_SB_SetFrequency(sbe, freq);
            pcSND_UnknownSoundPanAndVolumeSetByUnknown(sbe, pan);
            pcSND_SB_SetVolume(sbe, att);
            pcSND_SetPositionNotify(sbe);
            SPL_VoiceSetCursor(sbe->voice, 0);
            (SPL_VoicePlay(sbe->voice, 1), 0);
        }
    }
}

tSndSoundBufferElem* pcSND_CreateTrack(tSndHandles* handles, tSndWavHdrStruct* wavHdr)
{
    tSndSoundBufferElem* track;
    tSbeMiscData* misc;

    wavHdr->wfex.wFormatTag = 1;
    if (!handles->deviceOpen)
        return 0;
    track = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(track, 0, sizeof(tSndSoundBufferElem));
    misc = (tSbeMiscData*)MEM_p_Alloc(sizeof(tSbeMiscData));
    memset(misc, 0, sizeof(tSbeMiscData));
    misc->sizeToUpload = 307200 / wavHdr->wfex.nBlockAlign / 4;
    misc->sizeToUpload *= wavHdr->wfex.nBlockAlign;
    misc->soundBufferSize = 4 * misc->sizeToUpload;

    // preserved: both allocations leak when the call fails
    track->voice = SPL_p_VoiceCreate(wavHdr->wfex.nChannels,
                                     wavHdr->wfex.nSamplesPerSec,
                                     misc->soundBufferSize);
    if (!track->voice)
        return 0;
    pcSND_AddToSNDRaster(track->voice, misc->soundBufferSize);
    track->format = wavHdr->wfex.nChannels;
    track->unk2SetTo64InCreate = 64;
    track->miscData = misc;
    misc->eventHandle1 = (HANDLE)-1;
    misc->eventHandle2 = (HANDLE)-1;
    misc->dsndEventsArrayIdx = trackEventsArrayNextIdx;
    trackEventsArrayNextIdx = misc->dsndEventsArrayIdx + 1;
    /* Read-only, shared, and it must already exist: the flags the
       engine asked Windows for were GENERIC_READ, share read and
       write, OPEN_EXISTING, with the sequential-scan and
       random-access hints, which is a hint and not a mode. */
    misc->fileHandle = SPL_p_FileOpen(BIG_gst.filePath);
    return track;
}

void pcSND_CreateTrackSB(tSndSoundBufferElem** oneChannelTrack, tSndSoundBufferElem** twoChannelTrack)
{
    tSndSoundBufferElem* mono;
    tSndSoundBufferElem* stereo;
    tSndWavHdrStruct wavHdr;

    wavHdr.fullSoundSize = 0;
    wavHdr.dataStartPos = 0;
    wavHdr.wfex.cbSize = 0;
    wavHdr.wfex.nChannels = 1;
    wavHdr.wfex.wFormatTag = 1;
    wavHdr.wfex.wBitsPerSample = 16;
    wavHdr.wfex.nSamplesPerSec = 48000;
    wavHdr.wfex.nBlockAlign = 2;
    wavHdr.wfex.nAvgBytesPerSec = 96000;
    mono = pcSND_CreateTrack(SND_gst_Params.handles, &wavHdr);
    wavHdr.wfex.nBlockAlign *= 2;
    *oneChannelTrack = mono;
    wavHdr.wfex.nAvgBytesPerSec *= 2;
    wavHdr.wfex.nChannels = 2;
    stereo = pcSND_CreateTrack(SND_gst_Params.handles, &wavHdr);
    *twoChannelTrack = stereo;
    pcSND_SNDRasterRelease(stereo->voice);
}

void pcSND_ReleaseTrackSB(tSndSoundBufferElem* oneChannelTrack, tSndSoundBufferElem* twoChannelTrack)
{
    pcSND_ReleaseSoundBuffer(oneChannelTrack);
    pcSND_ReleaseSoundBuffer(twoChannelTrack);
}

void pcSND_ShutDownStreams()
{
    int i;
    tSoundInstanceElem* inst;
    int flags;

    for (i = 0; i < SND_gst_Params.soundInstanceNum; ++i)
    {
        inst = &SND_gst_Params.soundInstances[i];
        flags = inst->flags;
        if ((flags & 0x400) != 0 && (flags & 0x86000) != 0)
        {
            if (SND_i_IsPlaying(i))
                inst->soundIntf->Stop(inst->soundBufElemFromModel);
        }
    }
}

int pcSND_l_GetGlobalVol()
{
    return gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0;
}

void pcSND_SetGlobalVol(int att)
{
    int clamped;
    int i;

    clamped = att;
    if (gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 != att)
    {
        if (att < -10000)
            clamped = -10000;
        else if (att > 0)
            clamped = 0;
        gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 = clamped;
        for (i = 0; i < SND_gst_Params.soundInstanceNum; ++i)
        {
            if ((SND_gst_Params.soundInstances[i].flags & 0x400) != 0)
                SND_SetInstVolume(&SND_gst_Params.soundInstances[i]);
        }
    }
}

void SND_ComputeDoppler(Vec3* listenerSpeed, Vec3* listenerPos, tSoundInstanceElem* sie)
{
    tGameObjectMain* gao;
    tGameObjectExtended* extended;
    double gaoLen;
    tDyn* dyn;
    double dx;
    double dy;
    double invDt;
    double dz;
    long double posLen;
    double closing;
    double ratio;
    tSndSoundBufferElem* buffer;
    tSndInterfaceStruct* intf;
    double freqRounded;
    Vec3 toSource;
    Vec3 sourceSpeed;
    float distLen;
    float invDtF;

    if (!sie->sndPosMtxSetInSetPos)
    {
        gao = sie->gao;
        if (gao)
        {
            if ((gao->flags & 0x8000000) == 0)
                return;
            extended = gao->extended;
            if (!extended || !extended->snd)
                return;
            sie->sndPosMtxSetInSetPos = gao->matrices;
        }
    }
    if (sie->sndPosMtxSetInSetPos)
    {
        gao = sie->gao;
        if (!gao)
            return;
        if ((gao->flags & 0x8000000) == 0)
            return;
        extended = gao->extended;
        if (!extended)
            return;
        if (!extended->snd)
            return;
        MATH_SubstractVec(&toSource, &gao->matrices->Pos, listenerPos);
        gaoLen = MATH_VecLen(&toSource);
        distLen = (float)gaoLen;
        if (gaoLen < 0.0000099999997)
            return;
        if (OBJ_pst_GetDynaStruct(gao))
        {
            dyn = OBJ_pst_GetDynaStruct(gao);
            DYN_SpeedGetVector(dyn, &sourceSpeed);
        }
        else
        {
            sourceSpeed.x = 0.0;
            sourceSpeed.y = 0.0;
            sourceSpeed.z = 0.0;
        }
    }
    else
    {
        if (sie->sndPositionSetInSetPosPtr != &sie->sndInstPosVec)
            return;

        dx = sie->sndInstPosVec.x - sie->sndInstPosVecPrevious.x;
        dy = sie->sndInstPosVec.y - sie->sndInstPosVecPrevious.y;
        sourceSpeed.z = sie->sndInstPosVec.z - sie->sndInstPosVecPrevious.z;
        invDt = 1.0 / TIM_gf_dt;
        sie->sndInstPosVecPrevious.x = sie->sndInstPosVec.x;
        sie->sndInstPosVecPrevious.y = sie->sndInstPosVec.y;
        sie->sndInstPosVecPrevious.z = sie->sndInstPosVec.z;
        invDtF = (float)invDt;
        sourceSpeed.x = (float)(dx * invDtF);
        sourceSpeed.y = (float)(dy * invDtF);
        sourceSpeed.z = sourceSpeed.z * invDtF;
        toSource.x = sie->sndInstPosVec.x - listenerPos->x;
        toSource.y = sie->sndInstPosVec.y - listenerPos->y;
        dz = sie->sndInstPosVec.z - listenerPos->z;
        toSource.z = (float)dz;
        posLen = sqrt(dz * toSource.z + toSource.y * toSource.y + toSource.x * toSource.x);
        distLen = (float)posLen;
        if (posLen < 0.0000099999997)
            return;
    }
    closing = SND_gst_Params.fx_DopplerSpeed * distLen
              - (toSource.y * listenerSpeed->y + toSource.x * listenerSpeed->x + toSource.z * listenerSpeed->z);
    if (closing != 0.0f)
    {
        ratio = ((listenerSpeed->z - sourceSpeed.z) * toSource.z
                 + (listenerSpeed->y - sourceSpeed.y) * toSource.y
                 + (listenerSpeed->x - sourceSpeed.x) * toSource.x) / closing * SND_gst_Params.fx_doppler;
        if (ratio >= -1.0)
        {
            if (ratio > 1000.0)
                ratio = 1000.0;
        }
        else
        {
            ratio = -0.99900001;
        }
        buffer = sie->soundBufElemFromModel;
        intf = sie->soundIntf;
        freqRounded = (double)sie->freq * (ratio + 1.0) + MATH_gd_magicDoubleForRound;
        //*(double*)&dest.x = v21;
        intf->SetFrequency(buffer, (*(int32_t *)&freqRounded));
    }
}

float SND_f_Compute3DVol(tSoundInstanceElem* inst) // @SND_f_GetInstVolume
{
    int flags;
    int playRequested;
    int i;
    Mtx4x4* mtx;
    Vec3* sourcePos;
    double dz;
    int pan;
    int front;
    double side;
    double sideLow;
    double depth;
    double depth2;
    double depthLow;
    float axisFactor[3];
    int masks[3];
    Vec3 dir;
    float vol;
    float factor;
    float t;
    int stopPlayFlags;

    if (!SND_gst_Params.sndMatrixRef)
        return inst->volume;
    if (!inst->sndPositionSetInSetPosPtr)
        return inst->volume;
    flags = inst->flags;
    if ((flags & 0x1F000000) == 0)
        return inst->volume;
    if ((flags & 0x10000000) != 0)
    {
        factor = SND_f_SphericComputing(&SND_gst_Params.sndMatrixRef->Pos, inst, &stopPlayFlags);
        if ((stopPlayFlags & 1) != 0)
            SND_AutoStop(inst);
        playRequested = (stopPlayFlags & 2) != 0;
    }
    else
    {
        masks[0] = 0x1000000;
        masks[1] = 0x2000000;
        masks[2] = 0x4000000;
        for (i = 0; i < 3; ++i)
        {
            if ((masks[i] & inst->flags) != 0)
            {
                axisFactor[i] = SND_f_AxisComputing(&SND_gst_Params.sndMatrixRef->Pos, inst, &masks[i], i);
            }
            else
            {
                axisFactor[i] = 1.0;
                masks[i] = 2;
            }
        }
        factor = axisFactor[2] * axisFactor[1] * axisFactor[0];
        if (((masks[0] | masks[1] | masks[2]) & 1) != 0)
            SND_AutoStop(inst);
        playRequested = (masks[0] & masks[1] & masks[2] & 2) != 0;
    }
    if (playRequested)
        SND_AutoPlay(inst);
    vol = factor * inst->volume;
    if ((inst->flags & 0x8000000) == 0)
        return vol;
    pan = 0;
    front = 0;
    mtx = SND_gst_Params.microPosMtxPtr;
    if (mtx)
    {
        sourcePos = inst->sndPositionSetInSetPosPtr;
        dir.x = sourcePos->x - SND_gst_Params.microPosMtxPtr->Pos.x;
        dir.y = sourcePos->y - SND_gst_Params.microPosMtxPtr->Pos.y;
        dz = sourcePos->z - SND_gst_Params.microPosMtxPtr->Pos.z;
        dir.z = (float)dz;
        if (sqrt(dz * dir.z + dir.y * dir.y + dir.x * dir.x) != 0.0)
        {
            MATH_NormalizeVector(&dir, &dir);
            side = (mtx->v1.z * dir.z + mtx->v1.y * dir.y + dir.x * mtx->v1.x) * SND_gst_Params.f_surround;
            if (side > 1.0)
                side = 1.0;
            else if (!(side >= -1.0))
                side = -1.0;
            if (side > 0.0)
            {
                t = (float)(1.0 - side + inst->minPan);
                if (t >= 0.0000001)
                {
                    if (t > 1.0)
                        t = 1.0;
                }
                else
                {
                    t = 0.0;
                }
                pan = -SND_l_GetAttFromVol(t);
            }
            else if (side >= 0.0)
            {
                pan = 0;
            }
            else
            {
                sideLow = side + inst->minPan + 1.0;
                t = (float)sideLow;
                if (sideLow >= 0.0000001)
                {
                    if (t > 1.0)
                        t = 1.0;
                    pan = SND_l_GetAttFromVol(t);
                }
                else
                {
                    pan = SND_l_GetAttFromVol(0.0);
                }
            }
            if (SND_gst_Params.soundRenderMode == 8 || SND_gst_Params.soundRenderMode == 16 || SND_gst_Params.soundRenderMode == 32)
            {
                depth = mtx->v3.z * dir.z + mtx->v3.y * dir.y + dir.x * mtx->v3.x;
                depth2 = depth + depth;
                if (depth2 <= 0.0)
                {
                    if (depth2 >= 0.0)
                    {
                        front = 0;
                    }
                    else
                    {
                        depthLow = depth2 + inst->minPan + 1.0;
                        t = (float)depthLow;
                        if (depthLow >= 0.0000001)
                        {
                            if (t > 1.0)
                                t = 1.0;
                            front = SND_l_GetAttFromVol(t);
                        }
                        else
                        {
                            front = SND_l_GetAttFromVol(0.0);
                        }
                    }
                }
                else
                {
                    t = (float)(1.0 - depth2 + inst->minPan);
                    if (t >= 0.0000001)
                    {
                        if (t > 1.0)
                            t = 1.0;
                    }
                    else
                    {
                        t = 0.0;
                    }
                    front = -SND_l_GetAttFromVol(t);
                }
            }
        }
    }
    inst->soundIntf->SetPan(inst->soundBufElemFromModel, pan, front);
    inst->pan = pan;
    inst->front = front;
    return vol;
}

int pcSND_i_FxInit()
{

    SND_gst_FxParam.voiceListCount = 16;
    SND_gst_FxDefaultParam.voiceListCount = 16;
    SND_gst_FxDefaultParam.voiceList = (tSndSoundBufferElem**)MEM_p_Alloc(64u);
    SND_gst_FxParam.voiceList = (tSndSoundBufferElem**)MEM_p_Alloc(4 * SND_gst_FxParam.voiceListCount);
    memset(SND_gst_FxParam.voiceList, 0, 4 * SND_gst_FxParam.voiceListCount);
    fxWetVol = 0.0;
    memset(&gFXGlobal, 0, sizeof(gFXGlobal));
    /* Whether the effects are on came from a registry value the
       installer wrote. They are off: the effects were EAX's, through
       a property set that does not exist, and this is the answer the
       engine already had on a card without it. */
    gFXGlobal.fxEnabled = 0;
    return 0;
}

void pcSND_FxClose()
{
    if (SND_gst_FxParam.voiceList)
        MEM_Free(SND_gst_FxParam.voiceList);
    SND_gst_FxParam.voiceList = 0;
    if (SND_gst_FxDefaultParam.voiceList)
        MEM_Free(SND_gst_FxDefaultParam.voiceList);
    SND_gst_FxDefaultParam.voiceList = 0;
}

void pcSND_fxStructSet(tSndRoomProperties* dest, int preset)
{
    tSndRoomProperties props;

    if (preset < 0 || preset > 11)
        return;

    props.flEnvironmentSize = 7.5f;
    props.flReverbDelay = 0.011f;
    props.flReflectionsDelay = 0.0070000002f;
    props.flDecayTime = 1.49f;
    props.lRoom = -1000;
    props.flEchoDepth = 0.0;
    props.ulFlags = 63;
    props.flDecayLFRatio = 1.0;
    props.flHFReference = 5000.0f;
    props.flLFReference = 250.0f;
    props.flModulationDepth = 0.0;
    props.flModulationTime = 0.25f;
    props.flEchoTime = 0.25f;
    props.flAirAbsorptionHF = -5.0f;
    props.flRoomRolloffFactor = 0.0;
    props.vReverbPan.x = 0.0;
    props.vReverbPan.y = 0.0;
    props.vReverbPan.z = 0.0;
    props.vReflectionsPan.x = 0.0;
    props.vReflectionsPan.y = 0.0;
    props.vReflectionsPan.z = 0.0;

    switch (preset)
    {
    case 0:
        props.ulEnvironment = 0;
        props.flEnvironmentDiffusion = 1.0;
        props.lRoomHF = -100;
        props.lRoomLF = 0;
        props.flDecayHFRatio = 0.82999998f;
        props.lReflections = -2602;
        props.lReverb = 200;
        break;
    case 1:
        props.ulEnvironment = 2;
        props.flEnvironmentSize = 1.9f;
        props.flEnvironmentDiffusion = 1.0;
        props.lRoomHF = -454;
        props.lRoomLF = 0;
        props.flDecayTime = 0.40000001f;
        props.flDecayHFRatio = 0.82999998f;
        props.lReflections = -1646;
        props.flReflectionsDelay = 0.0020000001f;
        props.lReverb = 53;
        props.flReverbDelay = 0.003f;
        break;
    case 2:
        props.ulEnvironment = 3;
        props.flEnvironmentSize = 1.4f;
        props.flEnvironmentDiffusion = 1.0;
        props.lRoomHF = -1200;
        props.lRoomLF = 0;
        props.flDecayHFRatio = 0.54000002f;
        props.lReflections = -370;
        props.lReverb = 1030;
        break;
    case 3:
        props.ulEnvironment = 4;
        props.flEnvironmentSize = 2.5f;
        props.flEnvironmentDiffusion = 1.0;
        props.lRoomHF = -6000;
        props.lRoomLF = 0;
        props.flDecayTime = 0.5f;
        props.flDecayHFRatio = 0.1f;
        props.lReflections = -1376;
        props.flReflectionsDelay = 0.003f;
        props.lReverb = -1104;
        props.flReverbDelay = 0.0040000002f;
        break;
    case 4:
        props.ulEnvironment = 5;
        props.flEnvironmentSize = 11.6f;
        props.flEnvironmentDiffusion = 1.0;
        props.lRoomHF = -300;
        props.lRoomLF = 0;
        props.flDecayTime = 2.3099999f;
        props.flDecayHFRatio = 0.63999999f;
        props.lReflections = -711;
        props.flReflectionsDelay = 0.012f;
        props.lReverb = 83;
        props.flReverbDelay = 0.017000001f;
        break;
    case 5:
        props.ulEnvironment = 7;
        props.flEnvironmentSize = 19.6f;
        props.flEnvironmentDiffusion = 1.0;
        props.lRoomHF = -500;
        props.lRoomLF = 0;
        props.flDecayTime = 3.9200001f;
        props.flDecayHFRatio = 0.69999999f;
        props.lReflections = -1230;
        props.flReflectionsDelay = 0.02f;
        props.lReverb = -2;
        props.flReverbDelay = 0.028999999f;
        break;
    case 6:
        props.ulEnvironment = 26;
        props.flEnvironmentSize = 1.9f;
        props.flEnvironmentDiffusion = 0.87f;
        props.lRoom = -1000;
        props.lRoomHF = -400;
        props.lRoomLF = -100;
        props.flDecayTime = 7.1100001f;
        props.flDecayHFRatio = 0.38f;
        props.flDecayLFRatio = 0.61000001f;
        props.lReflections = -1500;
        props.flReflectionsDelay = 0.1f;
        props.lReverb = -1000;
        props.flReverbDelay = 0.046999998f;
        props.flEchoDepth = 0.25f;
        props.flHFReference = 3316.1001f;
        props.flLFReference = 458.20001f;
        props.ulFlags = 32;
        break;
    case 7:
        props.ulEnvironment = 24;
        props.flEnvironmentSize = 1.8f;
        props.flEnvironmentDiffusion = 0.60000002f;
        props.lRoom = -1000;
        props.lRoomHF = -400;
        props.lRoomLF = 0;
        props.flDecayTime = 17.23f;
        props.flDecayHFRatio = 0.56f;
        props.flDecayLFRatio = 1.0;
        props.lReflections = -1713;
        props.flReflectionsDelay = 0.02f;
        props.lReverb = -613;
        props.flReverbDelay = 0.029999999f;
        props.flEchoDepth = 1.0;
        props.flModulationTime = 0.81f;
        props.flModulationDepth = 0.31f;
        props.flHFReference = 5000.0f;
        props.flLFReference = 250.0f;
        props.ulFlags = 31;
        break;
    case 8:
        props.ulEnvironment = 26;
        props.flEnvironmentSize = 2.9000001f;
        props.flEnvironmentDiffusion = 0.75999999f;
        props.lRoom = -1000;
        props.lRoomHF = -700;
        props.lRoomLF = -500;
        props.flDecayTime = 5.4899998f;
        props.flDecayHFRatio = 1.53f;
        props.flDecayLFRatio = 0.38f;
        props.lReflections = -1900;
        props.flReflectionsDelay = 0.054000001f;
        props.lReverb = -1400;
        props.flReverbDelay = 0.052000001f;
        props.flEchoTime = 0.226f;
        props.flEchoDepth = 0.11f;
        props.flModulationTime = 0.25f;
        props.flModulationDepth = 0.0;
        props.flAirAbsorptionHF = 0.0;
        props.flHFReference = 12428.5f;
        props.flLFReference = 99.599998f;
        props.ulFlags = 32;
        break;
    case 9:
        props.ulEnvironment = 21;
        props.flEnvironmentSize = 1.7f;
        props.flEnvironmentDiffusion = 0.80000001f;
        props.lRoom = -1000;
        props.lRoomHF = -1000;
        props.lRoomLF = 0;
        props.flDecayTime = 2.8099999f;
        props.flDecayHFRatio = 0.14f;
        props.lReflections = 429;
        props.flReflectionsDelay = 0.014f;
        props.lReverb = 1023;
        props.flReverbDelay = 0.021f;
        break;
    case 10:
        props.ulEnvironment = 17;
        props.flEnvironmentSize = 100.0f;
        props.flEnvironmentDiffusion = 0.27000001f;
        props.lRoom = -1000;
        props.lRoomHF = -2500;
        props.lRoomLF = 0;
        props.flDecayTime = 1.49f;
        props.flDecayHFRatio = 0.20999999f;
        props.lReflections = -2780;
        props.flReflectionsDelay = 0.30000001f;
        props.lReverb = -1434;
        props.flReverbDelay = 0.1f;
        props.flEchoDepth = 1.0f;
        props.ulFlags = 31;
        break;
    case 11:
        props.ulEnvironment = 16;
        props.flEnvironmentDiffusion = 0.5f;
        props.lRoomHF = -800;
        props.lRoomLF = 0;
        props.flDecayHFRatio = 0.67000002f;
        props.lReflections = -2273;
        props.lReverb = -1691;
        break;
    }
    memcpy(dest, &props, sizeof(tSndRoomProperties));
}

int pcSND_EAXListenerPropertiesSet(int preset)
{
    tSndRoomProperties props;

    pcSND_fxStructSet(&props, preset);
    /* The listener properties went to EAX through the property set.
       There is none, so the room the engine computed is worked out and
       thrown away -- which is what happened on every card without
       EAX, and is why gFXGlobal.fxEnabled exists. */
    (void)props;
    return 0;
}

void pcSND_DummyFXFunc(tSoundFxParams* params)
{
    int count;
    int i;

    if (gFXGlobal.fxEnabled && (params->flags & 1) != 0 && params->voiceListCount > 0)
    {
        count = params->voiceListCount;
        for (i = 0; i < count; ++i)
        {
            if (params->voiceList[i]) {
                //nullsub_32();
            }
        }
    }
}

int pcSND_i_FxSetMode(tSoundFxParams* params, int mode)
{
    if (gFXGlobal.fxEnabled)
    {
        pcSND_EAXListenerPropertiesSet(mode);
        gFXGlobal.mode = params->mode;
        pcSND_DummyFXFunc(params);
    }
    return 0;
}

int pcSND_i_FxGetMode(tSoundFxParams* params)
{
    return params->mode;
}

int pcSND_i_FxSetFeedback(tSoundFxParams* params, int feedback)
{
    if (gFXGlobal.fxEnabled)
    {
        switch (params->mode)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
        case 12:
        case 65537:
        case 65538:
        case 65541:
        case 65542:
        case 65543:
        case 65544:
            break;
        default:
            params->mode = 0;
            pcSND_DummyFXFunc(params);
            break;
        }
    }
    return 0;
}

int pcSND_i_FxGetFeedback(tSoundFxParams* params)
{
    switch (params->mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 12:
    case 65537:
    case 65538:
    case 65541:
    case 65542:
    case 65543:
    case 65544:
        break;
    default:
        params->mode = 0;
        break;
    }
    return 0;
}

int pcSND_i_FxSetDelay(tSoundFxParams* params, int delay)
{
    if (!gFXGlobal.fxEnabled)
        return 0;
    switch (params->mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 12:
    case 65537:
    case 65538:
    case 65540:
    case 65541:
    case 65542:
    case 65543:
    case 65544:
        return 0;
    case 8:
    case 65536:
        pcSND_DummyFXFunc(params);
        return 0;
    default:
        params->mode = 0;
        return -1;
    }
}

int pcSND_i_FxGetDelay(tSoundFxParams* params)
{
    int result;

    switch (params->mode)
    {
    case 0:
        result = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xC:
        result = (int)params; // TODO FIXME: ezt nem gondolnám, hogy jó megnézni. mi hívja ezt?
        break;
    default:
        params->mode = 0;
        result = -1;
        break;
    }
    return result;
}

int pcSND_i_FxSetWetVolume(tSoundFxParams* params, int att)
{
    if (gFXGlobal.fxEnabled)
    {
        SND_f_GetGlobalVol();
        fxWetVol = SND_f_GetVolFromAtt(att);
        if (params->mode)
            pcSND_DummyFXFunc(params);
    }
    return 0;
}

int pcSND_i_FxGetWetVolume(tSoundFxParams* params)
{
    int mode;
    float globalVol;
    float wet = 0;

    globalVol = SND_f_GetGlobalVol();
    mode = params->mode;
    if (mode < 0 || (mode > 9 && mode != 12 && mode != 65543 && mode != 65544))
        wet = globalVol;
    fxWetVol = wet;
    return SND_l_GetAttFromVol(wet);
}

int pcSND_i_FxSetWetPan(tSoundFxParams* params, int pan)
{
    params->wetPan = pan;
    return 0;
}

int pcSND_i_FxGetWetPan(tSoundFxParams* params)
{
    return params->wetPan;
}

void pcSND_SNDPropertyEnableRoomReverb(tSoundFxParams* params, tSndSoundBufferElem* sbe)
{
    if (gFXGlobal.fxEnabled)
        pcSND_SNDPropertySetRoomReverb(sbe, 1);
}

int pcSND_i_FxEnable(tSoundFxParams* params)
{
    int i;
    tSndSoundBufferElem* sbe;

    if (gFXGlobal.fxEnabled)
    {
        params->flags |= 1u;
        for (i = 0; i < params->voiceListCount; ++i)
        {
            sbe = params->voiceList[i];
            if (sbe)
                pcSND_SNDPropertyEnableRoomReverb(params, sbe);
        }
    }
    return 0;
}

int pcSND_i_FxDisable(tSoundFxParams* params)
{
    int i;

    if (gFXGlobal.fxEnabled)
    {
        params->flags &= ~1u;
        for (i = 0; i < params->voiceListCount; ++i)
        {
            if (params->voiceList[i])
                pcSND_DisableReverbAndRestartPlaying(params, params->voiceList[i]);
        }
    }
    return 0;
}

int pcSND_Stream_GetStatus(tSndSoundBufferElem* sndBufferElem, int* status)
{
    int handle = (int)sndBufferElem;

    *status = 0;
    if (handle > 0)
        return (handle <= SND_gi_StreamListSize) - 1;
    return -1;
}

int pcSND_Stream_Play(tSndSoundBufferElem* sndBufferElem, int flags)
{
    int handle = (int)sndBufferElem;

    if (handle > 0)
        return (handle <= SND_gi_StreamListSize) - 1;
    return -1;
}

void pcSND_Stream_Duplicate(tSndHandles* handles, tSndSoundBufferElem* src, tSndSoundBufferElem** dest)
{
    tSndStreamList* from;
    tSndStreamList* to;
    int newHandle;

    int* destHandle = (int*)dest;
    int srcHandle = (int)src;

    *destHandle = 0;
    if (srcHandle > 0 && srcHandle <= SND_gi_StreamListSize)
    {
        from = &SND_gast_StreamList[srcHandle - 1];
        newHandle = SND_i_StreamAlloc();
        *destHandle = newHandle;
        if (newHandle > 0 && newHandle <= SND_gi_StreamListSize)
        {
            to = &SND_gast_StreamList[newHandle - 1];
            to->freq = from->freq;
            to->pan2 = from->pan2;
            to->pan1 = from->pan1;
            to->currPos = 0;
            to->vol = from->vol;
        }
    }
}

void pcSND_Stream_GetFrequency(tSndSoundBufferElem* sndBufferElem, int* freq)
{
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
        *freq = SND_gast_StreamList[handle - 1].freq;
}

void pcSND_Stream_GetPan(tSndSoundBufferElem* sndBufferElem, int* pan, int* front)
{
    tSndStreamList* stream;
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
    {
        stream = &SND_gast_StreamList[handle - 1];
        *pan = stream->pan1;
        *front = stream->pan2;
    }
}

void pcSND_Stream_GetCurrPos(tSndSoundBufferElem* sndBufferElem, int* currPos, int* writePos)
{
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
    {
        *currPos = SND_gast_StreamList[handle - 1].currPos;
        *writePos = 0;
    }
}

void pcSND_Stream_SetVolume(tSndSoundBufferElem* sndBufferElem, int att)
{
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
        SND_gast_StreamList[handle - 1].vol = att;
}

int pcSND_iTrack_GetStatus(tSndSoundBufferElem* key, int* status)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        trackIdx = SND_gpst_Interface.win.GetStatus(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, status);
    return trackIdx;
}

int pcSND_iTrack_Play(tSndSoundBufferElem* key, int flags)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        trackIdx = SND_gpst_Interface.win.Play(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, flags);
    return trackIdx;
}

void pcSND_Track_GetFrequency(tSndSoundBufferElem* key, int* freq)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        SND_gpst_Interface.win.GetFrequency(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, freq);
}

void pcSND_Track_GetPan(tSndSoundBufferElem* key, int* pan, int* front)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        SND_gpst_Interface.win.GetPan(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, pan, front);
}

void pcSND_Track_GetCurrPos(tSndSoundBufferElem* key, int* currPos, int* writePos)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        SND_gpst_Interface.win.GetCurrPos(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, currPos, writePos);
}

void pcSND_Track_Pause(tSndSoundBufferElem* key)
{
    int trackIdx;
    tTrackListElem* track;
    int instIdx;
    tSoundInstanceElem* inst;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
    {
        track = &SND_gax_TrackPlayingList[trackIdx];
        SND_gpst_Interface.win.Pause(track->sndBufferElem);
        instIdx = track->trackID;
        if (instIdx >= 0 && instIdx < SND_gst_Params.soundInstanceNum)
        {
            inst = &SND_gst_Params.soundInstances[instIdx];
            if ((inst->flags & 0x400) != 0)
                inst->flags &= ~0x10u;
        }
    }
}

void pcSND_Track_Release(tSndSoundBufferElem* key)
{
    int trackIdx;
    tTrackListElem* track;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
    {
        track = &SND_gax_TrackPlayingList[trackIdx];
        SND_gpst_Interface.win.Stop(track->sndBufferElem);
        SND_TrackDeleteInstanceFromList(track->trackID);
    }
}

void pcSND_Track_SetFrequency(tSndSoundBufferElem* key, int freq)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        SND_gpst_Interface.win.SetFrequency(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, freq);
}

void pcSND_Track_SetPan(tSndSoundBufferElem* key, int pan, int front)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        SND_gpst_Interface.win.SetPan(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, pan, front);
}

void pcSND_Track_SetCurrPos(tSndSoundBufferElem* key, int pos)
{
    int trackIdx;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
        SND_gpst_Interface.win.SetCurrPos(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, pos);
}

void pcSND_Track_SetVolume(tSndSoundBufferElem* key, int att)
{
    int trackIdx;
    tTrackListElem* track;
    int scaledAtt;
    float vol;

    trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (trackIdx != -1)
    {
        track = &SND_gax_TrackPlayingList[trackIdx];
        if (track->attenuated)
        {
            vol = SND_f_GetVolFromAtt(att) * track->attenuationFactor;
            scaledAtt = SND_l_GetAttFromVol(vol);
            SND_gpst_Interface.win.Setvolume(track->sndBufferElem, scaledAtt);
        }
        else
        {
            SND_gpst_Interface.win.Setvolume(track->sndBufferElem, att);
        }
    }
}

void pcSND_Track_Stop(tSndSoundBufferElem* key)
{
    int trackIdx;
    tTrackListElem* track;
    int instIdx;
    tSoundInstanceElem* inst;

    if (key)
    {
        trackIdx = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
        if (trackIdx != -1)
        {
            track = &SND_gax_TrackPlayingList[trackIdx];
            SND_gpst_Interface.win.Stop(track->sndBufferElem);
            instIdx = track->trackID;
            if (instIdx >= 0 && instIdx < SND_gst_Params.soundInstanceNum)
            {
                inst = &SND_gst_Params.soundInstances[instIdx];
                if ((inst->flags & 0x400) != 0)
                    inst->flags &= ~0x10u;
            }
        }
    }
}

void pcSND_SB_GetCurrPos(tSndSoundBufferElem* sbe, int* currPos, int* writePos)
{
    tSbeMiscData* misc;
    int playPos;

    if (sbe->miscData)
    {
        *writePos = 0;
        *currPos = 0;
        misc = sbe->miscData;
        playPos = SPL_i_VoiceCursor(sbe->voice);
        if (playPos >= misc->currentPlayPosition)
            *currPos = playPos + misc->currSize - misc->currentPlayPosition;
        else
            *currPos = playPos + misc->soundBufferSize - misc->currentPlayPosition + misc->currSize;
    }
    else
    {
        *currPos = SPL_i_VoiceCursor(sbe->voice);
        *writePos = SPL_i_VoiceWriteCursor(sbe->voice);
    }
}

void pcSND_flipPlayStopState(tSndSoundBufferElem* sbe)
{
    int status;

    status = pcSND_l_VoiceStatus(sbe->voice);
    if ((status & PCSND_PLAYING) != 0)
    {
        sbe->miscData->Stopped_SetIN49e270 = 1;
        SPL_VoiceStop(sbe->voice);
        /* The buffer took a moment to come to a stop and this spun
           until it had. A voice that has been stopped has had its
           queue dropped with it, so there is nothing left to wait
           for. */
    }
    else
    {
        sbe->miscData->Stopped_SetIN49e270 = 0;
        (SPL_VoicePlay(sbe->voice, 1), 0);
    }
}

void pcSND_SB_Pause(tSndSoundBufferElem* sbe)
{
    int idx;
    int status;

    if (sbe->miscData) {
        pcSND_flipPlayStopState(sbe);
        return;
    }
    status = pcSND_l_VoiceStatus(sbe->voice);
    if ((status & PCSND_PLAYING) != 0) {
        SPL_VoiceStop(sbe->voice);
        return;
    }
    for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
    {
        if (SND_gst_Params.soundInstances[idx].soundBufElemFromModel == sbe)
            break;
    }
    // preserved: with no matching instance the index is one past the array
    SPL_VoicePlay(sbe->voice, (SND_gst_Params.soundInstances[idx].flags >> 23) & 1);
}

/* The 3D buffer and the property set were released here. Neither
   exists, so the function does not either; its one caller says so
   where the call used to be. */

void pcSND_SB_Release(tSndSoundBufferElem* sbe)
{
    int wasListed;

    if (sbe)
    {
        BAS_bdeletegetval((int)sbe, &dsndSBList, &wasListed);
        if (sbe->miscData)
        {
            if (wasListed)
                gDoSoundGlobal.dword_B303C0 -= sbe->format;
            pcSND_MiscDataRelease(sbe);
            MEM_Free(sbe);
        }
        else
        {
            if (wasListed)
                gDoSoundGlobal.sndConcurrentlyPlayedNum -= sbe->format;
            /* The 3D buffer and the property set were given back here.
               Neither is made any more. */
            pcSND_SNDRasterRelease(sbe->voice);
            SPL_VoiceDestroy(sbe->voice);
            MEM_Free(sbe);
        }
    }
}

int pcSND_Stream_Release(tSndSoundBufferElem* sndBufferElem)
{
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
    {
        memset(&SND_gast_StreamList[handle - 1], 0, sizeof(tSndStreamList));
        return 0;
    }
    return handle;
}

int pcSND_Stream_SetFrequency(tSndSoundBufferElem* sndBufferElem, int freq)
{
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
    {
        SND_gast_StreamList[handle - 1].freq = freq;
        return 3 * handle;
    }
    return handle;
}

void pcSND_Stream_SetPan(tSndSoundBufferElem* sndBufferElem, int pan, int front)
{
    tSndStreamList* stream;
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
    {
        stream = &SND_gast_StreamList[handle - 1];
        stream->pan1 = pan;
        stream->pan2 = front;
    }
}

void pcSND_Stream_SetCurrPos(tSndSoundBufferElem* sndBufferElem, int pos)
{
    int handle = (int)sndBufferElem;

    if (handle > 0 && handle <= SND_gi_StreamListSize)
        SND_gast_StreamList[handle - 1].currPos = pos;
}

int pcSND_l_GetGlobalPan() {
    return 0;
}

void pcSND_SetGlobalPan(int pan) {
}

void pcSND_Null(Vec3* v) {
}

void pcSND_StreamPrefetch() {
}

void pcSND_StreamFlush() {
}

void pcSND_StreamGetMonoToStereo(tSndSoundBufferElem** track, tSndSoundBufferElem** resource) {
}

void pcSND_StreamGetStereoToMono() {
}

void pcSND_Stream_Pause(tSndSoundBufferElem* sndBufferElem) {
}

void pcSND_Stream_Stop(tSndSoundBufferElem* sndBufferElem) {
}

int SND_i_SoftInit()
{
    /* The window has to exist, as it did for DirectSound -- but the
       device is SDL's now and needs no native handle, which off Windows
       there is none of: MAI_gh_MainWindow stays NULL there, and asking
       for it here left the whole sound system switched off. */
    if (!PLT_p_Window())
        return -1;
    SND_gst_Params.soundRenderModeMask |= 3u;
    SND_gpst_Interface.win.ChangeRenderMode = pcSND_i_ChangeRenderMode;
    memcpy(SND_gpst_Interface.win.tag, "win ", 4);
    SND_gpst_Interface.win.GetStatus = pcSND_SB_GetStatus;
    SND_gpst_Interface.win.Play = pcSND_SB_Play;
    SND_gpst_Interface.win.Create = pcSND_pst_SB_Create;
    SND_gpst_Interface.win.Close = pcSND_Close;
    SND_gpst_Interface.win.Init = pcSND_l_Init;
    SND_gpst_Interface.win.Duplicate = pcSND_SB_Duplicate;
    SND_gpst_Interface.win.GetFrequency = pcSND_SB_GetFrequency;
    SND_gpst_Interface.win.GetPan = pcSND_SB_GetPan;
    SND_gpst_Interface.win.GetCurrPos = pcSND_SB_GetCurrPos;
    SND_gpst_Interface.win.Pause = pcSND_SB_Pause;
    SND_gpst_Interface.win.Release = pcSND_SB_Release;
    SND_gpst_Interface.win.SetFrequency = pcSND_SB_SetFrequency;
    SND_gpst_Interface.win.SetPan = pcSND_SB_SetPan;
    SND_gpst_Interface.win.SetCurrPos = pcSND_SB_SetCurrPos;
    SND_gpst_Interface.win.Setvolume = pcSND_SB_SetVolume;
    SND_gpst_Interface.win.Stop = pcSND_SB_Stop;
    SND_gpst_Interface.win.Reload = pcSND_SB_Reload;
    SND_gpst_Interface.win.StreamPrefetch = pcSND_StreamPrefetch;
    SND_gpst_Interface.win.StreamPrefetchArray = pcSND_StreamPrefetchArray;
    SND_gpst_Interface.win.StreamFlush = pcSND_StreamFlush;
    SND_gpst_Interface.win.StreamReinitAndPlay = pcSND_StreamReinitAndPlay;
    SND_gpst_Interface.win.CreateTrack = pcSND_CreateTrackSB;
    SND_gpst_Interface.win.ReleaseTrack = pcSND_ReleaseTrackSB;
    SND_gpst_Interface.win.StreamGetMonoToStereo = pcSND_StreamGetMonoToStereo;
    SND_gpst_Interface.win.StreamGetStereoToMono = pcSND_StreamGetStereoToMono;
    SND_gpst_Interface.win.ShutdownStreams = pcSND_ShutDownStreams;
    SND_gpst_Interface.win.nullptr1 = NULL;
    SND_gpst_Interface.win.GetGlobalPan = pcSND_l_GetGlobalPan;
    SND_gpst_Interface.win.GetGlobalVol = pcSND_l_GetGlobalVol;
    SND_gpst_Interface.win.SetGlobalPan = pcSND_SetGlobalPan;
    SND_gpst_Interface.win.SetGlobalVol = pcSND_SetGlobalVol;
    SND_gpst_Interface.win.ComputeDoppler = SND_ComputeDoppler;
    SND_gpst_Interface.win.Compute3DVol = SND_f_Compute3DVol;
    SND_gpst_Interface.win.SNDNull = pcSND_Null;
    SND_gpst_Interface.win.FxInit = pcSND_i_FxInit;
    SND_gpst_Interface.win.FxClose = pcSND_FxClose;
    SND_gpst_Interface.win.FxSetMode = pcSND_i_FxSetMode;
    SND_gpst_Interface.win.FxGetMode = pcSND_i_FxGetMode;
    SND_gpst_Interface.win.FxSetFeedback = pcSND_i_FxSetFeedback;
    SND_gpst_Interface.win.FxGetFeedback = pcSND_i_FxGetFeedback;
    SND_gpst_Interface.win.FxSetDelay = pcSND_i_FxSetDelay;
    SND_gpst_Interface.win.FxGetDelay = pcSND_i_FxGetDelay;
    SND_gpst_Interface.win.FxSetWetVolume = pcSND_i_FxSetWetVolume;
    SND_gpst_Interface.win.FxGetWetVolume = pcSND_i_FxGetWetVolume;
    SND_gpst_Interface.win.FxSetWetPan = pcSND_i_FxSetWetPan;
    SND_gpst_Interface.win.FxGetWetPan = pcSND_i_FxGetWetPan;
    SND_gpst_Interface.win.FxEnable = pcSND_i_FxEnable;
    SND_gpst_Interface.win.FxDisable = pcSND_i_FxDisable;
    memcpy(&SND_gpst_Interface.stream, &SND_gpst_Interface, sizeof(SND_gpst_Interface.stream));
    memcpy(SND_gpst_Interface.stream.tag, "stre", 4);
    SND_gpst_Interface.stream.Close = 0;
    SND_gpst_Interface.stream.GetStatus = pcSND_Stream_GetStatus;
    SND_gpst_Interface.stream.Play = pcSND_Stream_Play;
    SND_gpst_Interface.stream.Create = 0;
    SND_gpst_Interface.stream.Duplicate = pcSND_Stream_Duplicate;
    SND_gpst_Interface.stream.GetFrequency = pcSND_Stream_GetFrequency;
    SND_gpst_Interface.stream.GetPan = pcSND_Stream_GetPan;
    SND_gpst_Interface.stream.GetCurrPos = pcSND_Stream_GetCurrPos;
    SND_gpst_Interface.stream.Pause = pcSND_Stream_Pause;
    /* Both return a handle that no caller can reach: every one of
       them goes through this slot, which is declared to return
       nothing. The cast says that is meant, and the functions keep
       their own signatures. */
    SND_gpst_Interface.stream.Release =
        (void (*)(tSndSoundBufferElem*))pcSND_Stream_Release;
    SND_gpst_Interface.stream.SetFrequency =
        (void (*)(tSndSoundBufferElem*, int))pcSND_Stream_SetFrequency;
    SND_gpst_Interface.stream.SetPan = pcSND_Stream_SetPan;
    SND_gpst_Interface.stream.SetCurrPos = pcSND_Stream_SetCurrPos;
    SND_gpst_Interface.stream.Setvolume = pcSND_Stream_SetVolume;
    SND_gpst_Interface.stream.Stop = pcSND_Stream_Stop;
    memcpy(&SND_gpst_Interface.music, &SND_gpst_Interface.stream, sizeof(SND_gpst_Interface.music));
    memcpy(SND_gpst_Interface.music.tag, "musi", 4);
    memcpy(&SND_gpst_Interface.ambience, &SND_gpst_Interface.stream, sizeof(SND_gpst_Interface.ambience));
    memcpy(SND_gpst_Interface.ambience.tag, "ambi", 4);
    memcpy(&SND_gpst_Interface.dialog, &SND_gpst_Interface.stream, sizeof(SND_gpst_Interface.dialog));
    memcpy(SND_gpst_Interface.dialog.tag, "dial", 4);
    memcpy(&SND_gpst_Interface.track, &SND_gpst_Interface, sizeof(SND_gpst_Interface.track));
    SND_gpst_Interface.track.Close = 0;
    SND_gpst_Interface.track.Create = 0;
    SND_gpst_Interface.track.Duplicate = 0;
    memcpy(SND_gpst_Interface.track.tag, "trac", 4);
    SND_gpst_Interface.track.GetStatus = pcSND_iTrack_GetStatus;
    SND_gpst_Interface.track.Play = pcSND_iTrack_Play;
    SND_gpst_Interface.track.GetFrequency = pcSND_Track_GetFrequency;
    SND_gpst_Interface.track.GetPan = pcSND_Track_GetPan;
    SND_gpst_Interface.track.GetCurrPos = pcSND_Track_GetCurrPos;
    SND_gpst_Interface.track.Pause = pcSND_Track_Pause;
    SND_gpst_Interface.track.Release = pcSND_Track_Release;
    SND_gpst_Interface.track.SetFrequency = pcSND_Track_SetFrequency;
    SND_gpst_Interface.track.SetPan = pcSND_Track_SetPan;
    SND_gpst_Interface.track.SetCurrPos = pcSND_Track_SetCurrPos;
    SND_gpst_Interface.track.Setvolume = pcSND_Track_SetVolume;
    SND_gpst_Interface.track.Stop = pcSND_Track_Stop;
    return 0;
}
