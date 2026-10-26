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

int pcSND_GetFXSettingsFromRegistry_sub_49FD00(LPBYTE lpData)
{
    DWORD cbData;
    HKEY phkResult;
    DWORD Type;

    if (RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\IRISEng\\Sound",
        0,
        0x20019u,
        &phkResult))
    {
        return 1;
    }
    cbData = 4;
    if (RegQueryValueExA(phkResult, "Sound FX", 0, &Type, lpData, &cbData) || cbData != 4 || Type != 4)
        return 1;
    RegCloseKey(phkResult);
    return 0;
}

int pcSND_checkAudioBoardGuid_sub_49FD80(LPBYTE lpData)
{
    DWORD v1;
    HKEY phkResult;
    DWORD cbData;
    DWORD Type;
    CHAR Buffer[1000];

    if (!RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\IRISEng\\Sound",
        0,
        1u,
        &phkResult))
    {
        cbData = 16;
        v1 = RegQueryValueExA(phkResult, "Audio board GUID", 0, &Type, lpData, &cbData);
        if (v1)
        {
            GetLastError();
            FormatMessageA(0x1000u, 0, v1, 0, Buffer, 1000u, 0);
        }
        else if (cbData == 16 && Type == 3)
        {
            RegCloseKey(phkResult);
            return 0;
        }
    }
    return 1;
}

HRESULT pcSND_EAXDSoundCreate_sub_49FE30(GUID* guid, LPDIRECTSOUND8* dsoundptr, IUnknown* ptr)
{
    snd_eaxDLLHandle = LoadLibraryA("eax.dll");
    pEAXDirectSoundCreate8 = (PEAXDirectSoundCreate8)GetProcAddress(snd_eaxDLLHandle, "EAXDirectSoundCreate8");
    return pEAXDirectSoundCreate8(guid, dsoundptr, ptr);
}

int pcSND_probablyDecompressADPCM_sub_4A0130(char* snddata, __int16* destPtr, int blockNum, int channelNum)
{
    pcSND_ADPCMToPCM_Convert(
        (uint8_t*)snddata,
        destPtr,
        60 * blockNum,
        channelNum,
        channelNum != 1 ? 72 : 36,
        60u);
    return 1;
}

void pcSND_AddSoundToThePlayList(BOOL a1)
{
    int i;
    size_t v2;
    HANDLE* v3;

    for (i = 0; i < sndEventsNum; ++i)
    {
        if (!sndPlayList[i].playListElemInUse)
            break;
    }
    if (i >= sndEventsNum)
    {
        v2 = 8 * (sndEventsNum + 200);
        if (sndEventsNum)
        {
            sndPlayList = (tPcSndEventRelated*)MEM_p_Realloc(sndPlayList, v2);
            v3 = (HANDLE*)MEM_p_Realloc(sndPlayList, v2);
        }
        else
        {
            sndPlayList = (tPcSndEventRelated*)MEM_p_Alloc(v2);
            v3 = (HANDLE*)MEM_p_Alloc(v2);
        }
        sndEventHandles = v3;
        memset(&sndPlayList[sndEventsNum], 0, sizeof(tPcSndEventRelated) * 200);
        memset(&sndEventHandles[2 * sndEventsNum], 0, 3200u); //TODO ez hogy jön ki? 2x4x200 az csak 1600
        i = sndEventsNum;
        sndEventsNum += 200;
    }
    sndPlayList[i].playListElemInUse = 1;
    sndPlayList[i].soundElem = sndCurrSoundBufferElem;
    sndEventHandles[2 * i] = sndCurrSoundBufferElem->miscData->eventHandle1;
    sndEventHandles[2 * i + 1] = sndCurrSoundBufferElem->miscData->eventHandle2;
    sndEventHandleNum += 2;
    if (a1)
        SetEvent(dsndEvents[5]);
}

void pcSND_RemoveSoundFromPlayList(tSndSoundBufferElem* a1, int fireEvent)
{
    int v2;
    int v3;

    if (a1 == (tSndSoundBufferElem*)-1)
    {
        sndEventHandleNum = 0;
        if (!fireEvent)
            return;
        goto LABEL_13;
    }
    v2 = 0;
    if ((sndEventHandleNum & 0xFFFFFFFE) != 0)
    {
        while (sndPlayList[v2].soundElem != a1)
        {
            if (++v2 >= sndEventHandleNum >> 1)
                goto LABEL_12;
        }
        v3 = v2 + 1;
        if (v2 != -1)
        {
            for (; v3 < sndEventHandleNum >> 1; ++v3)
            {
                sndPlayList[v2].soundElem = sndPlayList[v3].soundElem;
                sndEventHandles[2 * v2] = sndEventHandles[2 * v3];
                sndEventHandles[2 * v2++ + 1] = sndEventHandles[2 * v3 + 1];
            }
            sndPlayList[v2].soundElem = 0;
            sndPlayList[v2].playListElemInUse = 0;
            sndEventHandles[2 * v2] = 0;
            sndEventHandles[2 * v2 + 1] = 0;
            sndEventHandleNum -= 2;
        }
    }
LABEL_12:
    ResetEvent(a1->miscData->eventHandle1);
    a1->miscData->eventHandle1 = 0;
    ResetEvent(a1->miscData->eventHandle2);
    a1->miscData->eventHandle2 = 0;
    if (fireEvent)
        LABEL_13:
    SetEvent(dsndEvents[5]);
}

void pcSND_StopSoundAndRemoveFromThePlayList(tSndSoundBufferElem* a1, int a2)
{
    a1->dsndSBIF->lpVtbl->Stop(a1->dsndSBIF);
    pcSND_RemoveSoundFromPlayList(a1, a2);
}

void pcSND_updateCurrentPlayPositionAndSize(tSndSoundBufferElem* a1)
{
    tSndSoundBufferElem* v1;
    tSbeMiscData* v2;
    int v3;
    int v4;
    int a1a;

    v1 = a1;
    a1->dsndSBIF->lpVtbl->GetCurrentPosition(a1->dsndSBIF, &a1a, 0);
    v2 = v1->miscData;
    v3 = v2->currentPlayPosition;
    if (a1a >= v3)
        v4 = a1a - v3;
    else
        v4 = a1a + v2->soundBufferSize - v3;
    v2->currSize += v4;
    v2->currentPlayPosition = a1a;
}

unsigned int pcSND_CalcSizeToReadByFormat(unsigned int a1, unsigned int a2)
{
    unsigned int v2;
    unsigned int result;

    v2 = (a1 >> 1) / a2;
    if (a2 == 1)
        result = 36 * (v2 / 60);
    else
        result = 72 * (v2 / 60);
    return result;
}

unsigned int pcSND_calcBlockNumForDecompress(int size, int channels)
{
    unsigned int ret;

    if (channels == 1)
        ret = size / 36;
    else
        ret = size / 72;
    return ret;
}

unsigned int pcSND_calcMemoryNeededForTheSound(unsigned int fullsoundlen, int channels)
{
    unsigned int result;

    if (channels == 1)
        result = 120 * (fullsoundlen / 36);
    else
        result = 120 * channels * (fullsoundlen / 72);
    return result;
}

void pcSND_ReadAndDecompressSoundData(unsigned int a1, tSndSoundBufferElem* sbe, int TmpBufferToSelect, uint32_t* totalSizeAfterDecompress, int16_t* destRaw)
{
    tSbeMiscData* v6;
    unsigned int v7;
    int blockNum;
    DWORD v9;
    __int16* v10;
    __int16* v11;
    DWORD i;
    int v13;
    int v14;
    unsigned int v15;
    uint16_t* v16;
    int v17;
    void* lpBuffer;

    if (TmpBufferToSelect)
        lpBuffer = dsndReadBuffer1;
    else
        lpBuffer = dsndReadBuffer2;
    v6 = sbe->miscData;
    if (v6->fullSize - v6->sumSize < a1)
        a1 = v6->fullSize - v6->sumSize;
    v7 = pcSND_CalcSizeToReadByFormat(a1, sbe->format);
    memset(lpBuffer, 0, 307200u);
    ReadFile(sbe->miscData->fileHandle, lpBuffer, v7, totalSizeAfterDecompress, 0);
    v17 = sbe->format;
    blockNum = pcSND_calcBlockNumForDecompress(v7, v17);
    pcSND_probablyDecompressADPCM_sub_4A0130((char*)lpBuffer, destRaw, blockNum, v17);
    v9 = pcSND_calcMemoryNeededForTheSound(*totalSizeAfterDecompress, sbe->format);
    *totalSizeAfterDecompress = v9;
    sbe->miscData->sumSize += v9;
    if (sbe->format == 2 && SND_gst_Params.soundRenderMode == 1)// mono-ra alakítás?
    {
        v10 = destRaw;
        v11 = destRaw;
        for (i = 0; i < *totalSizeAfterDecompress; v11 = v16 + 1)
        {
            v13 = *v10;
            v14 = v10[1];
            v10 += 2;
            v15 = (unsigned int)(((double)v13 + (double)v14) * 0.5);
            *v11 = v15;
            v16 = v11 + 1;
            *v16 = v15;
            i += 4;
        }
    }
}

int WINAPI pcSND_SoundThread(LPVOID lpThreadParameter)
{
    DWORD v1;
    char v2;
    tSndSoundBufferElem* sbe;
    tSbeMiscData* v4;
    IDirectSoundBuffer* dsndsbif;
    DWORD v6;
    unsigned int v7;
    unsigned int v8;
    char* v9;
    unsigned int v10;
    __int16* v11;
    unsigned int audioBytes1;
    char* audioPtr1;
    DWORD totalSizeAfterDecompress;
    LONG DistanceToMoveHigh;

    DistanceToMoveHigh = 0;
    while (1)
    {
        do
        {
            v1 = WaitForMultipleObjects(sndEventHandleNum, sndEventHandles, 0, 0xFFFFFFFF);
            v2 = (char)v1;
        } while (v1 == -1);

        switch (v1)
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
            sbe = sndPlayList[v1 >> 1].soundElem;
            v4 = sbe->miscData;
            if (v4->Stopped_SetIN49e270 != 1)
            {
                pcSND_updateCurrentPlayPositionAndSize(sndPlayList[v1 >> 1].soundElem);
                if (v4->RemoveAfterPlayFinished && v4->currSize >= v4->fullSize)
                    v4->shouldBeRemovedFromPlayList = 1;
                if (v4->shouldBeRemovedFromPlayList == 1 || (v2 & 1) != 0)
                {
                    pcSND_StopSoundAndRemoveFromThePlayList(sbe, 0);
                }
                else
                {
                    dsndsbif = sbe->dsndSBIF;
                    if (v4->RemoveAfterPlayFinished)
                    {
                        dsndsbif->lpVtbl->Lock(dsndsbif, v4->currOffset, v4->sizeToUpload, &audioPtr1, &audioBytes1, 0, 0, 0);
                        memset(audioPtr1, 0, audioBytes1);
                        sbe->dsndSBIF->lpVtbl->Unlock(sbe->dsndSBIF, audioPtr1, audioBytes1, 0, 0);
                    }
                    else
                    {
                        dsndsbif->lpVtbl->Lock(dsndsbif, v4->currOffset, v4->sizeToUpload, &audioPtr1, &audioBytes1, 0, 0, 0);
                        totalSizeAfterDecompress = 0;
                        pcSND_ReadAndDecompressSoundData(audioBytes1, sbe, 1, &totalSizeAfterDecompress, (int16_t*)audioPtr1);
                        v6 = totalSizeAfterDecompress;
                        v7 = audioBytes1;
                        if (totalSizeAfterDecompress < audioBytes1)
                        {
                            if (v4->readThroughWithMultipleLoop)
                            {
                                do
                                {
                                    SetFilePointer(v4->fileHandle, v4->distanceToMove, &DistanceToMoveHigh, 0);
                                    v11 = (int16_t*)&audioPtr1[v6];
                                    v8 = audioBytes1 - v6;
                                    v4->sumSize = 0;
                                    pcSND_ReadAndDecompressSoundData(v8, sbe, 1, &totalSizeAfterDecompress, v11);
                                    v6 += totalSizeAfterDecompress;
                                    v7 = audioBytes1;
                                } while (v6 < audioBytes1);
                            }
                            else
                            {
                                v9 = &audioPtr1[totalSizeAfterDecompress];
                                v10 = audioBytes1 - totalSizeAfterDecompress;
                                v4->RemoveAfterPlayFinished = 1;
                                memset(v9, 0, v10);
                                v7 = audioBytes1;
                            }
                        }
                        sbe->dsndSBIF->lpVtbl->Unlock(sbe->dsndSBIF, audioPtr1, v7, 0, 0);
                        v4->currOffset += audioBytes1;
                        if (v4->currOffset >= v4->soundBufferSize)
                            v4->currOffset -= v4->soundBufferSize;
                    }
                }
            }
            break;
        }
    }
    return 0;
}

void pcSND_StartThread_sub_49EDA0()
{
    HANDLE* v0;
    unsigned int i;

    if (!SND_gc_NoSound)
    {
        pcSNDThreadId = 0;
        pcSNDhObject = NULL;
        sndCurrSoundBufferElem = 0;
        trackEventsArrayNextIdx = 0;
        dsndReadBuffer1 = malloc(307200u);
        dsndReadBuffer2 = malloc(307200u);
        v0 = dsndEvents;
        do
            *v0++ = CreateEventA(NULL, FALSE, FALSE, NULL);
        while (v0 <= &dsndEvents[22]);
        sndEventsNum = 23;
        sndPlayList = (tPcSndEventRelated*)MEM_p_Alloc(sizeof(tPcSndEventRelated) * 23);
        memset(sndPlayList, 0, 8 * sndEventsNum);
        sndEventHandles = (HANDLE*)MEM_p_Alloc(8 * sndEventsNum);
        memset(sndEventHandles, 0, 8 * sndEventsNum);
        sndEventHandleNum = 4;
        for (i = 0; i < 16; i += 8)
        {
            sndEventHandles[i / 4] = dsndEvents[i / 4];
            sndEventHandles[i / 4 + 1] = dsndEvents[i / 4 + 1];
            sndPlayList[i / 8].playListElemInUse = 1;
            sndPlayList[i / 8].soundElem = 0;
        }
        pcSNDhObject = CreateThread(0, 0, pcSND_SoundThread, 0, 0, &pcSNDThreadId);
    }
}

int pcSND_CreateSoundBuffer(IDirectSoundBuffer *a1, IDirectSound8* dsndif)
{
    IDirectSound8* v2;
    WAVEFORMATEX v4;
    DSBUFFERDESC v5;
    ULONG a1a;

    a1->lpVtbl->QueryInterface(a1, &IID_IDirectSound3DListener, &dsndIDisectSound3DListenerIF);
    v4.wFormatTag = 0;
    v4.nChannels = 0;
    v4.nBlockAlign = 0;
    v4.wBitsPerSample = 0;
    memset(&v5, 0, sizeof(v5));
    v4.cbSize = 0;
    v5.guid3DAlgorithm.Data1 = 0;
    v5.lpwfxFormat = &v4;
    v4.nAvgBytesPerSec = 10000;
    v4.nSamplesPerSec = 10000;
    v5.dwBufferBytes = 10000;
    memset(v5.guid3DAlgorithm.Data4, 0, sizeof(v5.guid3DAlgorithm.Data4));
    v5.guid3DAlgorithm.Data2 = 0;
    v5.guid3DAlgorithm.Data3 = 0;
    v2 = dsndif;
    v4.nBlockAlign = 1;
    v4.nChannels = 1;
    v4.wBitsPerSample = 8;
    v4.wFormatTag = 1;
    v5.dwSize = 36;
    v5.dwFlags = 0x90;
    v2->lpVtbl->CreateSoundBuffer(dsndif, &v5, &d3dSndBuffer, 0);
    d3dSndBuffer->lpVtbl->QueryInterface(d3dSndBuffer, &IID_IDirectSound3DBuffer, &ppdsndDSND3DBuffer);
    d3dSndBuffer->lpVtbl->QueryInterface(d3dSndBuffer, &IID_IKsPropertySet, &ppdsndIKSPropertySet);
    if (ppdsndIKSPropertySet->lpVtbl->QuerySupport(ppdsndIKSPropertySet,  &DSPROPSETID_EAX30_ListenerProperties, 1, &a1a) < 0
        || (pcSNDEAXLisenerPropertiesSetIsOk = 1, ((uint8_t)a1a & 3) != 3))
    {
        pcSNDEAXLisenerPropertiesSetIsOk = 0;
    }
    return 0;
}

BOOL pcSND_CreateSoundBufferAndPlayTest(tSndHandles* a1)
{
    IDirectSoundBuffer** v1;
    IDirectSound8* dsndif;
    BOOL result;
    IDirectSoundBuffer* v4;
    WAVEFORMATEX v5;
    DSBUFFERDESC v6;

    memset(&v6, 0, sizeof(v6));
    v6.guid3DAlgorithm.Data1 = 0;
    memset(v6.guid3DAlgorithm.Data4, 0, sizeof(v6.guid3DAlgorithm.Data4));
    v1 = &a1->SoundBuffer;
    dsndif = a1->dsndif;
    v6.dwSize = 36;
    v6.dwFlags = 17;
    v6.dwBufferBytes = 0;
    v6.lpwfxFormat = 0;
    v6.guid3DAlgorithm.Data2 = 0;
    if (dsndif->lpVtbl->CreateSoundBuffer(dsndif, &v6, &(a1->SoundBuffer), 0) < 0)
        goto LABEL_2;
    v5.wFormatTag = 0;
    v5.nChannels = 0;
    v5.nSamplesPerSec = 0;
    v5.nAvgBytesPerSec = 0;
    v5.nBlockAlign = 0;
    v5.wBitsPerSample = 0;
    v5.cbSize = 0;
    v4 = *v1;
    v5.nSamplesPerSec = 48000;
    v5.wBitsPerSample = 16;
    v5.nChannels = 2;
    v5.nBlockAlign = 4;
    v5.nAvgBytesPerSec = 192000;
    v5.cbSize = 0;
    v5.wFormatTag = 1;
    if (v4->lpVtbl->SetFormat(v4, &v5) != S_OK || pcSND_CreateSoundBuffer(*v1, a1->dsndif))
    {
    LABEL_2:
        result = 0;
    }
    else
    {
        result = (*v1)->lpVtbl->Play(*v1, 0, 0, 1) == 0;
    }
    return result;
}

int pcSND_l_Init(tSndHandles* sndh)
{
    IDirectSound8** dnsif;
    BYTE Data[4];
    BYTE v4[16];

    BAS_binit(&dsndSBList, 48);
    memset(&gDoSoundGlobal, 0, sizeof(gDoSoundGlobal));
    pcSNDhObject = NULL;
    gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 = 0;
    sndh->MainWindowHandle = MAI_gh_MainWindow;
    if (CoInitialize(NULL) != S_OK || pcSND_GetFXSettingsFromRegistry_sub_49FD00(Data) || pcSND_checkAudioBoardGuid_sub_49FD80(v4))
        return 0;
    dnsif = &sndh->dsndif;
    if (pcSND_EAXDSoundCreate_sub_49FE30((GUID*)v4, &sndh->dsndif, 0) != S_OK)
        return 0;
    gDoSoundGlobal.dsndCaps.dwSize = 96;
    (*dnsif)->lpVtbl->GetCaps(*dnsif, &(gDoSoundGlobal.dsndCaps));
    (*dnsif)->lpVtbl->SetCooperativeLevel(*dnsif, sndh->MainWindowHandle, 3);
    if (!pcSND_CreateSoundBufferAndPlayTest(sndh))
        return 0;
    pcSND_StartThread_sub_49EDA0();
    return 1;
}

int pcSND_i_ChangeRenderMode(int vol)
{
    int i;
    tSndSoundBufferElem* v2;
    int v3;
    int v4;
    int pan;
    int v6;
    int v7;
    int v8;
    unsigned int v9;

    SND_gst_Params.soundRenderMode = vol;
    for (i = 0; i < dsndSBList.actElemNum; ++i)
    {
        v2 = (tSndSoundBufferElem*)dsndSBList.keyValPtr[i].key;
        if (v2 != (tSndSoundBufferElem*)-1)
        {
            v2->dsndSBIF->lpVtbl->GetVolume(v2->dsndSBIF, &vol);
            v3 = v2->unk2SetTo64InCreate;
            if (SND_gst_Params.soundRenderMode == 1)
            {
                if (v3 <= 64)
                {
                    if (v3 >= 64)
                        v6 = vol - gpcSNDVolumeAdjustmentArr[64];
                    else
                        v6 = vol - gpcSNDVolumeAdjustmentArr[v3];
                    pan = 0;
                    vol = gpcSNDVolumeAdjustmentArr[64] + v6;
                }
                else
                {
                    v4 = vol - gpcSNDVolumeAdjustmentArr[127 - v3];
                    pan = 0;
                    vol = gpcSNDVolumeAdjustmentArr[64] + v4;
                }
            }
            else
            {
                v7 = gpcSNDVolumeAdjustmentArr[64];
                vol -= gpcSNDVolumeAdjustmentArr[64];
                if (v3 <= 64)
                {
                    if (v3 >= 64)
                    {
                        pan = 0;
                    }
                    else
                    {
                        v9 = 4 * v3;
                        v7 = gpcSNDVolumeAdjustmentArr[v9 / 4];
                        pan = gpcSNDVolumeAdjustmentArr[127 - (v9 / 4)] - v7;
                    }
                    vol += v7;
                }
                else
                {
                    v8 = gpcSNDVolumeAdjustmentArr[127 - v3];
                    pan = v8 - gpcSNDVolumeAdjustmentArr[v3];
                    vol += v8;
                }
            }
            v2->dsndSBIF->lpVtbl->SetPan(v2->dsndSBIF, pan);
            v2->dsndSBIF->lpVtbl->SetVolume(v2->dsndSBIF, vol);
        }
    }
    return 0;
}

int pcSND_SetDefaultSoundPositionAndDistance(tSndSoundBufferElem* a1)
{
    IDirectSound3DBuffer** v1;
    void** v1b;

    v1 = &a1->dsnd3dbufferIF;
    v1b = v1; // TODO FIXME: ezek így biztosan jók?
    if (SUCCEEDED(a1->dsndSBIF->lpVtbl->QueryInterface(a1->dsndSBIF, &IID_IDirectSound3DBuffer, &a1->dsnd3dbufferIF)))
    {
        (*v1)->lpVtbl->SetMinDistance(*v1, 0x40000000, 0);
        (*v1)->lpVtbl->SetMaxDistance(*v1, 0x40400000, 0);
        (*v1)->lpVtbl->SetPosition(*v1, 0, 0, 0x3F800000, 0);
        v1b = &a1->dsndIKPropertySetIF;
        if (SUCCEEDED(a1->dsndSBIF->lpVtbl->QueryInterface(a1->dsndSBIF, &IID_IKsPropertySet, &a1->dsndIKPropertySetIF)))
            return 0;
    }
    *v1b = NULL;
    return -1;
}

int pcSND_SNDPropertySetRoomReverb(tSndSoundBufferElem* a1, int a2)
{
    IKsPropertySet** v2;
    int result;

    a2 = a2 != 0 ? 0 : -10000;
    v2 = &(a1->dsndIKPropertySetIF);
    if (v2 && ((*v2)->lpVtbl->Set(*v2, &DSPROPSETID_EAX30_BufferProperties, DSPROPERTY_EAXBUFFER_ROOM, 0, 0, (LPVOID*)&a2, 4) != S_OK))
        result = -1;
    else
        result = 0;
    return result;
}

void pcSND_DisableReverbAndRestartPlaying(tSoundFxParams* a1, tSndSoundBufferElem* a2)
{
    IDirectSoundBuffer* v3;
    unsigned int v4;

    if (pcSNDFXEnabled_dword_B30400)
    {
        a2->dsndSBIF->lpVtbl->GetStatus(a2->dsndSBIF, &v4);
        if ((v4 & 1) != 0)
            a2->dsndSBIF->lpVtbl->Stop(a2->dsndSBIF);
        pcSND_SNDPropertySetRoomReverb(a2, 0);
        if ((v4 & 1) != 0)
        {
            v3 = a2->dsndSBIF;
            v4 = (v4 >> 2) & 1;
            v3->lpVtbl->Play(v3, 0, 0, v4);
        }
    }
}

int pcSND_DisableReverbIf_i7_not_match_with_idx(tSoundFxParams* a1, tSndSoundBufferElem* a2)
{
    int v2;
    int v3;
    tSndSoundBufferElem** v4;

    v2 = a1->i7;
    v3 = 0;
    if (v2 > 0)
    {
        v4 = a1->i8;
        do
        {
            if (*v4 == a2)
                break;
            ++v3;
            ++v4;
        } while (v3 < v2);
    }
    if (v3 != v2)
    {
        pcSND_DisableReverbAndRestartPlaying(a1, a2);
        a1->i8[v3] = NULL;
    }
    return 0;
}

void pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(tSndSoundBufferElem* a1)
{
    if (a1->miscData->eventHandle2 != (HANDLE)-1)
    {
        sndCurrSoundBufferElem = a1;
        SetEvent(dsndEvents[3]);
        WaitForSingleObject(dsndEvents[5], 5000u);
    }
}

void pcSND_SB_Stop(tSndSoundBufferElem* a1)
{
    tSndSoundBufferElem* v1;

    v1 = a1;
    if (a1)
    {
        BAS_bdeletegetval((int)a1, &dsndSBList, (int *)&a1);
        if (v1->miscData)
        {
            if (a1)
                gDoSoundGlobal.dword_B303C0 -= v1->format;
            pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(v1);
        }
        else
        {
            if (a1)
                gDoSoundGlobal.sndConcurrentlyPlayedNum -= v1->format;
            pcSND_DisableReverbIf_i7_not_match_with_idx(&SND_gst_FxDefaultParam, v1);
            v1->dsndSBIF->lpVtbl->Stop(v1->dsndSBIF);
        }
    }
}

void pcSND_stopSoundIfReadFinished(tSndSoundBufferElem* a1)
{
    tSbeMiscData* v1;

    v1 = a1->miscData;
    if (v1 && !v1->readThroughWithMultipleLoop && v1->currSize >= v1->fullSize)
        pcSND_SB_Stop(a1);
}

int pcSND_SB_GetStatus(tSndSoundBufferElem* a1, int* a2)
{
    int result;

    if (!a1)
        return 0;
    if (a1->miscData)
        pcSND_stopSoundIfReadFinished(a1);
    result = a1->dsndSBIF->lpVtbl->GetStatus(a1->dsndSBIF, a2);
    if (a1->miscData)
    {
        if ((*a2 & 1) != 0)
            *a2 |= 0x10u;
    }
    return result;
}

int pcSND_SB_Play(tSndSoundBufferElem* a1, int a2)
{
    int v2;

    if (!a1)
        return -1;
    if (a1->miscData)
        return -1;
    v2 = gDoSoundGlobal.sndConcurrentlyPlayedNum + a1->format;
    if (v2 > 44)
        return -1;
    gDoSoundGlobal.sndConcurrentlyPlayedNum += a1->format;
    if (v2 > gDoSoundGlobal.sndConcurrentlyPlayedPeakNum)
        gDoSoundGlobal.sndConcurrentlyPlayedPeakNum = v2;
    if ((a1->dsndSBIF->lpVtbl->Play(a1->dsndSBIF, 0, 0, a2 & 1)) != S_OK)
        OutputDebugStringA("Voice not allocated in hardware");
    else
        OutputDebugStringA("HARDWARE voice\n");
    BAS_binsert((unsigned int)a1, (int)a1, &dsndSBList);
    return 0;
}

int sub_49CEF0()
{
    return 6000000 - sndRasterStat.i7;
}

void pcSND_AddToSNDRaster(IDirectSoundBuffer* a1, int a2)
{
    BOOL v2;

    if (SND_gst_Params.soundEnabled)
    {
        BAS_binsert((unsigned int)a1, a2, &rasterbas1);
        BAS_binsert((unsigned int)a1, SND_gi_RasterCurrentFileId, &rasterbas2);
        if (SND_gi_RasterCurrentFileId > 0x4000)
        {
            v2 = SND_gi_RasterCurrentFileId == 0x80000;
        }
        else
        {
            if (SND_gi_RasterCurrentFileId == 0x4000)
                goto LABEL_10;
            if (SND_gi_RasterCurrentFileId == 8)
            {
                sndRasterStat.i3 += a2;
            LABEL_11:
                sndRasterStat.i7 += a2;
                sndRasterStat.i8 = sub_49CEF0();
                return;
            }
            v2 = SND_gi_RasterCurrentFileId == 0x2000;
        }
        if (!v2)
        {
            sndRasterStat.i5 += a2;
            goto LABEL_11;
        }
    LABEL_10:
        sndRasterStat.i1 += a2;
        goto LABEL_11;
    }
}

tSndSoundBufferElem* pcSND_pst_SB_Create(tSndHandles* a2, tSndWavHdrStruct* sndHdr, int sndStartPos)
{
    char* sndData;
    char* v5;
    int v6;
    unsigned int v7;
    char* uncompressedSnd;
    unsigned int v9;
    DWORD v10;
    tSndSoundBufferElem* sndSNDBuf;
    IDirectSound8* dsndIf;
    IDirectSoundBuffer* dsndSBIF;
    int v14;
    char* offset;
    int v16;
    char* v17;
    unsigned int v18;
    char* v19;
    int v20;
    DSBUFFERDESC dsBufferDesc;

    if (!a2->dsndif)
        return 0;
    sndData = 0;
    if (sndHdr->wfex.wFormatTag != 2)
        return 0;
    if (!LOA_gb_SpeedMode)
    {
        if (!sndStartPos)
            goto LABEL_14;
        sndData = (char*)MEM_p_Alloc(sndHdr->fullSoundSize);
        if (sndData)
            goto LABEL_13;
        return 0;
    }
    if (!sndStartPos)
        goto LABEL_14;
    sndData = (char*)MEM_p_Alloc(sndHdr->fullSoundSize);
    if (!sndData)
        return 0;
    if (!isSpeedModeAndBinFile())
    {
    LABEL_13:
        BIG_ReadNoSeek(sndStartPos, sndData, sndHdr->fullSoundSize);
        goto LABEL_14;
    }
    if (sndHdr->wfex.nChannels == 1)
    {
        v5 = LOA_FetchBuffer(sndHdr->fullSoundSize);
        v14 = sndHdr->fullSoundSize;
        offset = v5;
        LOA_ReadCharArray(&offset, sndData, v14, 1);
    }
    else
    {
        v6 = sndHdr->fullSoundSize;
        sndHdr->wfex.cbSize >>= 1;
        sndHdr->fullSoundSize = v6 >> 1;
        offset = LOA_FetchBuffer(v6 >> 1);
        LOA_ReadCharArray(&offset, sndData, sndHdr->fullSoundSize, 1);
        offset = LOA_FetchBuffer(sndHdr->fullSoundSize);
        LOA_ReadCharArray(&offset, &sndData[sndHdr->fullSoundSize], sndHdr->fullSoundSize, 1);
    }
LABEL_14:
    if (sndHdr->wfex.wFormatTag != 2)
    {
        if (sndData)
            MEM_Free(sndData);
        return 0;
    }
    if (sndStartPos)
    {
        v7 = pcSND_calcMemoryNeededForTheSound(sndHdr->fullSoundSize, sndHdr->wfex.nChannels);
        uncompressedSnd = (char*)MEM_p_Alloc(v7);
        memset(uncompressedSnd, 0, v7);
        pcSND_probablyDecompressADPCM_sub_4A0130(sndData, (__int16*)uncompressedSnd, sndHdr->fullSoundSize / sndHdr->wfex.nBlockAlign, sndHdr->wfex.nChannels);
        MEM_Free(sndData);
        sndData = uncompressedSnd;
    }
    v9 = pcSND_calcMemoryNeededForTheSound(sndHdr->fullSoundSize, sndHdr->wfex.nChannels);
    v10 = sndHdr->wfex.nSamplesPerSec;
    sndHdr->fullSoundSize = v9;
    v9 = 2 * sndHdr->wfex.nChannels;
    sndHdr->wfex.nBlockAlign = v9;
    sndHdr->wfex.wBitsPerSample = 16;
    sndHdr->wfex.wFormatTag = 1;
    sndHdr->wfex.cbSize = 0;
    sndHdr->wfex.nAvgBytesPerSec = (uint16_t)v9 * v10;
    v18 = 0;
    v16 = 0;
    sndSNDBuf = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(sndSNDBuf, 0, sizeof(tSndSoundBufferElem));
    sndSNDBuf->unk2SetTo64InCreate = 64;
    memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));
    dsBufferDesc.dwBufferBytes = sndHdr->fullSoundSize;
    dsBufferDesc.lpwfxFormat = &sndHdr->wfex;
    dsBufferDesc.guid3DAlgorithm.Data2 = 0;
    dsBufferDesc.guid3DAlgorithm.Data3 = 0;
    memset(dsBufferDesc.guid3DAlgorithm.Data4, 0, sizeof(dsBufferDesc.guid3DAlgorithm.Data4));
    dsndIf = a2->dsndif;
    dsBufferDesc.guid3DAlgorithm.Data1 = 0;
    dsBufferDesc.dwSize = 36;
    dsBufferDesc.dwFlags = 0x400B0;
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
    if ((sndSNDBuf->dsndSBIF->lpVtbl->Lock(sndSNDBuf->dsndSBIF, 0, sndHdr->fullSoundSize, &v19, &v18, &v17, &v20, 0)) != S_OK)
    {
        sndSNDBuf->dsndSBIF->lpVtbl->Release(sndSNDBuf->dsndSBIF);
        MEM_Free(sndSNDBuf);
        sndSNDBuf = NULL;
        MEM_Free(sndData);
        return sndSNDBuf;
    }
    memcpy(v19, sndData, v18);
    v14 = v20;
    if (v20)
    {
        memcpy(v17, &sndData[v18], (unsigned int)v20);
        v14 = v20;
    }
    sndSNDBuf->dsndSBIF->lpVtbl->Unlock(sndSNDBuf->dsndSBIF, v19, v18, v17, v14);
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
    HANDLE* v0;

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
        v0 = dsndEvents;
        do
        {
            if (*v0)
                CloseHandle(*v0);
            *v0++ = 0;
        } while (v0 <= &dsndEvents[22]);
        if (dsndReadBuffer1)
            free(dsndReadBuffer1);
        dsndReadBuffer1 = NULL;
        if (dsndReadBuffer2)
            free(dsndReadBuffer2);
        dsndReadBuffer2 = NULL;
    }
}

void pcSND_Close(tSndHandles* a1)
{
    pcSND_Cleanup();
    BAS_bfree(&dsndSBList);
    pcSND_DSoundBuffersRelease();
    a1->dsndif->lpVtbl->Release(a1->dsndif);
    CoUninitialize();
}

void pcSND_SB_Duplicate(tSndHandles* a1, tSndSoundBufferElem* a2, tSndSoundBufferElem** a3)
{
    tSndSoundBufferElem* v3;

    *a3 = NULL;
    if (!a2->miscData)
    {
        v3 = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
        *a3 = v3;
        if (v3)
        {
            memset(v3, 0, sizeof(tSndSoundBufferElem));
            a1->dsndif->lpVtbl->DuplicateSoundBuffer(a1->dsndif, a2->dsndSBIF, &((*a3)->dsndSBIF));
            (*a3)->format = a2->format;
            (*a3)->unk2SetTo64InCreate = 64;
            pcSND_SetDefaultSoundPositionAndDistance(*a3);
        }
    }
}

void pcSND_SB_GetFrequency(tSndSoundBufferElem* a1, int* a2)
{
    if (a1)
        a1->dsndSBIF->lpVtbl->GetFrequency(a1->dsndSBIF, a2);
}

void pcSND_SB_GetPan(tSndSoundBufferElem* a1, int* a2, int* a3)
{
    int v3;
    float a1a;
    float a1b;

    if (a1)
    {
        *a3 = 0;
        *a2 = 0;
        if (SND_gst_Params.soundRenderMode == 2)
        {
            v3 = a1->unk2SetTo64InCreate;
            if (v3 <= 64)
            {
                if (v3 >= 64)
                {
                    *a2 = 0;
                }
                else
                {
                    a1b = (float)((double)v3 * 0.015625);
                    *a2 = SND_l_GetAttFromVol(a1b);
                }
            }
            else
            {
                a1a = (float)((127.0 - (double)v3) * 0.015873017);
                *a2 = SND_l_GetAttFromVol(a1a);
            }
        }
    }
}

int pcSND_3DSoundBufferSetPos(tSndSoundBufferElem* a1, int a2)
{
    IDirectSound3DBuffer* v2; 
    long double v3;
    float v5;
    float v6;

    v2 = a1->dsnd3dbufferIF;
    if (v2)
    {
        v3 = (10000.0 - (double)a2) * 0.0089999996 * 0.017453292;
        v6 = (float)sin(v3);
        v5 = (float)cos(v3);
        v2->lpVtbl->SetPosition(v2, v5, v6, 0.0f, 0);
    }
    return 0;
}

void pcSND_SB_SetPan(tSndSoundBufferElem* arg1, int a1, int a2) // ehhez nagyon hasonló ez: pcSND_UnknownSoundPanAndVolumeSetByUnknown
{
    tSndSoundBufferElem* v2;
    int v3;
    int v4;
    int v5;
    unsigned int v6;
    //int v6a;
    int v7;
    int v8;
    unsigned int v9;
    unsigned int v10;
    int v11;
    int arg0;

    v2 = arg1;
    if (!arg1)
        return;
    arg1->dsndSBIF->lpVtbl->GetVolume(arg1->dsndSBIF, &arg0);
    v3 = v2->unk2SetTo64InCreate;
    if (v3 > 64)
    {
        v4 = gpcSNDVolumeAdjustmentArr[127 - v3];
    LABEL_7:
        v5 = arg0 - v4;
        goto LABEL_8;
    }
    if (v3 >= 64)
    {
        v4 = gpcSNDVolumeAdjustmentArr[64];
        goto LABEL_7;
    }
    v5 = arg0 - gpcSNDVolumeAdjustmentArr[v3];
LABEL_8:
    arg0 = v5;
    if (a1 <= 0)
    {
        if (a1 >= 0)
        {
            v2->unk2SetTo64InCreate = 64;
            v7 = gpcSNDVolumeAdjustmentArr[64];
            v8 = 0;
        }
        else
        {
            v9 = (unsigned int)(SND_f_GetVolFromAtt(a1) * 64.0);
            if ((v9 & 0x80000000) == 0)
            {
                if (v9 > 127)
                    v9 = 127;
            }
            else
            {
                v9 = 0;
            }
            v2->unk2SetTo64InCreate = v9;
            v10 = 4 * v9;
            v7 = gpcSNDVolumeAdjustmentArr[v10 / 4];
            v5 = arg0;
            v8 = gpcSNDVolumeAdjustmentArr[127 - (v10 / 4)] - v7;
        }
    }
    else
    {
        v6 = (unsigned int)(127.0 - SND_f_GetVolFromAtt(-a1) * 63.0);
        if ((v6 & 0x80000000) == 0)
        {
            if (v6 > 127)
                v6 = 127;
        }
        else
        {
            v6 = 0;
        }
        v2->unk2SetTo64InCreate = v6;
        // TODO FIXME!!!!!!!!!
        v7 = gpcSNDVolumeAdjustmentArr[127 - v6]; // v6a = gpcSNDVolumeAdjustmentArr[127 - v6]
        v5 = arg0;
        v8 = v7 - gpcSNDVolumeAdjustmentArr[v6]; //v8 = v6a - gpcSNDVolumeAdjustmentArr[v6];
    }
    v11 = v5 + v7;
    arg0 = v11;
    if (SND_gst_Params.soundRenderMode == 1)
    {
        v2->dsndSBIF->lpVtbl->SetPan(v2->dsndSBIF, 0);
    }
    else
    {
        if (v11 <= 0)
        {
            if (v11 < -10000)
                arg0 = -10000;
        }
        else
        {
            arg0 = 0;
        }
        if (v8 <= 10000)
        {
            if (v8 < -10000)
                v8 = -10000;
        }
        else
        {
            v8 = 10000;
        }
        pcSND_3DSoundBufferSetPos(v2, v8);
        if (v2->unknownFiledinsub49db40ToSetVolume != arg0)
            v2->dsndSBIF->lpVtbl->SetVolume(v2->dsndSBIF, arg0);
    }
}

void pcSND_SB_SetCurrPos(tSndSoundBufferElem* a1, int a2)
{
    if (a1)
        a1->dsndSBIF->lpVtbl->SetCurrentPosition(a1->dsndSBIF, a2);
}

void pcSND_SB_SetVolume(tSndSoundBufferElem* a1, int a2)
{
    int v2;
    int v3;
    int v4;
    int v5;
    IDirectSoundBuffer* v6;

    if (!a1)
        return;
    v2 = gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 + a2;
    if (SND_gst_Params.soundRenderMode != 2)
        goto LABEL_7;
    v3 = a1->unk2SetTo64InCreate;
    if (v3 > 64)
    {
        v4 = gpcSNDVolumeAdjustmentArr[127 - v3];
    LABEL_8:
        v5 = v4 + v2;
        goto LABEL_9;
    }
    if (v3 >= 64)
    {
    LABEL_7:
        v4 = gpcSNDVolumeAdjustmentArr[64];
        goto LABEL_8;
    }
    v5 = gpcSNDVolumeAdjustmentArr[v3] + v2;
LABEL_9:
    if (v5 <= 0)
    {
        if (v5 < -10000)
            v5 = -10000;
    }
    else
    {
        v5 = 0;
    }
    if (a1->unknownFiledinsub49db40ToSetVolume != v5)
    {
        v6 = a1->dsndSBIF;
        a1->unknownFiledinsub49db40ToSetVolume = v5;
        v6->lpVtbl->SetVolume(v6, v5);
    }
}

void pcSND_SNDRasterRelease(IDirectSoundBuffer* a1)
{
    BOOL v1;
    int a3;
    int v3;

    if (SND_gst_Params.soundEnabled)
    {
        BAS_bdeletegetval((int)a1, &rasterbas2, &a3);
        BAS_bdeletegetval((int)a1, &rasterbas1, &v3);
        if (a3 != -1 && v3 != -1)
        {
            if (a3 > 0x4000)
            {
                v1 = a3 == 0x80000;
            }
            else
            {
                if (a3 == 0x4000)
                    goto LABEL_12;
                if (a3 == 8)
                {
                    sndRasterStat.i3 -= v3;
                LABEL_13:
                    sndRasterStat.i7 -= v3;
                    sndRasterStat.i8 = sub_49CEF0();
                    return;
                }
                v1 = a3 == 0x2000;
            }
            if (!v1)
            {
                sndRasterStat.i5 -= v3;
                goto LABEL_13;
            }
        LABEL_12:
            sndRasterStat.i1 -= v3;
            goto LABEL_13;
        }
    }
}

void pcSND_MiscDataRelease(tSndSoundBufferElem* a1)
{
    tSbeMiscData* v1;
    IDirectSoundNotify* v2;
    BOOL v3;

    if (a1)
    {
        v1 = a1->miscData;
        if (v1)
        {
            pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(a1);
            if (v1->fileHandle != (HANDLE)-1)
                CloseHandle(v1->fileHandle);
            v2 = v1->vmilenEventHandlptr_Sub49e950;
            v3 = v1->vmilenEventHandlptr_Sub49e950 == 0;
            v1->fileHandle = (HANDLE)-1;
            if (!v3)
                v2->lpVtbl->Release(v2);
            v1->vmilenEventHandlptr_Sub49e950 = 0;
            if (a1->dsndSBIF)
            {
                pcSND_SNDRasterRelease(a1->dsndSBIF);
                a1->dsndSBIF->lpVtbl->Release(a1->dsndSBIF);
            }
            a1->dsndSBIF = 0;
            MEM_Free(a1->miscData);
            a1->miscData = 0;
        }
    }
}

void pcSND_ReleaseSoundBuffer(tSndSoundBufferElem* a1)
{
    tSndSoundBufferElem* v1;

    v1 = a1;
    if (a1)
    {
        BAS_bdeletegetval((int)a1, &dsndSBList, (int *)&a1);
        if (v1->miscData)
        {
            if (a1)
                gDoSoundGlobal.dword_B303C0 -= v1->format;
            pcSND_MiscDataRelease(v1);
            MEM_Free(v1);
        }
        else
        {
            if (a1)
                gDoSoundGlobal.sndConcurrentlyPlayedNum -= v1->format;
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
            pcSND_SNDRasterRelease(v1->dsndSBIF);
            v1->dsndSBIF->lpVtbl->Release(v1->dsndSBIF);
            MEM_Free(v1);
        }
    }
}

int pcSND_Return160()
{
    return 160;
}

tSndSoundBufferElem* pcSND_SoundReloadToDSound(tSndHandles* a2, tSndWavHdrStruct* a3, int Offset)
{
    char* v4;
    char* v5;
    int v6;
    unsigned int v7;
    char* v8;
    unsigned int v9;
    DWORD v10;
    tSndSoundBufferElem* v11;
    IDirectSound8* v12;
    IDirectSoundBuffer* v13;
    int v14;
    unsigned int v15;
    char* offset;
    char* v17;
    unsigned int v18;
    char* v19;
    int v20;
    DSBUFFERDESC v21;

    if (!a2->dsndif)
        return 0;
    v4 = 0;
    if (a3->wfex.wFormatTag != 2)
        return 0;
    if (!LOA_gb_SpeedMode)
    {
        if (!Offset)
            goto LABEL_14;
        v4 = (char*)MEM_p_Alloc(a3->fullSoundSize);
        if (v4)
            goto LABEL_13;
        return 0;
    }
    if (!Offset)
        goto LABEL_14;
    v4 = (char*)MEM_p_Alloc(a3->fullSoundSize);
    if (!v4)
        return 0;
    if (!isSpeedModeAndBinFile())
    {
    LABEL_13:
        BIG_ReadNoSeek(Offset, v4, a3->fullSoundSize);
        goto LABEL_14;
    }
    if (a3->wfex.nChannels == 1)
    {
        v5 = LOA_FetchBuffer(a3->fullSoundSize);
        v15 = a3->fullSoundSize;
        offset = v5;
        LOA_ReadCharArray(&offset, v4, v15, 1);
    }
    else
    {
        v6 = a3->fullSoundSize;
        a3->wfex.cbSize >>= 1;
        a3->fullSoundSize = v6 >> 1;
        offset = LOA_FetchBuffer(v6 >> 1);
        LOA_ReadCharArray(&offset, v4, a3->fullSoundSize, 1);
        offset = LOA_FetchBuffer(a3->fullSoundSize);
        LOA_ReadCharArray(&offset, &v4[a3->fullSoundSize], a3->fullSoundSize, 1);
    }
LABEL_14:
    if (a3->wfex.wFormatTag != 2)
    {
        if (v4)
            MEM_Free(v4);
        return 0;
    }
    if (Offset)                                 // itt mvi fv van ami nem látszik sub_4A0590, 
                                                  // kioptimalizálja az ida, mert vna egy vizsgálat != 2-re ami return 0-val zárul ellenkezõ esetben ellenõrzõdik 
                                                  // hogy == 0x69-el ami nyilván nem teljesül.
                                                  //  Az a kódrész akkor futna le.
    {
        v7 = pcSND_calcMemoryNeededForTheSound(a3->fullSoundSize, a3->wfex.nChannels);
        v8 = (char*)MEM_p_Alloc(v7);
        memset(v8, 0, v7);
        pcSND_probablyDecompressADPCM_sub_4A0130(v4, (__int16*)v8, a3->fullSoundSize / (int)a3->wfex.nBlockAlign, a3->wfex.nChannels);
        MEM_Free(v4);
        v4 = v8;
    }
    v9 = pcSND_calcMemoryNeededForTheSound(a3->fullSoundSize, a3->wfex.nChannels);
    v10 = a3->wfex.nSamplesPerSec;
    a3->fullSoundSize = v9;
    v9 = 2 * a3->wfex.nChannels;
    a3->wfex.nBlockAlign = v9;
    a3->wfex.wBitsPerSample = 16;
    a3->wfex.wFormatTag = 1;
    a3->wfex.cbSize = 0;
    a3->wfex.nAvgBytesPerSec = (uint16_t)v9 * v10;
    v19 = 0;
    v17 = 0;
    v11 = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(v11, 0, sizeof(tSndSoundBufferElem));
    v11->unk2SetTo64InCreate = 64;
    memset(&v21, 0, sizeof(v21));
    v21.dwSize = 36;
    v21.dwFlags = pcSND_Return160();
    v21.dwBufferBytes = a3->fullSoundSize;
    v21.lpwfxFormat = &a3->wfex;
    v21.guid3DAlgorithm.Data2 = 0;
    v21.guid3DAlgorithm.Data3 = 0;
    memset(v21.guid3DAlgorithm.Data4, 0, sizeof(v21.guid3DAlgorithm.Data4));
    v12 = a2->dsndif;
    v21.guid3DAlgorithm.Data1 = 0;
    if (v12->lpVtbl->CreateSoundBuffer(v12, &v21, &(v11->dsndSBIF), 0) != S_OK)
    {
        if (v4)
            MEM_Free(v4);
        MEM_Free(v11);
        return 0;
    }
    v13 = v11->dsndSBIF;
    v11->format = v21.lpwfxFormat->nChannels;
    pcSND_AddToSNDRaster(v13, v21.dwBufferBytes);
    if (!v4)
        return v11;
    if (v11->dsndSBIF->lpVtbl->Lock(v11->dsndSBIF, 0, a3->fullSoundSize, &v19, &v18, &v17, &v20, 0) != S_OK)
    {
        v11->dsndSBIF->lpVtbl->Release(v11->dsndSBIF);
        MEM_Free(v11);
        v11 = 0;
        MEM_Free(v4);
        return v11;
    }
    memcpy(v19, v4, v18);
    v14 = v20;
    if (v20)
    {
        memcpy(v17, &v4[v18], v20);
        v14 = v20;
    }
    v11->dsndSBIF->lpVtbl->Unlock(v11->dsndSBIF, v19, v18, v17, v14);
    MEM_Free(v4);
    return v11;
}

tSndSoundBufferElem* pcSND_SB_Reload(tSndHandles* a2, tSndSoundBufferElem* a1, tSndWavHdrStruct* a4, int Offset)
{
    tSndSoundBufferElem* v4; // esi

    v4 = pcSND_SoundReloadToDSound(a2, a4, Offset);
    if (v4)
        pcSND_ReleaseSoundBuffer(a1);
    return v4;
}

void pcSND_StreamPrefetchArray(int* a1, int* a2) //// @SND_StreamResolveSyncPrefetch
{
    if (a1)
        memset(a1, 0, 32u);
}

void pcSND_setCurrentSoundBuffElemAndSetAnEvent(tSndSoundBufferElem* this)
{
    tSbeMiscData* v1;

    v1 = this->miscData;
    if ((v1->eventHandle1 == (HANDLE)-1 || !v1->eventHandle1) && (v1->eventHandle2 == (HANDLE)-1 || !v1->eventHandle2))
    {
        v1->eventHandle1 = dsndEvents[v1->dsndEventsArrayIdx + 6];
        this->miscData->eventHandle2 = dsndEvents[this->miscData->dsndEventsArrayIdx + 15];
        sndCurrSoundBufferElem = this;
        SetEvent(dsndEvents[0]);
        WaitForSingleObject(dsndEvents[5], 5000);
    }
}

void pcSND_SetPositionNotify(tSndSoundBufferElem* this)
{
    tSbeMiscData* v1;
    DWORD v2; 
    int i;
    DSBPOSITIONNOTIFY v4[5];

    v1 = this->miscData;
    v2 = v1->sizeToUpload;
    pcSND_setCurrentSoundBuffElemAndSetAnEvent(this);
    v4[0].dwOffset = v2;
    v4[0].hEventNotify = v1->eventHandle1;
    for (i = 1; i < 4; ++i)
    {
        v4[i].dwOffset = v2 + v4[i - 1].dwOffset;
        v4[i].hEventNotify = v1->eventHandle1;
    }
    --v4[i - 1].dwOffset;
    v4[i].dwOffset = -1;
    v4[i].hEventNotify = v1->eventHandle2;
    v1->vmilenEventHandlptr_Sub49e950->lpVtbl->SetNotificationPositions(v1->vmilenEventHandlptr_Sub49e950, 5, v4);
}

void pcSND_fillSoundBuffer(int a1, tSbeMiscData* a2, tSndSoundBufferElem* a3)
{
    tSndSoundBufferElem* sbe;
    tSbeMiscData* v4;
    int v5;
    int v6;
    int v7;
    char *v8;
    DWORD totalSizeAfterDecompress;
    int audioBytes;
    char *DistanceToMoveHigh;
    int a3a;

    sbe = a3;
    v4 = a3->miscData;
    totalSizeAfterDecompress = 0;
    if (v4)
    {
        a3->dsndSBIF->lpVtbl->Lock(a3->dsndSBIF, 0, v4->soundBufferSize, &v8, &a3a, &DistanceToMoveHigh, &audioBytes, 2);
        v5 = pcSND_CalcSizeToReadByFormat(v4->unk6, sbe->format);
        SetFilePointer(v4->fileHandle, v4->distanceToMove + v5, &totalSizeAfterDecompress, 0);
        v4->sumSize = v4->unk6;
        pcSND_ReadAndDecompressSoundData(a3a, sbe, 0, &v7, (__int16*)v8);
        v6 = v7;
        if (v7 < a3a)
        {
            if (v4->readThroughWithMultipleLoop)
            {
                do
                {
                    SetFilePointer(v4->fileHandle, v4->distanceToMove, &totalSizeAfterDecompress, 0);
                    v4->sumSize = 0;
                    pcSND_ReadAndDecompressSoundData(a3a - v6, sbe, 0, &v7, (__int16*)(&v8[v6]));
                    v6 += v7;
                } while (v6 < a3a);
            }
            else
            {
                if (v7 < v4->sizeToUpload)
                {
                    v4->RemoveAfterPlayFinished = 1;
                    v6 = v7;
                }
                memset(&v8[v6], 0, a3a - v6);
            }
        }
        sbe->dsndSBIF->lpVtbl->Unlock(sbe->dsndSBIF, v8, a3a, 0, 0);
        v4->currOffset += a3a;
        if (v4->currOffset >= v4->soundBufferSize)
            v4->currOffset -= v4->soundBufferSize;
        v4->currSize = v4->unk6;
        v4->currentPlayPosition = 0;
    }
}

void pcSND_SB_SetFrequency(tSndSoundBufferElem* a1, int a2)
{
    int v2;

    if (a1)
    {
        v2 = a2;
        if (a2 <= 200000)
        {
            if (a2 < 100)
                v2 = 100;
            a1->dsndSBIF->lpVtbl->SetFrequency(a1->dsndSBIF, v2);
        }
        else
        {
            a1->dsndSBIF->lpVtbl->SetFrequency(a1->dsndSBIF, 200000);
        }
    }
}

void pcSND_UnknownSoundPanAndVolumeSetByUnknown(tSndSoundBufferElem* a1a, int a2)
{
    tSndSoundBufferElem* v2;
    int v3;
    int v4;
    int v5;
    unsigned int v6a;
    //unsigned int v6b;
    int v7;
    LONG v8;
    unsigned int v9;
    unsigned int v10;
    int v11;
    int a1;

    v2 = a1a;
    if (!a1a)
        return;
    a1a->dsndSBIF->lpVtbl->GetVolume(a1a->dsndSBIF, &a1);
    v3 = v2->unk2SetTo64InCreate;
    if (v3 > 64)
    {
        v4 = gpcSNDVolumeAdjustmentArr[-v3 + 127];
    LABEL_7:
        v5 = a1 - v4;
        goto LABEL_8;
    }
    if (v3 >= 64)
    {
        v4 = gpcSNDVolumeAdjustmentArr[64];
        goto LABEL_7;
    }
    v5 = a1 - gpcSNDVolumeAdjustmentArr[v3];
LABEL_8:
    a1 = v5;
    if (a2 <= 0)
    {
        if (a2 >= 0)
        {
            v2->unk2SetTo64InCreate = 64;
            v7 = gpcSNDVolumeAdjustmentArr[64];
            v8 = 0;
        }
        else
        {
            v9 = (unsigned int)(SND_f_GetVolFromAtt(a2) * 64.0);
            if ((v9 & 0x80000000) == 0)
            {
                if (v9 > 127)
                    v9 = 127;
            }
            else
            {
                v9 = 0;
            }
            v2->unk2SetTo64InCreate = v9;
            v10 = 4 * v9;
            v7 = gpcSNDVolumeAdjustmentArr[v10 / 4];
            v5 = a1;
            v8 = gpcSNDVolumeAdjustmentArr[127 - (v10 / 4)] - v7;
        }
    }
    else
    {
        v6a = (unsigned int)(127.0 - SND_f_GetVolFromAtt(-a2) * 63.0);
        if ((v6a & 0x80000000) == 0)
        {
            if (v6a > 127)
                v6a = 127;
        }
        else
        {
            v6a = 0;
        }
        v2->unk2SetTo64InCreate = v6a;
        // TODO FIXME!!!!!!!!!
        v7 = gpcSNDVolumeAdjustmentArr[127 - v6a]; // v6b = gpcSNDVolumeAdjustmentArr[127 - v6a]
        v5 = a1;
        v8 = v7 - gpcSNDVolumeAdjustmentArr[v6a]; //v8 = v6b - gpcSNDVolumeAdjustmentArr[v6a];
    }
    v11 = v5 + v7;
    a1 = v11;
    if (SND_gst_Params.soundRenderMode == 1)
    {
        v2->dsndSBIF->lpVtbl->SetPan(v2->dsndSBIF, 0);
    }
    else
    {
        if (v11 <= 0)
        {
            if (v11 < -10000)
                a1 = -10000;
        }
        else
        {
            a1 = 0;
        }
        if (v8 <= 10000)
        {
            if (v8 < -10000)
                v8 = -10000;
        }
        else
        {
            v8 = 10000;
        }
        v2->dsndSBIF->lpVtbl->SetPan(v2->dsndSBIF, v8);
        if (v2->unknownFiledinsub49db40ToSetVolume != a1)
            v2->dsndSBIF->lpVtbl->SetVolume(v2->dsndSBIF, a1);
    }
}

void pcSND_StreamReinitAndPlay(tSndSoundBufferElem* sbe, int fullsoundlen, int a3, int a4, int a5, char a6, int a2, int a8, int a9)
{
    tSbeMiscData* v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int fullsoundlena;

    if (sbe)
    {
        v9 = sbe->miscData;
        fullsoundlena = pcSND_calcMemoryNeededForTheSound(fullsoundlen, sbe->format);
        pcSND_setCurrSoundBuffElemAndTriggerToAddToThePlayList(sbe);
        v10 = pcSND_CalcSizeToReadByFormat(a4, sbe->format);
        v11 = pcSND_calcMemoryNeededForTheSound(sizeof(tSndSoundBufferElem) * sbe->format * (v10 / (sizeof(tSndSoundBufferElem) * sbe->format)), sbe->format);
        if (v11 < 0 || v11 > fullsoundlena)
            v11 = 0;
        v9->RemoveAfterPlayFinished = 0;
        v9->shouldBeRemovedFromPlayList = 0;
        v9->Stopped_SetIN49e270 = 0;
        v9->readThroughWithMultipleLoop = a6 & 1;
        v9->currOffset = 0;
        v9->currentPlayPosition = 0;
        v9->currSize = 0;
        v9->unk6 = v11;
        v9->fullSize = fullsoundlena;
        v9->distanceToMove = a3;
        v9->sumSize = 0;
        v12 = sbe->format;
        v13 = gDoSoundGlobal.dword_B303C0 + v12;
        if (gDoSoundGlobal.dword_B303C0 + v12 <= 4)
        {
            gDoSoundGlobal.dword_B303C0 += v12;
            if (v13 > gDoSoundGlobal.dword_B303C4)
                gDoSoundGlobal.dword_B303C4 = v13;
            BAS_binsert((unsigned int)sbe, (int)sbe, &dsndSBList);
            pcSND_fillSoundBuffer(0, v9, sbe);
            pcSND_SB_SetFrequency(sbe, a2);
            pcSND_UnknownSoundPanAndVolumeSetByUnknown(sbe, a8);
            pcSND_SB_SetVolume(sbe, a9);
            pcSND_SetPositionNotify(sbe);
            sbe->dsndSBIF->lpVtbl->SetCurrentPosition(sbe->dsndSBIF, 0);
            sbe->dsndSBIF->lpVtbl->Play(sbe->dsndSBIF, 0, 0, 1);
        }
    }
}

tSndSoundBufferElem* pcSND_CreateTrack(tSndHandles* sndHandles, tSndWavHdrStruct* a2)
{
    tSndSoundBufferElem* v4;
    tSbeMiscData* v5;
    IDirectSound8* v6;
    int v7;
    DSBUFFERDESC v8;
    WAVEFORMATEX* v9;

    a2->wfex.wFormatTag = 1;
    v9 = &a2->wfex;
    if (!sndHandles->dsndif)
        return 0;
    v4 = (tSndSoundBufferElem*)MEM_p_Alloc(sizeof(tSndSoundBufferElem));
    memset(v4, 0, sizeof(tSndSoundBufferElem));
    v5 = (tSbeMiscData*)MEM_p_Alloc(sizeof(tSbeMiscData));
    memset(v5, 0, sizeof(tSbeMiscData));
    v5->sizeToUpload = 307200 / a2->wfex.nBlockAlign / 4;
    v5->sizeToUpload *= a2->wfex.nBlockAlign;
    v5->soundBufferSize = 4 * v5->sizeToUpload;
    memset(&v8, 0, sizeof(v8));
    v8.dwSize = 36;
    v8.dwBufferBytes = v5->soundBufferSize;
    v8.lpwfxFormat = v9;
    v8.guid3DAlgorithm.Data2 = 0;
    v8.guid3DAlgorithm.Data3 = 0;
    memset(v8.guid3DAlgorithm.Data4, 0, sizeof(v8.guid3DAlgorithm.Data4));
    v6 = sndHandles->dsndif;
    v8.guid3DAlgorithm.Data1 = 0;
    v8.dwFlags = 0x101E8;
    if (v6->lpVtbl->CreateSoundBuffer(v6, &v8, &(v4->dsndSBIF), 0) != S_OK
        || v4->dsndSBIF->lpVtbl->QueryInterface(v4->dsndSBIF, &IID_IDirectSoundNotify, &(v5->vmilenEventHandlptr_Sub49e950)) != S_OK)
    {
        return 0;
    }
    pcSND_AddToSNDRaster(v4->dsndSBIF, v5->soundBufferSize);
    v4->format = v8.lpwfxFormat->nChannels;
    v4->unk2SetTo64InCreate = 64;
    v4->miscData = v5;
    v5->eventHandle1 = (HANDLE)-1;
    v5->eventHandle2 = (HANDLE)-1;
    v7 = trackEventsArrayNextIdx;
    v5->dsndEventsArrayIdx = trackEventsArrayNextIdx;
    trackEventsArrayNextIdx = v7 + 1;
    v5->fileHandle = CreateFileA(BIG_gst.filePath, 0x80000000, 3u, 0, 3u, 0x10000080u, 0);
    return v4;
}

void pcSND_CreateTrackSB(tSndSoundBufferElem** a1, tSndSoundBufferElem** a2)
{
    tSndSoundBufferElem* v2;
    tSndSoundBufferElem* v3;
    tSndWavHdrStruct v4;

    v4.fullSoundSize = 0;
    v4.dataStartPos = 0;
    v4.wfex.cbSize = 0;
    v4.wfex.nChannels = 1;
    v4.wfex.wFormatTag = 1;
    v4.wfex.wBitsPerSample = 16;
    v4.wfex.nSamplesPerSec = 48000;
    v4.wfex.nBlockAlign = 2;
    v4.wfex.nAvgBytesPerSec = 96000;
    v2 = pcSND_CreateTrack(SND_gst_Params.handles, &v4);
    v4.wfex.nBlockAlign *= 2;
    *a1 = v2;
    v4.wfex.nAvgBytesPerSec *= 2;
    v4.wfex.nChannels = 2;
    v3 = pcSND_CreateTrack(SND_gst_Params.handles, &v4);
    *a2 = v3;
    pcSND_SNDRasterRelease(v3->dsndSBIF);
}

void pcSND_ReleaseTrackSB(tSndSoundBufferElem* a1, tSndSoundBufferElem* a2)
{
    pcSND_ReleaseSoundBuffer(a1);
    pcSND_ReleaseSoundBuffer(a2);
}

void pcSND_ShutDownStreams()
{
    int v0;
    int v1;
    tSoundInstanceElem* v2;
    int v3;

    v0 = 0;
    if (SND_gst_Params.soundInstanceNum > 0)
    {
        v1 = 0;
        do
        {
            v2 = &SND_gst_Params.soundInstances[v1];
            v3 = SND_gst_Params.soundInstances[v1].flags;
            if ((v3 & 0x400) != 0 && (v3 & 0x86000) != 0)
            {
                if (SND_i_IsPlaying(v0))
                    v2->soundIntf->Stop(v2->soundBufElemFromModel);
            }
            ++v0;
            ++v1;
        } while (v0 < SND_gst_Params.soundInstanceNum);
    }
}

int pcSND_l_GetGlobalVol()
{
    return gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0;
}

void pcSND_SetGlobalVol(int a1)
{
    int v1;
    int v2;
    int v3;

    v1 = a1;
    if (gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 != a1)
    {
        if (a1 >= -10000)
        {
            if (a1 > 0)
                v1 = 0;
        }
        else
        {
            v1 = -10000;
        }
        gDoSoundGlobal.pcSNDGlobalVol_dword_B303D0 = v1;
        v2 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v3 = 0;
            do
            {
                if ((SND_gst_Params.soundInstances[v3].flags & 0x400) != 0)
                    SND_SetInstVolume(&SND_gst_Params.soundInstances[v3]);
                ++v2;
                ++v3;
            } while (v2 < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_ComputeDoppler(Vec3* distance, Vec3* position, tSoundInstanceElem* sie)
{
    tGameObjectMain* v4;
    tGameObjectExtended* v5;
    tGameObjectMain* v6;
    tGameObjectExtended* v7;
    double v8;
    tDyn* v9;
    double v12;
    double v13;
    double v14;
    double v15;
    long double v16;
    double v17;
    double v18;
    tSndSoundBufferElem* v19;
    tSndInterfaceStruct* v20;
    double v21;
    Vec3 dest;
    Vec3 a2;
    float siea;
    float sieb;

    if (!sie->sndPosMtxSetInSetPos)
    {
        v4 = sie->gao;
        if (v4)
        {
            if ((v4->flags & 0x8000000) == 0)
                return;
            v5 = v4->extended;
            if (!v5 || !v5->snd)
                return;
            sie->sndPosMtxSetInSetPos = v4->matrices;
        }
    }
    if (sie->sndPosMtxSetInSetPos)
    {
        v6 = sie->gao;
        if (!v6)
            return;
        if ((v6->flags & 0x8000000) == 0)
            return;
        v7 = v6->extended;
        if (!v7)
            return;
        if (!v7->snd)
            return;
        MATH_SubstractVec(&dest, &v6->matrices->Pos, position);
        v8 = MATH_VecLen(&dest);
        siea = (float)v8;
        if (v8 < 0.0000099999997)
            return;
        if (OBJ_pst_GetDynaStruct(v6))
        {
            v9 = OBJ_pst_GetDynaStruct(v6);
            DYN_SpeedGetVector(v9, &a2);
        }
        else
        {
            a2.x = 0.0;
            a2.y = 0.0;
            a2.z = 0.0;
        }
    }
    else
    {
        if (sie->sndPositionSetInSetPosPtr != &sie->sndInstPosVec)
            return;

        v12 = sie->sndInstPosVec.x - sie->sndInstPosVecPrevious.x;
        v13 = sie->sndInstPosVec.y - sie->sndInstPosVecPrevious.y;
        a2.z = sie->sndInstPosVec.z - sie->sndInstPosVecPrevious.z;
        v14 = 1.0 / TIM_gf_dt;
        sie->sndInstPosVecPrevious.x = sie->sndInstPosVec.x;
        sie->sndInstPosVecPrevious.y = sie->sndInstPosVec.y;
        sie->sndInstPosVecPrevious.z = sie->sndInstPosVec.z;
        sieb = (float)v14;
        a2.x = (float)(v12 * sieb);
        a2.y = (float)(v13 * sieb);
        a2.z = a2.z * sieb;
        dest.x = sie->sndInstPosVec.x - position->x;
        dest.y = sie->sndInstPosVec.y - position->y;
        v15 = sie->sndInstPosVec.z - position->z;
        dest.z = (float)v15;
        v16 = sqrt(v15 * dest.z + dest.y * dest.y + dest.x * dest.x);
        siea = (float)v16;
        if (v16 < 0.0000099999997)
            return;
    }
    v17 = SND_gst_Params.fx_DopplerSpeed * siea - (dest.y * distance->y + dest.x * distance->x + dest.z * distance->z);
    if (v17 != 0.0f)
    {
        v18 = ((distance->z - a2.z) * dest.z + (distance->y - a2.y) * dest.y + (distance->x - a2.x) * dest.x) / v17 * SND_gst_Params.fx_doppler;
        if (v18 >= -1.0)
        {
            if (v18 > 1000.0)
                v18 = 1000.0;
        }
        else
        {
            v18 = -0.99900001;
        }
        v19 = sie->soundBufElemFromModel;
        v20 = sie->soundIntf;
        v21 = (double)sie->freq * (v18 + 1.0) + MATH_gd_magicDoubleForRound;
        //*(double*)&dest.x = v21;
        v20->SetFrequency(v19, (*(int32_t *)&v21));
    }
}

float SND_f_Compute3DVol(tSoundInstanceElem* arg0) // @SND_f_GetInstVolume
{
    int v2;
    BOOL v3;
    int i;
    int* v5;
    char v6;
    char v7;
    int v8; 
    Mtx4x4* v9;
    Vec3* v10; 
    double v11;
    int v12;
    int v13;
    double v14;
    double v15;
    double v16;
    double v17;
    double v18;
    float v19[3];
    int a1[3];
    Vec3 a1a;
    float a3; 
    float v22;
    int a3a;

    if (!SND_gst_Params.sndMatrixRef)
        return arg0->volume;
    if (!arg0->sndPositionSetInSetPosPtr)
        return arg0->volume;
    v2 = arg0->flags;
    if ((v2 & 0x1F000000) == 0)
        return arg0->volume;
    if ((v2 & 0x10000000) != 0)
    {
        v22 = SND_f_SphericComputing(&SND_gst_Params.sndMatrixRef->Pos, arg0, &a3a);
        if ((a3a & 1) != 0)
            SND_AutoStop(arg0);
        v3 = (a3a & 2) == 0;
    }
    else
    {
        a1[0] = 0x1000000;
        a1[1] = 0x2000000;
        a1[2] = 0x4000000;
        for (i = 0; i < 3; ++i)
        {
            v5 = &a1[i];
            if ((*v5 & arg0->flags) != 0)
            {
                v19[i] = SND_f_AxisComputing(&SND_gst_Params.sndMatrixRef->Pos, arg0, v5, i);
            }
            else
            {
                v19[i] = 1.0;
                *v5 = 2;
            }
        }
        v6 = a1[2];
        v7 = a1[1];
        v22 = v19[2] * v19[1] * v19[0];
        if ((((a1[0] & 0xff) | (uint8_t)((a1[1] & 0xff) | (a1[2] & 0xff))) & 1) != 0)
            SND_AutoStop(arg0);
        v3 = (a1[0] & (uint8_t)(v7 & v6) & 2) == 0;
    }
    if (!v3)
        SND_AutoPlay(arg0);
    v8 = arg0->flags;
    a3 = v22 * arg0->volume;
    if ((v8 & 0x8000000) != 0)
    {
        v9 = SND_gst_Params.microPosMtxPtr;
        if (!SND_gst_Params.microPosMtxPtr)
        {
            v13 = 0;
            v12 = 0;
            goto LABEL_59;
        }
        v10 = arg0->sndPositionSetInSetPosPtr;
        a1a.x = v10->x - SND_gst_Params.microPosMtxPtr->Pos.x;
        a1a.y = v10->y - SND_gst_Params.microPosMtxPtr->Pos.y;
        v11 = v10->z - SND_gst_Params.microPosMtxPtr->Pos.z;
        a1a.z = (float)v11;
        if (sqrt(v11 * a1a.z + a1a.y * a1a.y + a1a.x * a1a.x) == 0.0)
        {
            v12 = 0;
        LABEL_24:
            v13 = 0;
        LABEL_59:
            arg0->soundIntf->SetPan(arg0->soundBufElemFromModel, v12, v13);
            arg0->pan = v12;
            arg0->front = v13;
            return a3;
        }
        MATH_NormalizeVector(&a1a, &a1a);
        v14 = (v9->v1.z * a1a.z + v9->v1.y * a1a.y + a1a.x * v9->v1.x) * SND_gst_Params.f_surround;
        if (v14 > 1.0)
        {
            v14 = 1.0;
        LABEL_30:
            v22 = (float)(1.0 - v14 + arg0->minPan);
            if (v22 >= 0.0000001)
            {
                if (v22 > 1.0)
                    v22 = 1.0;
            }
            else
            {
                v22 = 0.0;
            }
            v12 = -SND_l_GetAttFromVol(v22);
            goto LABEL_42;
        }
        if (v14 >= -1.0)
        {
            if (v14 > 0.0)
                goto LABEL_30;
            if (v14 >= 0.0)
            {
                v12 = 0;
                goto LABEL_42;
            }
        }
        else
        {
            v14 = -1.0;
        }
        v15 = v14 + arg0->minPan + 1.0;
        v22 = (float)v15;
        if (v15 >= 0.0000001)
        {
            if (v22 > 1.0)
                v22 = 1.0;
            v12 = SND_l_GetAttFromVol(v22);
        }
        else
        {
            v22 = 0.0;
            v12 = SND_l_GetAttFromVol(0.0);
        }
    LABEL_42:
        if (SND_gst_Params.soundRenderMode == 8 || SND_gst_Params.soundRenderMode == 16 || SND_gst_Params.soundRenderMode == 32)
        {
            v16 = v9->v3.z * a1a.z + v9->v3.y * a1a.y + a1a.x * v9->v3.x;
            v17 = v16 + v16;
            if (v17 <= 0.0)
            {
                if (v17 >= 0.0)
                {
                    v13 = 0;
                }
                else
                {
                    v18 = v17 + arg0->minPan + 1.0;
                    v22 = (float)v18;
                    if (v18 >= 0.0000001)
                    {
                        if (v22 > 1.0)
                            v22 = 1.0;
                        v13 = SND_l_GetAttFromVol(v22);
                    }
                    else
                    {
                        v22 = 0.0;
                        v13 = SND_l_GetAttFromVol(0.0);
                    }
                }
            }
            else
            {
                v22 = (float)(1.0 - v17 + arg0->minPan);
                if (v22 >= 0.0000001)
                {
                    if (v22 > 1.0)
                        v22 = 1.0;
                }
                else
                {
                    v22 = 0.0;
                }
                v13 = -SND_l_GetAttFromVol(v22);
            }
            goto LABEL_59;
        }
        goto LABEL_24;
    }
    return a3;
}

int pcSND_i_FxInit()
{
    BYTE Data[4];

    SND_gst_FxParam.i7 = 16;
    SND_gst_FxDefaultParam.i7 = 16;
    SND_gst_FxDefaultParam.i8 = (tSndSoundBufferElem**)MEM_p_Alloc(64u);
    SND_gst_FxParam.i8 = (tSndSoundBufferElem**)MEM_p_Alloc(4 * SND_gst_FxParam.i7);
    memset(SND_gst_FxParam.i8, 0, 4 * SND_gst_FxParam.i7);
    fxWetVol = 0.0;
    memset(&gFXGlobal, 0, sizeof(gFXGlobal));
    if (pcSND_GetFXSettingsFromRegistry_sub_49FD00(Data))
        gFXGlobal.fxEnabled = 0;
    else
        gFXGlobal.fxEnabled = *(uint32_t*)Data != 1;
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

void pcSND_fxStructSet(EAXLISTENERPROPERTIES* a1, int a2)
{
    EAXLISTENERPROPERTIES v2;

    switch (a2)
    {
    case 0:
        v2.ulEnvironment = 0;
        v2.flEnvironmentDiffusion = 1.0;
        v2.lRoomHF = -100;
        v2.lRoomLF = 0;
        v2.flDecayHFRatio = 0.82999998f;
        v2.lReflections = -2602;
        v2.lReverb = 200;
        goto LABEL_14;
    case 1:
        v2.ulEnvironment = 2;
        v2.flEnvironmentSize = 1.9f;
        v2.flEnvironmentDiffusion = 1.0;
        v2.lRoomHF = -454;
        v2.lRoomLF = 0;
        v2.flDecayTime = 0.40000001f;
        v2.flDecayHFRatio = 0.82999998f;
        v2.lReflections = -1646;
        v2.flReflectionsDelay = 0.0020000001f;
        v2.lReverb = 53;
        v2.flReverbDelay = 0.003f;
        goto LABEL_16;
    case 2:
        v2.ulEnvironment = 3;
        v2.flEnvironmentSize = 1.4f;
        v2.flEnvironmentDiffusion = 1.0;
        v2.lRoomHF = -1200;
        v2.lRoomLF = 0;
        v2.flDecayHFRatio = 0.54000002f;
        v2.lReflections = -370;
        v2.lReverb = 1030;
        goto LABEL_15;
    case 3:
        v2.ulEnvironment = 4;
        v2.flEnvironmentSize = 2.5f;
        v2.flEnvironmentDiffusion = 1.0;
        v2.lRoomHF = -6000;
        v2.lRoomLF = 0;
        v2.flDecayTime = 0.5f;
        v2.flDecayHFRatio = 0.1f;
        v2.lReflections = -1376;
        v2.flReflectionsDelay = 0.003f;
        v2.lReverb = -1104;
        v2.flReverbDelay = 0.0040000002f;
        goto LABEL_16;
    case 4:
        v2.ulEnvironment = 5;
        v2.flEnvironmentSize = 11.6f;
        v2.flEnvironmentDiffusion = 1.0;
        v2.lRoomHF = -300;
        v2.lRoomLF = 0;
        v2.flDecayTime = 2.3099999f;
        v2.flDecayHFRatio = 0.63999999f;
        v2.lReflections = -711;
        v2.flReflectionsDelay = 0.012f;
        v2.lReverb = 83;
        v2.flReverbDelay = 0.017000001f;
        goto LABEL_16;
    case 5:
        v2.ulEnvironment = 7;
        v2.flEnvironmentSize = 19.6f;
        v2.flEnvironmentDiffusion = 1.0;
        v2.lRoomHF = -500;
        v2.lRoomLF = 0;
        v2.flDecayTime = 3.9200001f;
        v2.flDecayHFRatio = 0.69999999f;
        v2.lReflections = -1230;
        v2.flReflectionsDelay = 0.02f;
        v2.lReverb = -2;
        v2.flReverbDelay = 0.028999999f;
        goto LABEL_16;
    case 6:
        v2.ulEnvironment = 26;
        v2.flEnvironmentSize = 1.9f;
        v2.flEnvironmentDiffusion = 0.87f;
        v2.lRoom = -1000;
        v2.lRoomHF = -400;
        v2.lRoomLF = -100;
        v2.flDecayTime = 7.1100001f;
        v2.flDecayHFRatio = 0.38f;
        v2.flDecayLFRatio = 0.61000001f;
        v2.lReflections = -1500;
        v2.flReflectionsDelay = 0.1f;
        v2.lReverb = -1000;
        v2.flReverbDelay = 0.046999998f;
        v2.flEchoDepth = 0.25f;
        v2.flHFReference = 3316.1001f;
        v2.flLFReference = 458.20001f;
        v2.ulFlags = 32;
        goto LABEL_19;
    case 7:
        v2.ulEnvironment = 24;
        v2.flEnvironmentSize = 1.8f;
        v2.flEnvironmentDiffusion = 0.60000002f;
        v2.lRoom = -1000;
        v2.lRoomHF = -400;
        v2.lRoomLF = 0;
        v2.flDecayTime = 17.23f;
        v2.flDecayHFRatio = 0.56f;
        v2.flDecayLFRatio = 1.0;
        v2.lReflections = -1713;
        v2.flReflectionsDelay = 0.02f;
        v2.lReverb = -613;
        v2.flReverbDelay = 0.029999999f;
        v2.flEchoDepth = 1.0;
        v2.flModulationTime = 0.81f;
        v2.flModulationDepth = 0.31f;
        v2.flHFReference = 5000.0f;
        v2.flLFReference = 250.0f;
        v2.ulFlags = 31;
        goto LABEL_20;
    case 8:
        v2.ulEnvironment = 26;
        v2.flEnvironmentSize = 2.9000001f;
        v2.flEnvironmentDiffusion = 0.75999999f;
        v2.lRoom = -1000;
        v2.lRoomHF = -700;
        v2.lRoomLF = -500;
        v2.flDecayTime = 5.4899998f;
        v2.flDecayHFRatio = 1.53f;
        v2.flDecayLFRatio = 0.38f;
        v2.lReflections = -1900;
        v2.flReflectionsDelay = 0.054000001f;
        v2.lReverb = -1400;
        v2.flReverbDelay = 0.052000001f;
        v2.flEchoTime = 0.226f;
        v2.flEchoDepth = 0.11f;
        v2.flModulationTime = 0.25f;
        v2.flModulationDepth = 0.0;
        v2.flAirAbsorptionHF = 0.0;
        v2.flHFReference = 12428.5f;
        v2.flLFReference = 99.599998f;
        v2.ulFlags = 32;
        goto LABEL_21;
    case 9:
        v2.ulEnvironment = 21;
        v2.flEnvironmentSize = 1.7f;
        v2.flEnvironmentDiffusion = 0.80000001f;
        v2.lRoom = -1000;
        v2.lRoomHF = -1000;
        v2.lRoomLF = 0;
        v2.flDecayTime = 2.8099999f;
        v2.flDecayHFRatio = 0.14f;
        v2.lReflections = 429;
        v2.flReflectionsDelay = 0.014f;
        v2.lReverb = 1023;
        v2.flReverbDelay = 0.021f;
        goto LABEL_17;
    case 10:
        v2.ulEnvironment = 17;
        v2.flEnvironmentSize = 100.0f;
        v2.flEnvironmentDiffusion = 0.27000001f;
        v2.lRoom = -1000;
        v2.lRoomHF = -2500;
        v2.lRoomLF = 0;
        v2.flDecayTime = 1.49f;
        v2.flDecayHFRatio = 0.20999999f;
        v2.lReflections = -2780;
        v2.flReflectionsDelay = 0.30000001f;
        v2.lReverb = -1434;
        v2.flReverbDelay = 0.1f;
        v2.flEchoDepth = 1.0f;
        v2.ulFlags = 31;
        goto LABEL_18;
    case 11:
        v2.ulEnvironment = 16;
        v2.flEnvironmentDiffusion = 0.5f;
        v2.lRoomHF = -800;
        v2.lRoomLF = 0;
        v2.flDecayHFRatio = 0.67000002f;
        v2.lReflections = -2273;
        v2.lReverb = -1691;
    LABEL_14:
        v2.flEnvironmentSize = 7.5f;
    LABEL_15:
        v2.flReverbDelay = 0.011f;
        v2.flReflectionsDelay = 0.0070000002f;
        v2.flDecayTime = 1.49f;
    LABEL_16:
        v2.lRoom = -1000;
    LABEL_17:
        v2.flEchoDepth = 0.0;
        v2.ulFlags = 63;
    LABEL_18:
        v2.flDecayLFRatio = 1.0;
        v2.flHFReference = 5000.0f;
        v2.flLFReference = 250.0f;
    LABEL_19:
        v2.flModulationDepth = 0.0;
        v2.flModulationTime = 0.25f;
    LABEL_20:
        v2.flEchoTime = 0.25f;
        v2.flAirAbsorptionHF = -5.0f;
    LABEL_21:
        v2.flRoomRolloffFactor = 0.0;
        v2.vReverbPan.z = 0.0;
        v2.vReverbPan.y = 0.0;
        v2.vReverbPan.x = 0.0;
        v2.vReflectionsPan.z = 0.0;
        v2.vReflectionsPan.y = 0.0;
        v2.vReflectionsPan.x = 0.0;
        memcpy(a1, &v2, sizeof(EAXLISTENERPROPERTIES));
        break;
    default:
        return;
    }
}

int pcSND_EAXListenerPropertiesSet(int a2)
{
    EAXLISTENERPROPERTIES a1;

    pcSND_fxStructSet(&a1, a2);
    ppdsndIKSPropertySet->lpVtbl->Set(ppdsndIKSPropertySet, &DSPROPSETID_EAX30_ListenerProperties, 1, 0, 0, (LPVOID*)&a1, sizeof(EAXLISTENERPROPERTIES));
    return 0;
}

void pcSND_DummyFXFunc(tSoundFxParams* a1)
{
    tSndSoundBufferElem** v1;
    int v2;

    if (gFXGlobal.fxEnabled && (a1->flags & 1) != 0 && a1->i7 > 0)
    {
        v1 = a1->i8;
        v2 = a1->i7;
        do
        {
            if (*v1) {
                //nullsub_32();
            }
            ++v1;
            --v2;
        } while (v2);
    }
}

int pcSND_i_FxSetMode(tSoundFxParams* a1, int a2)
{
    if (gFXGlobal.fxEnabled)
    {
        pcSND_EAXListenerPropertiesSet(a2);
        gFXGlobal.mode = a1->mode;
        pcSND_DummyFXFunc(a1);
    }
    return 0;
}

int pcSND_i_FxGetMode(tSoundFxParams* a1)
{
    return a1->mode;
}

int pcSND_i_FxSetFeedback(tSoundFxParams* a1, int x)
{
    int v1;

    if (gFXGlobal.fxEnabled)
    {
        v1 = a1->mode;
        if (v1 > 12)
        {
            switch (v1)
            {
            case 65537:
            case 65538:
            case 65541:
            case 65542:
            case 65543:
            case 65544:
                return 0;
            default:
            LABEL_6:
                a1->mode = 0;
                pcSND_DummyFXFunc(a1);
                break;
            }
        }
        else if (v1 != 12)
        {
            switch (v1)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 9:
                return 0;
            default:
                goto LABEL_6;
            }
        }
    }
    return 0;
}

int pcSND_i_FxGetFeedback(tSoundFxParams* a1)
{
    int v1;

    v1 = a1->mode;
    if (v1 > 12)
    {
        switch (v1)
        {
        case 65537:
        case 65538:
        case 65541:
        case 65542:
        case 65543:
        case 65544:
            return 0;
        default:
        LABEL_5:
            a1->mode = 0;
            break;
        }
    }
    else if (v1 != 12)
    {
        switch (v1)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
            return 0;
        default:
            goto LABEL_5;
        }
    }
    return 0;
}

int pcSND_i_FxSetDelay(tSoundFxParams* a1, int x)
{
    int v1;
    int result;

    if (!gFXGlobal.fxEnabled)
        return 0;
    v1 = a1->mode;
    if (v1 <= 12)
    {
        if (v1 != 12)
        {
            switch (v1)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 9:
                return 0;
            case 8:
                goto LABEL_6;
            default:
                goto LABEL_8;
            }
        }
        return 0;
    }
    switch (v1)
    {
    case 65536:
    LABEL_6:
        pcSND_DummyFXFunc(a1);
        return 0;
    case 65537:
    case 65538:
    case 65540:
    case 65541:
    case 65542:
    case 65543:
    case 65544:
        return 0;
    default:
    LABEL_8:
        a1->mode = 0;
        result = -1;
        break;
    }
    return result;
}

int pcSND_i_FxGetDelay(tSoundFxParams* a1)
{
    int result;

    switch (a1->mode)
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
        result = (int)a1; // TODO FIXME: ezt nem gondolnám, hogy jó megnézni. mi hívja ezt?
        break;
    default:
        a1->mode = 0;
        result = -1;
        break;
    }
    return result;
}

int pcSND_i_FxSetWetVolume(tSoundFxParams* arg0, int a1)
{
    if (gFXGlobal.fxEnabled)
    {
        SND_f_GetGlobalVol();
        fxWetVol = SND_f_GetVolFromAtt(a1);
        if (arg0->mode)
            pcSND_DummyFXFunc(arg0);
    }
    return 0;
}

int pcSND_i_FxGetWetVolume(tSoundFxParams* a1)
{
    int v1;
    int v2;
    int v3;
    float v5;
    float a1a = 0;

    v5 = SND_f_GetGlobalVol();
    v1 = a1->mode;
    if (v1 > 9)
    {
        v2 = v1 - 12;
        if (v2)
        {
            v3 = v2 - 65531;
            if (v3)
            {
                if (v3 != 1)
                    LABEL_8:
                a1a = v5;
            }
        }
    }
    else if (v1 < 8)
    {
        switch (v1)
        {
        case 0:
            a1a = 0;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            break;
        default:
            goto LABEL_8;
        }
    }
    fxWetVol = a1a;
    return SND_l_GetAttFromVol(a1a);
}

int pcSND_i_FxSetWetPan(tSoundFxParams* a1, int a2)
{
    a1->wetPan = a2;
    return 0;
}

int pcSND_i_FxGetWetPan(tSoundFxParams* a1)
{
    return a1->wetPan;
}

void pcSND_SNDPropertyEnableRoomReverb(tSoundFxParams* xx, tSndSoundBufferElem* a1)
{
    if (gFXGlobal.fxEnabled)
        pcSND_SNDPropertySetRoomReverb(a1, 1);
}

int pcSND_i_FxEnable(tSoundFxParams* xx)
{
    int v1;
    int v2;
    tSndSoundBufferElem* v3;

    if (gFXGlobal.fxEnabled)
    {
        v1 = xx->i7;
        v2 = 0;
        xx->flags |= 1u;
        if (v1 > 0)
        {
            do
            {
                v3 = xx->i8[v2];
                if (v3)
                    pcSND_SNDPropertyEnableRoomReverb(xx, v3);
                ++v2;
            } while (v2 < xx->i7);
        }
    }
    return 0;
}

int pcSND_i_FxDisable(tSoundFxParams* a1)
{
    int v1;
    int v2;

    if (gFXGlobal.fxEnabled)
    {
        v1 = a1->i7;
        v2 = 0;
        a1->flags &= ~1u;
        if (v1 > 0)
        {
            do
            {
                if (a1->i8[v2])
                    pcSND_DisableReverbAndRestartPlaying(a1, a1->i8[v2]);
                ++v2;
            } while (v2 < a1->i7);
        }
    }
    return 0;
}

int pcSND_Stream_GetStatus(tSndSoundBufferElem* a1a, int* a2)
{
    int result;
    int a1 = (int)a1a;

    *a2 = 0;
    if (a1 > 0)
        result = (a1 <= SND_gi_StreamListSize) - 1;
    else
        result = -1;
    return result;
}

int pcSND_Stream_Play(tSndSoundBufferElem* a1a, int flags)
{
    int result; 
    int a1 = (int)a1a;

    if (a1 > 0)
        result = (a1 <= SND_gi_StreamListSize) - 1;
    else
        result = -1;
    return result;
}

void pcSND_Stream_Duplicate(tSndHandles* a1a, tSndSoundBufferElem* a2a, tSndSoundBufferElem** a3a)
{
    tSndStreamList* v3;
    int v4;
    tSndStreamList* v5;
    
    int* a3 = (int*)a3a;
    int a2 = (int)a2a;

    *a3 = 0;
    if (a2 > 0 && a2 <= SND_gi_StreamListSize)
    {
        v3 = &SND_gast_StreamList[a2 - 1];
        v4 = SND_i_StreamAlloc();
        *a3 = v4;
        if (v4 > 0 && v4 <= SND_gi_StreamListSize)
        {
            v5 = &SND_gast_StreamList[v4 - 1];
            v5->freq = v3->freq;
            v5->pan2 = v3->pan2;
            v5->pan1 = v3->pan1;
            v5->currPos = 0;
            v5->vol = v3->vol;
        }
    }
}

void pcSND_Stream_GetFrequency(tSndSoundBufferElem* a1a, int* a2)
{
    int a1 = (int)a1a;

    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
        *a2 = SND_gast_StreamList[a1 - 1].freq;
}

void pcSND_Stream_GetPan(tSndSoundBufferElem* a1a, int* a2, int* a3)
{
    tSndStreamList* v3;
    int a1 = (int)a1a;

    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
    {
        v3 = &SND_gast_StreamList[a1 - 1];
        *a2 = SND_gast_StreamList[a1 - 1].pan1;
        *a3 = v3->pan2;
    }
}

void pcSND_Stream_GetCurrPos(tSndSoundBufferElem* a1a, int* a2, int* a3)
{
    int a1 = (int)a1a;

    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
    {
        *a2 = SND_gast_StreamList[a1 - 1].currPos;
        *a3 = 0;
    }
}

void pcSND_Stream_SetVolume(tSndSoundBufferElem* a1a, int a2)
{
    int a1 = (int)a1a;

    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
        SND_gast_StreamList[a1 - 1].vol = a2;
}

int pcSND_iTrack_GetStatus(tSndSoundBufferElem* a1, int *a2)
{
    int result;

    result = BAS_bsearch((int)a1, &SND_gst_TrackLookUpTable);
    if (result != -1)
        result = SND_gpst_Interface.win.GetStatus(SND_gax_TrackPlayingList[result].sndBufferElem, a2);
    return result;
}

int pcSND_iTrack_Play(tSndSoundBufferElem* a1, int a2)
{
    int result;

    result = BAS_bsearch((int)a1, &SND_gst_TrackLookUpTable);
    if (result != -1)
        result = SND_gpst_Interface.win.Play(SND_gax_TrackPlayingList[result].sndBufferElem, a2);
    return result;
}

void pcSND_Track_GetFrequency(tSndSoundBufferElem* key, int *a2)
{
    int result;

    result = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (result != -1)
        SND_gpst_Interface.win.GetFrequency(SND_gax_TrackPlayingList[result].sndBufferElem, a2);
}

void pcSND_Track_GetPan(tSndSoundBufferElem* key, int *a2, int *a3)
{
    int result; 

    result = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (result != -1)
        SND_gpst_Interface.win.GetPan(SND_gax_TrackPlayingList[result].sndBufferElem, a2, a3);
}

void pcSND_Track_GetCurrPos(tSndSoundBufferElem* key, int *a2, int *a3)
{
    int result;

    result = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (result != -1)
        SND_gpst_Interface.win.GetCurrPos(SND_gax_TrackPlayingList[result].sndBufferElem, a2, a3);
}

void pcSND_Track_Pause(tSndSoundBufferElem* a1)
{
    int v1;
    tTrackListElem* v2;
    int v3;
    int v4;
    int v5;
    tSoundInstanceElem* v6;

    v1 = BAS_bsearch((int)a1, &SND_gst_TrackLookUpTable);
    if (v1 != -1)
    {
        v2 = &SND_gax_TrackPlayingList[v1];
        SND_gpst_Interface.win.Pause(v2->sndBufferElem);
        v3 = v2->trackID;
        if (v3 >= 0 && v3 < SND_gst_Params.soundInstanceNum)
        {
            v4 = v3;
            v5 = SND_gst_Params.soundInstances[v4].flags;
            v6 = &SND_gst_Params.soundInstances[v4];
            if ((v5 & 0x400) != 0)
                v6->flags = v5 & ~0x10u;
        }
    }
}

void pcSND_Track_Release(tSndSoundBufferElem* a1)
{
    int v1;
    tTrackListElem* v2;

    v1 = BAS_bsearch((int)a1, &SND_gst_TrackLookUpTable);
    if (v1 != -1)
    {
        v2 = &SND_gax_TrackPlayingList[v1];
        SND_gpst_Interface.win.Stop(v2->sndBufferElem);
        SND_TrackDeleteInstanceFromList(v2->trackID);
    }
}

void pcSND_Track_SetFrequency(tSndSoundBufferElem* key, int a2)
{
    int v2;

    v2 = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (v2 != -1)
        SND_gpst_Interface.win.SetFrequency(SND_gax_TrackPlayingList[v2].sndBufferElem, a2);
}

void pcSND_Track_SetPan(tSndSoundBufferElem* a1, int a2, int a3)
{
    int v3;

    v3 = BAS_bsearch((int)a1, &SND_gst_TrackLookUpTable);
    if (v3 != -1)
        SND_gpst_Interface.win.SetPan(SND_gax_TrackPlayingList[v3].sndBufferElem, a2, a3);
}

void pcSND_Track_SetCurrPos(tSndSoundBufferElem* key, int a2)
{
    int result;

    result = BAS_bsearch((int)key, &SND_gst_TrackLookUpTable);
    if (result != -1)
        SND_gpst_Interface.win.SetCurrPos(SND_gax_TrackPlayingList[result].sndBufferElem, a2);
}

void pcSND_Track_SetVolume(tSndSoundBufferElem* arg0, int a1)
{
    int v2;
    tTrackListElem* v3;
    int v4;
    float v5;

    v2 = BAS_bsearch((int)arg0, &SND_gst_TrackLookUpTable);
    if (v2 != -1)
    {
        v3 = &SND_gax_TrackPlayingList[v2];
        if (v3->unk1)
        {
            v5 = SND_f_GetVolFromAtt(a1) * v3->f1;
            v4 = SND_l_GetAttFromVol(v5);
            SND_gpst_Interface.win.Setvolume(v3->sndBufferElem, v4);
        }
        else
        {
            SND_gpst_Interface.win.Setvolume(v3->sndBufferElem, a1);
        }
    }
}

void pcSND_Track_Stop(tSndSoundBufferElem* a1)
{
    int v1;
    tTrackListElem* v2;
    int v3;
    int v4;
    int v5;
    tSoundInstanceElem* v6;

    if (a1)
    {
        v1 = BAS_bsearch((int)a1, &SND_gst_TrackLookUpTable);
        if (v1 != -1)
        {
            v2 = &SND_gax_TrackPlayingList[v1];
            SND_gpst_Interface.win.Stop(v2->sndBufferElem);
            v3 = v2->trackID;
            if (v3 >= 0 && v3 < SND_gst_Params.soundInstanceNum)
            {
                v4 = v3;
                v5 = SND_gst_Params.soundInstances[v4].flags;
                v6 = &SND_gst_Params.soundInstances[v4];
                if ((v5 & 0x400) != 0)
                    v6->flags = v5 & ~0x10u;
            }
        }
    }
}

void pcSND_SB_GetCurrPos(tSndSoundBufferElem* a1, int* a2, int* a3)
{
    tSbeMiscData* v3;
    int v4;
    int a3a;

    if (a1->miscData)
    {
        *a3 = 0;
        *a2 = 0;
        if (a1)
        {
            v3 = a1->miscData;
            if (v3)
            {
                a1->dsndSBIF->lpVtbl->GetCurrentPosition(a1->dsndSBIF, &a3a, 0); // !!!!!!!!!!!!!!!!!!!!!!!!a3a
                v4 = v3->currentPlayPosition;
                if (a3a >= v4)
                    *a2 = a3a + v3->currSize - v4;
                else
                    *a2 = a3a + v3->soundBufferSize - v4 + v3->currSize; 
            }
        }
    }
    else
    {
        a1->dsndSBIF->lpVtbl->GetCurrentPosition(a1->dsndSBIF, a2, a3);
    }
}

void pcSND_flipPlayStopState(tSndSoundBufferElem* a1)
{
    tSndSoundBufferElem* v1;
    int a1a;

    v1 = a1;
    a1->dsndSBIF->lpVtbl->GetStatus(a1->dsndSBIF, &a1a);
    if ((a1a & 1) != 0)
    {
        v1->miscData->Stopped_SetIN49e270 = 1;
        v1->dsndSBIF->lpVtbl->Stop(v1->dsndSBIF);
        do
            v1->dsndSBIF->lpVtbl->GetStatus(v1->dsndSBIF, &a1a);
        while ((a1a & 1) != 0);
    }
    else
    {
        v1->miscData->Stopped_SetIN49e270 = 0;
        v1->dsndSBIF->lpVtbl->Play(v1->dsndSBIF, 0, 0, 1);
    }
}

void pcSND_SB_Pause(tSndSoundBufferElem* i)
{
    tSndSoundBufferElem* v1;
    int v3;
    IDirectSoundBuffer* v4;
    int ia;

    v1 = i;
    if (i->miscData) {
        pcSND_flipPlayStopState(i);
        return;
    }
    i->dsndSBIF->lpVtbl->GetStatus(i->dsndSBIF, &ia);
    if ((ia & 1) != 0) {
        v1->dsndSBIF->lpVtbl->Stop(v1->dsndSBIF);
        return;
    }
    v3 = 0;
    for (ia = 0; v3 < SND_gst_Params.soundInstanceNum; ia = ++v3)
    {
        if (SND_gst_Params.soundInstances[v3].soundBufElemFromModel == v1)
            break;
    }
    v4 = v1->dsndSBIF;
    ia = (SND_gst_Params.soundInstances[v3].flags >> 23) & 1;
    v4->lpVtbl->Play(v4, 0, 0, ia);
}

void pcSND_D3DSoundBufAAndIKPopertySetRelease(tSndSoundBufferElem* a1)
{
    IDirectSound3DBuffer* v1;
    IKsPropertySet* v2;

    v1 = a1->dsnd3dbufferIF;
    if (v1)
    {
        v1->lpVtbl->Release(a1->dsnd3dbufferIF);
        v2 = a1->dsndIKPropertySetIF;
        if (v2)
            v2->lpVtbl->Release(a1->dsndIKPropertySetIF);
    }
}

void pcSND_SB_Release(tSndSoundBufferElem* a1)
{
    tSndSoundBufferElem* v1;
    int a1a;

    v1 = a1;
    if (a1)
    {
        BAS_bdeletegetval((int)a1, &dsndSBList, &a1a);
        if (v1->miscData)
        {
            if (a1a)
                gDoSoundGlobal.dword_B303C0 -= v1->format;
            pcSND_MiscDataRelease(v1);
            MEM_Free(v1);
        }
        else
        {
            if (a1a)
                gDoSoundGlobal.sndConcurrentlyPlayedNum -= v1->format;
            pcSND_D3DSoundBufAAndIKPopertySetRelease(v1);
            pcSND_SNDRasterRelease(v1->dsndSBIF);
            v1->dsndSBIF->lpVtbl->Release(v1->dsndSBIF);
            MEM_Free(v1);
        }
    }
}

int pcSND_Stream_Release(tSndSoundBufferElem* a1a)
{
    int result;
    int a1 = (int)a1a;

    result = a1;
    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
    {
        result = 0;
        memset(&SND_gast_StreamList[a1 - 1], 0, sizeof(tSndStreamList));
    }
    return result;
}

int pcSND_Stream_SetFrequency(tSndSoundBufferElem* a1a, int a2)
{
    int result;
    int a1 = (int)a1a;

    result = a1;
    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
    {
        result = 3 * a1;
        SND_gast_StreamList[a1 - 1].freq = a2;
    }
    return result;
}

void pcSND_Stream_SetPan(tSndSoundBufferElem* a1a, int a2, int a3)
{
    tSndStreamList* v3;
    int a1 = (int)a1a;

    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
    {
        v3 = &SND_gast_StreamList[a1 - 1];
        v3->pan1 = a2;
        v3->pan2 = a3;
    }
}

void pcSND_Stream_SetCurrPos(tSndSoundBufferElem* a1a, int a2)
{
    int a1 = (int)a1a;

    if (a1 > 0 && a1 <= SND_gi_StreamListSize)
        SND_gast_StreamList[a1 - 1].currPos = a2;
}

int pcSND_l_GetGlobalPan() {
    return 0;
}

void pcSND_SetGlobalPan(int a1) {
}

void pcSND_Null(Vec3* v) {
}

void pcSND_StreamPrefetch() {
}

void pcSND_StreamFlush() {
}

void pcSND_StreamGetMonoToStereo(tSndSoundBufferElem** a1, tSndSoundBufferElem** a2) {
}

void pcSND_StreamGetStereoToMono() {
}

void pcSND_Stream_Pause(tSndSoundBufferElem* x) {
}

void pcSND_Stream_Stop(tSndSoundBufferElem* x) {
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