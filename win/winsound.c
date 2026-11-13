#include "../common.h"
// Do not include these! dsound.h defines IKsPropertySet and differently!
//#include <ks.h>
//#include <ksproxy.h>
#include "adpcm.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../time.h"
#include "wingdi.h"
#include "winsound.h"

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

static int pcSND_ReadFXSettingFromRegistry(LPBYTE dest)
{
    DWORD size;
    HKEY key;
    DWORD type;

    if (RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\IRISEng\\Sound",
        0,
        0x20019u,
        &key))
    {
        return 1;
    }
    size = 4;
    // preserved: the key is left open when the value cannot be read
    if (RegQueryValueExA(key, "Sound FX", 0, &type, dest, &size) || size != 4 || type != 4)
        return 1;
    RegCloseKey(key);
    return 0;
}

static int pcSND_ReadAudioBoardGuid(LPBYTE dest)
{
    DWORD status;
    HKEY key;
    DWORD size;
    DWORD type;
    CHAR message[1000];

    if (!RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\IRISEng\\Sound",
        0,
        1u,
        &key))
    {
        size = 16;
        status = RegQueryValueExA(key, "Audio board GUID", 0, &type, dest, &size);
        if (status)
        {
            GetLastError();
            FormatMessageA(0x1000u, 0, status, 0, message, 1000u, 0);
        }
        else if (size == 16 && type == 3)
        {
            RegCloseKey(key);
            return 0;
        }
    }
    // preserved: the key is left open on every failing path
    return 1;
}

static HRESULT pcSND_EAXDirectSoundCreate(GUID* driverGuid, LPDIRECTSOUND8* dsound, IUnknown* outer)
{
    snd_eaxDLLHandle = LoadLibraryA("eax.dll");
    pEAXDirectSoundCreate8 = (PEAXDirectSoundCreate8)GetProcAddress(snd_eaxDLLHandle, "EAXDirectSoundCreate8");
    return pEAXDirectSoundCreate8(driverGuid, dsound, outer);
}

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
    if (notifyThread)
        SetEvent(dsndEvents[5]);
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
            SetEvent(dsndEvents[5]);
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
    ResetEvent(sbe->miscData->eventHandle1);
    sbe->miscData->eventHandle1 = 0;
    ResetEvent(sbe->miscData->eventHandle2);
    sbe->miscData->eventHandle2 = 0;
    if (fireEvent)
        SetEvent(dsndEvents[5]);
}

void pcSND_StopSoundAndRemoveFromThePlayList(tSndSoundBufferElem* sbe, int fireEvent)
{
    sbe->dsndSBIF->lpVtbl->Stop(sbe->dsndSBIF);
    pcSND_RemoveSoundFromPlayList(sbe, fireEvent);
}

void pcSND_updateCurrentPlayPositionAndSize(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    int advanced;
    int playPos;

    sbe->dsndSBIF->lpVtbl->GetCurrentPosition(sbe->dsndSBIF, &playPos, 0);
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
    ReadFile(sbe->miscData->fileHandle, buffer, readSize, decodedSize, 0);
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

int WINAPI pcSND_SoundThread(LPVOID parameter)
{
    DWORD signalled;
    tSndSoundBufferElem* sbe;
    tSbeMiscData* misc;
    IDirectSoundBuffer* buffer;
    DWORD produced;
    unsigned int audioBytes1;
    char* audioPtr1;
    DWORD totalSizeAfterDecompress;
    LONG distanceToMoveHigh;

    distanceToMoveHigh = 0;
    while (1)
    {
        do
            signalled = WaitForMultipleObjects(sndEventHandleNum, sndEventHandles, 0, 0xFFFFFFFF);
        while (signalled == -1);

        switch (signalled)
        {
        case 0u:
            pcSND_AddSoundToThePlayList(1);
            break;
        case 1u:
            pcSND_RemoveSoundFromPlayList(sndCurrSoundBufferElem, 1);
            break;
        case 2u:
            SetEvent(dsndEvents[5]);
            ExitThread(0);
        case 3u:
            pcSND_StopSoundAndRemoveFromThePlayList(sndCurrSoundBufferElem, 1);
            break;
        default:
            sbe = sndPlayList[signalled >> 1].soundElem;
            misc = sbe->miscData;
            if (misc->Stopped_SetIN49e270 != 1)
            {
                pcSND_updateCurrentPlayPositionAndSize(sndPlayList[signalled >> 1].soundElem);
                if (misc->RemoveAfterPlayFinished && misc->currSize >= misc->fullSize)
                    misc->shouldBeRemovedFromPlayList = 1;
                if (misc->shouldBeRemovedFromPlayList == 1 || (signalled & 1) != 0)
                {
                    pcSND_StopSoundAndRemoveFromThePlayList(sbe, 0);
                }
                else if (misc->RemoveAfterPlayFinished)
                {
                    buffer = sbe->dsndSBIF;
                    buffer->lpVtbl->Lock(buffer, misc->currOffset, misc->sizeToUpload, &audioPtr1, &audioBytes1, 0, 0, 0);
                    memset(audioPtr1, 0, audioBytes1);
                    sbe->dsndSBIF->lpVtbl->Unlock(sbe->dsndSBIF, audioPtr1, audioBytes1, 0, 0);
                }
                else
                {
                    buffer = sbe->dsndSBIF;
                    buffer->lpVtbl->Lock(buffer, misc->currOffset, misc->sizeToUpload, &audioPtr1, &audioBytes1, 0, 0, 0);
                    totalSizeAfterDecompress = 0;
                    pcSND_ReadAndDecompressSoundData(audioBytes1, sbe, 1, &totalSizeAfterDecompress, (int16_t*)audioPtr1);
                    produced = totalSizeAfterDecompress;
                    if (totalSizeAfterDecompress < audioBytes1)
                    {
                        if (misc->readThroughWithMultipleLoop)
                        {
                            do
                            {
                                SetFilePointer(misc->fileHandle, misc->distanceToMove, &distanceToMoveHigh, 0);
                                misc->sumSize = 0;
                                pcSND_ReadAndDecompressSoundData(audioBytes1 - produced, sbe, 1, &totalSizeAfterDecompress,
                                                                 (int16_t*)&audioPtr1[produced]);
                                produced += totalSizeAfterDecompress;
                            } while (produced < audioBytes1);
                        }
                        else
                        {
                            misc->RemoveAfterPlayFinished = 1;
                            memset(&audioPtr1[totalSizeAfterDecompress], 0, audioBytes1 - totalSizeAfterDecompress);
                        }
                    }
                    sbe->dsndSBIF->lpVtbl->Unlock(sbe->dsndSBIF, audioPtr1, audioBytes1, 0, 0);
                    misc->currOffset += audioBytes1;
                    if (misc->currOffset >= misc->soundBufferSize)
                        misc->currOffset -= misc->soundBufferSize;
                }
            }
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
        for (i = 0; i < 23; ++i)
            dsndEvents[i] = CreateEventA(NULL, FALSE, FALSE, NULL);
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
        pcSNDhObject = CreateThread(0, 0, pcSND_SoundThread, 0, 0, &pcSNDThreadId);
    }
}

int pcSND_CreateSoundBuffer(IDirectSoundBuffer* primary, IDirectSound8* dsndif)
{
    WAVEFORMATEX format;
    DSBUFFERDESC desc;
    ULONG support;

    primary->lpVtbl->QueryInterface(primary, &IID_IDirectSound3DListener, &dsndIDisectSound3DListenerIF);
    memset(&desc, 0, sizeof(desc));
    format.wFormatTag = 1;
    format.nChannels = 1;
    format.nSamplesPerSec = 10000;
    format.nAvgBytesPerSec = 10000;
    format.nBlockAlign = 1;
    format.wBitsPerSample = 8;
    format.cbSize = 0;
    desc.dwSize = 36;
    desc.dwFlags = 0x90;
    desc.dwBufferBytes = 10000;
    desc.lpwfxFormat = &format;
    desc.guid3DAlgorithm.Data1 = 0;
    desc.guid3DAlgorithm.Data2 = 0;
    desc.guid3DAlgorithm.Data3 = 0;
    memset(desc.guid3DAlgorithm.Data4, 0, sizeof(desc.guid3DAlgorithm.Data4));
    dsndif->lpVtbl->CreateSoundBuffer(dsndif, &desc, &d3dSndBuffer, 0);
    d3dSndBuffer->lpVtbl->QueryInterface(d3dSndBuffer, &IID_IDirectSound3DBuffer, &ppdsndDSND3DBuffer);
    d3dSndBuffer->lpVtbl->QueryInterface(d3dSndBuffer, &IID_IKsPropertySet, &ppdsndIKSPropertySet);
    if (ppdsndIKSPropertySet->lpVtbl->QuerySupport(ppdsndIKSPropertySet, &DSPROPSETID_EAX30_ListenerProperties, 1, &support) < 0)
    {
        pcSNDEAXLisenerPropertiesSetIsOk = 0;
    }
    else
    {
        pcSNDEAXLisenerPropertiesSetIsOk = 1;
        if (((uint8_t)support & 3) != 3)
            pcSNDEAXLisenerPropertiesSetIsOk = 0;
    }
    return 0;
}

BOOL pcSND_CreateSoundBufferAndPlayTest(tSndHandles* handles)
{
    IDirectSound8* dsndif;
    IDirectSoundBuffer* primary;
    WAVEFORMATEX format;
    DSBUFFERDESC desc;

    memset(&desc, 0, sizeof(desc));
    dsndif = handles->dsndif;
    desc.dwSize = 36;
    desc.dwFlags = 17;
    desc.dwBufferBytes = 0;
    desc.lpwfxFormat = 0;
    desc.guid3DAlgorithm.Data1 = 0;
    desc.guid3DAlgorithm.Data2 = 0;
    memset(desc.guid3DAlgorithm.Data4, 0, sizeof(desc.guid3DAlgorithm.Data4));
    if (dsndif->lpVtbl->CreateSoundBuffer(dsndif, &desc, &handles->SoundBuffer, 0) < 0)
        return 0;
    primary = handles->SoundBuffer;
    format.wFormatTag = 1;
    format.nChannels = 2;
    format.nSamplesPerSec = 48000;
    format.nAvgBytesPerSec = 192000;
    format.nBlockAlign = 4;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    if (primary->lpVtbl->SetFormat(primary, &format) != S_OK || pcSND_CreateSoundBuffer(handles->SoundBuffer, handles->dsndif))
        return 0;
    return handles->SoundBuffer->lpVtbl->Play(handles->SoundBuffer, 0, 0, 1) == 0;
}

int pcSND_l_Init(tSndHandles* sndh)
{
    BYTE fxSetting[4];
    BYTE driverGuid[16];

    BAS_binit(&dsndSBList, 48);
    memset(&gDoSoundGlobal, 0, sizeof(gDoSoundGlobal));
    pcSNDhObject = NULL;
    gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 = 0;
    sndh->MainWindowHandle = MAI_gh_MainWindow;
    if (CoInitialize(NULL) != S_OK || pcSND_ReadFXSettingFromRegistry(fxSetting) || pcSND_ReadAudioBoardGuid(driverGuid))
        return 0;
    if (pcSND_EAXDirectSoundCreate((GUID*)driverGuid, &sndh->dsndif, 0) != S_OK)
        return 0;
    gDoSoundGlobal.dsndCaps.dwSize = 96;
    sndh->dsndif->lpVtbl->GetCaps(sndh->dsndif, &(gDoSoundGlobal.dsndCaps));
    sndh->dsndif->lpVtbl->SetCooperativeLevel(sndh->dsndif, sndh->MainWindowHandle, 3);
    if (!pcSND_CreateSoundBufferAndPlayTest(sndh))
        return 0;
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
            sbe->dsndSBIF->lpVtbl->GetVolume(sbe->dsndSBIF, &vol);
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
            sbe->dsndSBIF->lpVtbl->SetPan(sbe->dsndSBIF, pan);
            sbe->dsndSBIF->lpVtbl->SetVolume(sbe->dsndSBIF, vol);
        }
    }
    return 0;
}

int pcSND_SetDefaultSoundPositionAndDistance(tSndSoundBufferElem* sbe)
{
    IDirectSound3DBuffer** buffer3d;
    void** failed;

    buffer3d = &sbe->dsnd3dbufferIF;
    failed = buffer3d; // TODO FIXME: ezek így biztosan jók?
    if (SUCCEEDED(sbe->dsndSBIF->lpVtbl->QueryInterface(sbe->dsndSBIF, &IID_IDirectSound3DBuffer, &sbe->dsnd3dbufferIF)))
    {
        // preserved: the distances and the position are float bit patterns passed as integers
        (*buffer3d)->lpVtbl->SetMinDistance(*buffer3d, 0x40000000, 0);
        (*buffer3d)->lpVtbl->SetMaxDistance(*buffer3d, 0x40400000, 0);
        (*buffer3d)->lpVtbl->SetPosition(*buffer3d, 0, 0, 0x3F800000, 0);
        failed = &sbe->dsndIKPropertySetIF;
        if (SUCCEEDED(sbe->dsndSBIF->lpVtbl->QueryInterface(sbe->dsndSBIF, &IID_IKsPropertySet, &sbe->dsndIKPropertySetIF)))
            return 0;
    }
    *failed = NULL;
    return -1;
}

int pcSND_SNDPropertySetRoomReverb(tSndSoundBufferElem* sbe, int enable)
{
    IKsPropertySet** propertySet;
    int room;

    room = enable != 0 ? 0 : -10000;
    propertySet = &sbe->dsndIKPropertySetIF;
    // preserved: the guard tests the address of the field, not the interface in it
    if (propertySet && ((*propertySet)->lpVtbl->Set(*propertySet, &DSPROPSETID_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_ROOM, 0, 0, (LPVOID*)&room, 4) != S_OK))
        return -1;
    return 0;
}

void pcSND_DisableReverbAndRestartPlaying(tSoundFxParams* params, tSndSoundBufferElem* sbe)
{
    IDirectSoundBuffer* buffer;
    unsigned int status;

    if (pcSNDFXEnabled_dword_B30400)
    {
        sbe->dsndSBIF->lpVtbl->GetStatus(sbe->dsndSBIF, &status);
        if ((status & 1) != 0)
            sbe->dsndSBIF->lpVtbl->Stop(sbe->dsndSBIF);
        pcSND_SNDPropertySetRoomReverb(sbe, 0);
        if ((status & 1) != 0)
        {
            buffer = sbe->dsndSBIF;
            buffer->lpVtbl->Play(buffer, 0, 0, (status >> 2) & 1);
        }
    }
}

int pcSND_DisableReverbIf_i7_not_match_with_idx(tSoundFxParams* params, tSndSoundBufferElem* sbe)
{
    int count;
    int idx;

    count = params->i7;
    for (idx = 0; idx < count; ++idx)
    {
        if (params->i8[idx] == sbe)
            break;
    }
    if (idx != count)
    {
        pcSND_DisableReverbAndRestartPlaying(params, sbe);
        params->i8[idx] = NULL;
    }
    return 0;
}

void pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(tSndSoundBufferElem* sbe)
{
    if (sbe->miscData->eventHandle2 != (HANDLE)-1)
    {
        sndCurrSoundBufferElem = sbe;
        SetEvent(dsndEvents[3]);
        WaitForSingleObject(dsndEvents[5], 5000u);
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
            sbe->dsndSBIF->lpVtbl->Stop(sbe->dsndSBIF);
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
    result = sbe->dsndSBIF->lpVtbl->GetStatus(sbe->dsndSBIF, status);
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
    if ((sbe->dsndSBIF->lpVtbl->Play(sbe->dsndSBIF, 0, 0, flags & 1)) != S_OK)
        OutputDebugStringA("Voice not allocated in hardware");
    else
        OutputDebugStringA("HARDWARE voice\n");
    BAS_binsert((unsigned int)sbe, (int)sbe, &dsndSBList);
    return 0;
}

static int pcSND_RasterBudgetLeft(void)
{
    return 6000000 - sndRasterStat.i7;
}

void pcSND_AddToSNDRaster(IDirectSoundBuffer* buffer, int bytes)
{
    int fileId;

    if (SND_gst_Params.soundEnabled)
    {
        BAS_binsert((unsigned int)buffer, bytes, &rasterbas1);
        BAS_binsert((unsigned int)buffer, SND_gi_RasterCurrentFileId, &rasterbas2);
        fileId = SND_gi_RasterCurrentFileId;
        if (fileId == 0x2000 || fileId == 0x4000 || fileId == 0x80000)
            sndRasterStat.i1 += bytes;
        else if (fileId == 8)
            sndRasterStat.i3 += bytes;
        else
            sndRasterStat.i5 += bytes;
        sndRasterStat.i7 += bytes;
        sndRasterStat.i8 = pcSND_RasterBudgetLeft();
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
    IDirectSound8* dsndIf;
    IDirectSoundBuffer* dsndSBIF;
    char* audioPtr1;
    char* audioPtr2;
    unsigned int audioBytes1;
    int audioBytes2;
    DSBUFFERDESC dsBufferDesc;

    if (!handles->dsndif)
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
    memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));
    dsBufferDesc.dwSize = 36;
    dsBufferDesc.dwFlags = 0x400B0;
    dsBufferDesc.dwBufferBytes = sndHdr->fullSoundSize;
    dsBufferDesc.lpwfxFormat = &sndHdr->wfex;
    dsBufferDesc.guid3DAlgorithm.Data1 = 0;
    dsBufferDesc.guid3DAlgorithm.Data2 = 0;
    dsBufferDesc.guid3DAlgorithm.Data3 = 0;
    memset(dsBufferDesc.guid3DAlgorithm.Data4, 0, sizeof(dsBufferDesc.guid3DAlgorithm.Data4));
    dsndIf = handles->dsndif;
    if ((dsndIf->lpVtbl->CreateSoundBuffer(dsndIf, &dsBufferDesc, &(sndSNDBuf->dsndSBIF), 0)) != S_OK)
    {
        if (sndData)
            MEM_Free(sndData);
        MEM_Free(sndSNDBuf);
        return 0;
    }
    dsndSBIF = sndSNDBuf->dsndSBIF;
    sndSNDBuf->format = dsBufferDesc.lpwfxFormat->nChannels;
    pcSND_AddToSNDRaster(dsndSBIF, dsBufferDesc.dwBufferBytes);
    if (!sndData)
        return sndSNDBuf;
    if ((sndSNDBuf->dsndSBIF->lpVtbl->Lock(sndSNDBuf->dsndSBIF, 0, sndHdr->fullSoundSize, &audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, 0)) != S_OK)
    {
        sndSNDBuf->dsndSBIF->lpVtbl->Release(sndSNDBuf->dsndSBIF);
        MEM_Free(sndSNDBuf);
        MEM_Free(sndData);
        return NULL;
    }
    memcpy(audioPtr1, sndData, audioBytes1);
    if (audioBytes2)
        memcpy(audioPtr2, &sndData[audioBytes1], (unsigned int)audioBytes2);
    sndSNDBuf->dsndSBIF->lpVtbl->Unlock(sndSNDBuf->dsndSBIF, audioPtr1, audioBytes1, audioPtr2, audioBytes2);
    pcSND_SetDefaultSoundPositionAndDistance(sndSNDBuf);
    MEM_Free(sndData);
    return sndSNDBuf;
}

int pcSND_DSoundBuffersRelease()
{
    dsndIDisectSound3DListenerIF->lpVtbl->Release(dsndIDisectSound3DListenerIF);
    d3dSndBuffer->lpVtbl->Release(d3dSndBuffer);
    ppdsndDSND3DBuffer->lpVtbl->Release(ppdsndDSND3DBuffer);
    return ppdsndIKSPropertySet->lpVtbl->Release(ppdsndIKSPropertySet);
}

void pcSND_Cleanup()
{
    int i;

    if (!SND_gc_NoSound)
    {
        sndCurrSoundBufferElem = (tSndSoundBufferElem*)-1;
        SetEvent(dsndEvents[2]);
        WaitForSingleObject(dsndEvents[5], 5000u);
        WaitForSingleObject(pcSNDhObject, 5000u);
        CloseHandle(pcSNDhObject);
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
        for (i = 0; i < 23; ++i)
        {
            if (dsndEvents[i])
                CloseHandle(dsndEvents[i]);
            dsndEvents[i] = 0;
        }
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
    pcSND_DSoundBuffersRelease();
    handles->dsndif->lpVtbl->Release(handles->dsndif);
    CoUninitialize();
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
            handles->dsndif->lpVtbl->DuplicateSoundBuffer(handles->dsndif, src->dsndSBIF, &((*dest)->dsndSBIF));
            (*dest)->format = src->format;
            (*dest)->unk2SetTo64InCreate = 64;
            pcSND_SetDefaultSoundPositionAndDistance(*dest);
        }
    }
}

void pcSND_SB_GetFrequency(tSndSoundBufferElem* sbe, int* freq)
{
    if (sbe)
        sbe->dsndSBIF->lpVtbl->GetFrequency(sbe->dsndSBIF, freq);
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

int pcSND_3DSoundBufferSetPos(tSndSoundBufferElem* sbe, int pan)
{
    IDirectSound3DBuffer* buffer3d;
    long double angle;
    float x;
    float y;

    buffer3d = sbe->dsnd3dbufferIF;
    if (buffer3d)
    {
        angle = (10000.0 - (double)pan) * 0.0089999996 * 0.017453292;
        y = (float)sin(angle);
        x = (float)cos(angle);
        buffer3d->lpVtbl->SetPosition(buffer3d, x, y, 0.0f, 0);
    }
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
    sbe->dsndSBIF->lpVtbl->GetVolume(sbe->dsndSBIF, &vol);
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
        sbe->dsndSBIF->lpVtbl->SetPan(sbe->dsndSBIF, 0);
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
        pcSND_3DSoundBufferSetPos(sbe, side);
        if (sbe->unknownFiledinsub49db40ToSetVolume != vol)
            sbe->dsndSBIF->lpVtbl->SetVolume(sbe->dsndSBIF, vol);
    }
}

void pcSND_SB_SetCurrPos(tSndSoundBufferElem* sbe, int pos)
{
    if (sbe)
        sbe->dsndSBIF->lpVtbl->SetCurrentPosition(sbe->dsndSBIF, pos);
}

void pcSND_SB_SetVolume(tSndSoundBufferElem* sbe, int att)
{
    int base;
    int balance;
    int vol;
    IDirectSoundBuffer* buffer;

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
        buffer = sbe->dsndSBIF;
        sbe->unknownFiledinsub49db40ToSetVolume = vol;
        buffer->lpVtbl->SetVolume(buffer, vol);
    }
}

void pcSND_SNDRasterRelease(IDirectSoundBuffer* buffer)
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
                sndRasterStat.i1 -= bytes;
            else if (fileId == 8)
                sndRasterStat.i3 -= bytes;
            else
                sndRasterStat.i5 -= bytes;
            sndRasterStat.i7 -= bytes;
            sndRasterStat.i8 = pcSND_RasterBudgetLeft();
        }
    }
}

void pcSND_MiscDataRelease(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    IDirectSoundNotify* notify;

    if (sbe)
    {
        misc = sbe->miscData;
        if (misc)
        {
            pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(sbe);
            if (misc->fileHandle != (HANDLE)-1)
                CloseHandle(misc->fileHandle);
            notify = misc->vmilenEventHandlptr_Sub49e950;
            misc->fileHandle = (HANDLE)-1;
            if (notify)
                notify->lpVtbl->Release(notify);
            misc->vmilenEventHandlptr_Sub49e950 = 0;
            if (sbe->dsndSBIF)
            {
                pcSND_SNDRasterRelease(sbe->dsndSBIF);
                sbe->dsndSBIF->lpVtbl->Release(sbe->dsndSBIF);
            }
            sbe->dsndSBIF = 0;
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
                ((void(__stdcall*)(dsndSoundBuffer**, _DWORD, _DWORD, _DWORD))(*v1->dsndSBIF)[1].QueryInterface)(
                    v1->dsndSBIF,
                    0,
                    0,
                    0);
            if (*(_DWORD*)v1->gap)
                MEM_Free(*(void**)v1->gap);
            */
            pcSND_SNDRasterRelease(sbe->dsndSBIF);
            sbe->dsndSBIF->lpVtbl->Release(sbe->dsndSBIF);
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
    IDirectSound8* dsndIf;
    IDirectSoundBuffer* dsndSBIF;
    char* audioPtr1;
    char* audioPtr2;
    unsigned int audioBytes1;
    int audioBytes2;
    DSBUFFERDESC dsBufferDesc;

    if (!handles->dsndif)
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
    memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));
    dsBufferDesc.dwSize = 36;
    dsBufferDesc.dwFlags = pcSND_Return160();
    dsBufferDesc.dwBufferBytes = wavHdr->fullSoundSize;
    dsBufferDesc.lpwfxFormat = &wavHdr->wfex;
    dsBufferDesc.guid3DAlgorithm.Data1 = 0;
    dsBufferDesc.guid3DAlgorithm.Data2 = 0;
    dsBufferDesc.guid3DAlgorithm.Data3 = 0;
    memset(dsBufferDesc.guid3DAlgorithm.Data4, 0, sizeof(dsBufferDesc.guid3DAlgorithm.Data4));
    dsndIf = handles->dsndif;
    if (dsndIf->lpVtbl->CreateSoundBuffer(dsndIf, &dsBufferDesc, &(sndSNDBuf->dsndSBIF), 0) != S_OK)
    {
        if (sndData)
            MEM_Free(sndData);
        MEM_Free(sndSNDBuf);
        return 0;
    }
    dsndSBIF = sndSNDBuf->dsndSBIF;
    sndSNDBuf->format = dsBufferDesc.lpwfxFormat->nChannels;
    pcSND_AddToSNDRaster(dsndSBIF, dsBufferDesc.dwBufferBytes);
    if (!sndData)
        return sndSNDBuf;
    if (sndSNDBuf->dsndSBIF->lpVtbl->Lock(sndSNDBuf->dsndSBIF, 0, wavHdr->fullSoundSize, &audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, 0) != S_OK)
    {
        sndSNDBuf->dsndSBIF->lpVtbl->Release(sndSNDBuf->dsndSBIF);
        MEM_Free(sndSNDBuf);
        MEM_Free(sndData);
        return NULL;
    }
    memcpy(audioPtr1, sndData, audioBytes1);
    if (audioBytes2)
        memcpy(audioPtr2, &sndData[audioBytes1], audioBytes2);
    sndSNDBuf->dsndSBIF->lpVtbl->Unlock(sndSNDBuf->dsndSBIF, audioPtr1, audioBytes1, audioPtr2, audioBytes2);
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
        SetEvent(dsndEvents[0]);
        WaitForSingleObject(dsndEvents[5], 5000);
    }
}

void pcSND_SetPositionNotify(tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    DWORD step;
    int i;
    DSBPOSITIONNOTIFY notify[5];

    misc = sbe->miscData;
    step = misc->sizeToUpload;
    pcSND_setCurrentSoundBuffElemAndSetAnEvent(sbe);
    notify[0].dwOffset = step;
    notify[0].hEventNotify = misc->eventHandle1;
    for (i = 1; i < 4; ++i)
    {
        notify[i].dwOffset = step + notify[i - 1].dwOffset;
        notify[i].hEventNotify = misc->eventHandle1;
    }
    --notify[3].dwOffset;
    notify[4].dwOffset = -1;
    notify[4].hEventNotify = misc->eventHandle2;
    misc->vmilenEventHandlptr_Sub49e950->lpVtbl->SetNotificationPositions(misc->vmilenEventHandlptr_Sub49e950, 5, notify);
}

void pcSND_fillSoundBuffer(int unused, tSbeMiscData* miscArg, tSndSoundBufferElem* sbe)
{
    tSbeMiscData* misc;
    int skipSize;
    int produced;
    int decoded;
    char* audioPtr1;
    char* audioPtr2;
    int audioBytes1;
    int audioBytes2;
    DWORD distanceToMoveHigh;

    misc = sbe->miscData;
    distanceToMoveHigh = 0;
    if (misc)
    {
        sbe->dsndSBIF->lpVtbl->Lock(sbe->dsndSBIF, 0, misc->soundBufferSize, &audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, 2);
        skipSize = pcSND_CalcSizeToReadByFormat(misc->unk6, sbe->format);
        SetFilePointer(misc->fileHandle, misc->distanceToMove + skipSize, &distanceToMoveHigh, 0);
        misc->sumSize = misc->unk6;
        pcSND_ReadAndDecompressSoundData(audioBytes1, sbe, 0, &decoded, (int16_t*)audioPtr1);
        produced = decoded;
        if (decoded < audioBytes1)
        {
            if (misc->readThroughWithMultipleLoop)
            {
                do
                {
                    SetFilePointer(misc->fileHandle, misc->distanceToMove, &distanceToMoveHigh, 0);
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
        sbe->dsndSBIF->lpVtbl->Unlock(sbe->dsndSBIF, audioPtr1, audioBytes1, 0, 0);
        misc->currOffset += audioBytes1;
        if (misc->currOffset >= misc->soundBufferSize)
            misc->currOffset -= misc->soundBufferSize;
        misc->currSize = misc->unk6;
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
        sbe->dsndSBIF->lpVtbl->SetFrequency(sbe->dsndSBIF, freq);
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
    sbe->dsndSBIF->lpVtbl->GetVolume(sbe->dsndSBIF, &vol);
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
        sbe->dsndSBIF->lpVtbl->SetPan(sbe->dsndSBIF, 0);
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
        sbe->dsndSBIF->lpVtbl->SetPan(sbe->dsndSBIF, side);
        if (sbe->unknownFiledinsub49db40ToSetVolume != vol)
            sbe->dsndSBIF->lpVtbl->SetVolume(sbe->dsndSBIF, vol);
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
        // preserved: this is the ADPCM block stride, which equals the struct size only in a 32-bit build
        startSize = pcSND_calcMemoryNeededForTheSound(sizeof(tSndSoundBufferElem) * sbe->format * (readSize / (sizeof(tSndSoundBufferElem) * sbe->format)), sbe->format);
        if (startSize < 0 || startSize > fullSize)
            startSize = 0;
        misc->RemoveAfterPlayFinished = 0;
        misc->shouldBeRemovedFromPlayList = 0;
        misc->Stopped_SetIN49e270 = 0;
        misc->readThroughWithMultipleLoop = flags & 1;
        misc->currOffset = 0;
        misc->currentPlayPosition = 0;
        misc->currSize = 0;
        misc->unk6 = startSize;
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
            sbe->dsndSBIF->lpVtbl->SetCurrentPosition(sbe->dsndSBIF, 0);
            sbe->dsndSBIF->lpVtbl->Play(sbe->dsndSBIF, 0, 0, 1);
        }
    }
}

tSndSoundBufferElem* pcSND_CreateTrack(tSndHandles* handles, tSndWavHdrStruct* wavHdr)
{
    tSndSoundBufferElem* track;
    tSbeMiscData* misc;
    IDirectSound8* dsndif;
    DSBUFFERDESC desc;

    wavHdr->wfex.wFormatTag = 1;
    if (!handles->dsndif)
        return 0;
    track = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(track, 0, sizeof(tSndSoundBufferElem));
    misc = (tSbeMiscData*)MEM_p_Alloc(sizeof(tSbeMiscData));
    memset(misc, 0, sizeof(tSbeMiscData));
    misc->sizeToUpload = 307200 / wavHdr->wfex.nBlockAlign / 4;
    misc->sizeToUpload *= wavHdr->wfex.nBlockAlign;
    misc->soundBufferSize = 4 * misc->sizeToUpload;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = 36;
    desc.dwFlags = 0x101E8;
    desc.dwBufferBytes = misc->soundBufferSize;
    desc.lpwfxFormat = &wavHdr->wfex;
    desc.guid3DAlgorithm.Data1 = 0;
    desc.guid3DAlgorithm.Data2 = 0;
    desc.guid3DAlgorithm.Data3 = 0;
    memset(desc.guid3DAlgorithm.Data4, 0, sizeof(desc.guid3DAlgorithm.Data4));
    dsndif = handles->dsndif;
    // preserved: both allocations leak when either call fails
    if (dsndif->lpVtbl->CreateSoundBuffer(dsndif, &desc, &(track->dsndSBIF), 0) != S_OK
        || track->dsndSBIF->lpVtbl->QueryInterface(track->dsndSBIF, &IID_IDirectSoundNotify, &(misc->vmilenEventHandlptr_Sub49e950)) != S_OK)
    {
        return 0;
    }
    pcSND_AddToSNDRaster(track->dsndSBIF, misc->soundBufferSize);
    track->format = desc.lpwfxFormat->nChannels;
    track->unk2SetTo64InCreate = 64;
    track->miscData = misc;
    misc->eventHandle1 = (HANDLE)-1;
    misc->eventHandle2 = (HANDLE)-1;
    misc->dsndEventsArrayIdx = trackEventsArrayNextIdx;
    trackEventsArrayNextIdx = misc->dsndEventsArrayIdx + 1;
    misc->fileHandle = CreateFileA(BIG_gst.filePath, 0x80000000, 3u, 0, 3u, 0x10000080u, 0);
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
    pcSND_SNDRasterRelease(stereo->dsndSBIF);
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
    BYTE fxSetting[4];

    SND_gst_FxParam.i7 = 16;
    SND_gst_FxDefaultParam.i7 = 16;
    SND_gst_FxDefaultParam.i8 = (tSndSoundBufferElem**)MEM_p_Alloc(64u);
    SND_gst_FxParam.i8 = (tSndSoundBufferElem**)MEM_p_Alloc(4 * SND_gst_FxParam.i7);
    memset(SND_gst_FxParam.i8, 0, 4 * SND_gst_FxParam.i7);
    fxWetVol = 0.0;
    memset(&gFXGlobal, 0, sizeof(gFXGlobal));
    if (pcSND_ReadFXSettingFromRegistry(fxSetting))
        gFXGlobal.fxEnabled = 0;
    else
        gFXGlobal.fxEnabled = *(uint32_t*)fxSetting != 1;
    return 0;
}

void pcSND_FxClose()
{
    if (SND_gst_FxParam.i8)
        MEM_Free(SND_gst_FxParam.i8);
    SND_gst_FxParam.i8 = 0;
    if (SND_gst_FxDefaultParam.i8)
        MEM_Free(SND_gst_FxDefaultParam.i8);
    SND_gst_FxDefaultParam.i8 = 0;
}

void pcSND_fxStructSet(EAXLISTENERPROPERTIES* dest, int preset)
{
    EAXLISTENERPROPERTIES props;

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
    memcpy(dest, &props, sizeof(EAXLISTENERPROPERTIES));
}

int pcSND_EAXListenerPropertiesSet(int preset)
{
    EAXLISTENERPROPERTIES props;

    pcSND_fxStructSet(&props, preset);
    ppdsndIKSPropertySet->lpVtbl->Set(ppdsndIKSPropertySet, &DSPROPSETID_EAX30_ListenerProperties, 1, 0, 0, (LPVOID*)&props, sizeof(EAXLISTENERPROPERTIES));
    return 0;
}

void pcSND_DummyFXFunc(tSoundFxParams* params)
{
    int count;
    int i;

    if (gFXGlobal.fxEnabled && (params->flags & 1) != 0 && params->i7 > 0)
    {
        count = params->i7;
        for (i = 0; i < count; ++i)
        {
            if (params->i8[i]) {
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
        for (i = 0; i < params->i7; ++i)
        {
            sbe = params->i8[i];
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
        for (i = 0; i < params->i7; ++i)
        {
            if (params->i8[i])
                pcSND_DisableReverbAndRestartPlaying(params, params->i8[i]);
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
        if (track->unk1)
        {
            vol = SND_f_GetVolFromAtt(att) * track->f1;
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
        sbe->dsndSBIF->lpVtbl->GetCurrentPosition(sbe->dsndSBIF, &playPos, 0); // !!!!!!!!!!!!!!!!!!!!!!!!a3a
        if (playPos >= misc->currentPlayPosition)
            *currPos = playPos + misc->currSize - misc->currentPlayPosition;
        else
            *currPos = playPos + misc->soundBufferSize - misc->currentPlayPosition + misc->currSize;
    }
    else
    {
        sbe->dsndSBIF->lpVtbl->GetCurrentPosition(sbe->dsndSBIF, currPos, writePos);
    }
}

void pcSND_flipPlayStopState(tSndSoundBufferElem* sbe)
{
    int status;

    sbe->dsndSBIF->lpVtbl->GetStatus(sbe->dsndSBIF, &status);
    if ((status & 1) != 0)
    {
        sbe->miscData->Stopped_SetIN49e270 = 1;
        sbe->dsndSBIF->lpVtbl->Stop(sbe->dsndSBIF);
        do
            sbe->dsndSBIF->lpVtbl->GetStatus(sbe->dsndSBIF, &status);
        while ((status & 1) != 0);
    }
    else
    {
        sbe->miscData->Stopped_SetIN49e270 = 0;
        sbe->dsndSBIF->lpVtbl->Play(sbe->dsndSBIF, 0, 0, 1);
    }
}

void pcSND_SB_Pause(tSndSoundBufferElem* sbe)
{
    int idx;
    IDirectSoundBuffer* buffer;
    int status;

    if (sbe->miscData) {
        pcSND_flipPlayStopState(sbe);
        return;
    }
    sbe->dsndSBIF->lpVtbl->GetStatus(sbe->dsndSBIF, &status);
    if ((status & 1) != 0) {
        sbe->dsndSBIF->lpVtbl->Stop(sbe->dsndSBIF);
        return;
    }
    for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
    {
        if (SND_gst_Params.soundInstances[idx].soundBufElemFromModel == sbe)
            break;
    }
    buffer = sbe->dsndSBIF;
    // preserved: with no matching instance the index is one past the array
    buffer->lpVtbl->Play(buffer, 0, 0, (SND_gst_Params.soundInstances[idx].flags >> 23) & 1);
}

void pcSND_D3DSoundBufAAndIKPopertySetRelease(tSndSoundBufferElem* sbe)
{
    IDirectSound3DBuffer* buffer3d;
    IKsPropertySet* propertySet;

    buffer3d = sbe->dsnd3dbufferIF;
    if (buffer3d)
    {
        buffer3d->lpVtbl->Release(sbe->dsnd3dbufferIF);
        propertySet = sbe->dsndIKPropertySetIF;
        if (propertySet)
            propertySet->lpVtbl->Release(sbe->dsndIKPropertySetIF);
    }
}

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
            pcSND_D3DSoundBufAAndIKPopertySetRelease(sbe);
            pcSND_SNDRasterRelease(sbe->dsndSBIF);
            sbe->dsndSBIF->lpVtbl->Release(sbe->dsndSBIF);
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
    if (!MAI_gh_MainWindow)
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
    SND_gpst_Interface.stream.Release = pcSND_Stream_Release;
    SND_gpst_Interface.stream.SetFrequency = pcSND_Stream_SetFrequency;
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
