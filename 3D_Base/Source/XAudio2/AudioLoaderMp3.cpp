#include "AudioLoaderMp3.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl/client.h>

#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Ole32.lib")

bool LoadMP3(const wchar_t* filename, SoundData& outData) {
    HRESULT hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<IMFSourceReader> pReader;
    hr = MFCreateSourceReaderFromURL(filename, nullptr, &pReader);
    if (FAILED(hr)) { MFShutdown(); return false; }

    // 出力形式をPCMに設定
    Microsoft::WRL::ComPtr<IMFMediaType> pPartialType;
    MFCreateMediaType(&pPartialType);
    pPartialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    pPartialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pPartialType.Get());

    // フォーマットの取得
    Microsoft::WRL::ComPtr<IMFMediaType> pUncompressedAudioType;
    pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pUncompressedAudioType);

    GUID majorType = { 0 };
    pUncompressedAudioType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
    if (majorType == MFMediaType_Audio) {
        UINT32 numChannels = 0, samplesPerSec = 0, bitsPerSample = 0;
        pUncompressedAudioType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &numChannels);
        pUncompressedAudioType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &samplesPerSec);
        pUncompressedAudioType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &bitsPerSample);

        outData.wfx.wFormatTag = WAVE_FORMAT_PCM;
        outData.wfx.nChannels = (WORD)numChannels;
        outData.wfx.nSamplesPerSec = samplesPerSec;
        outData.wfx.wBitsPerSample = (WORD)bitsPerSample;
        outData.wfx.nBlockAlign = (outData.wfx.nChannels * outData.wfx.wBitsPerSample) / 8;
        outData.wfx.nAvgBytesPerSec = outData.wfx.nSamplesPerSec * outData.wfx.nBlockAlign;
        outData.wfx.cbSize = 0;
    }

    // データの読み込み
    while (true) {
        DWORD flags = 0;
        Microsoft::WRL::ComPtr<IMFSample> pSample;
        hr = pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &pSample);
        if (FAILED(hr) || (flags & MF_SOURCE_READERF_ENDOFSTREAM)) break;

        if (pSample) {
            Microsoft::WRL::ComPtr<IMFMediaBuffer> pBuffer;
            pSample->ConvertToContiguousBuffer(&pBuffer);
            BYTE* pAudioData = nullptr;
            DWORD cbBufferLength = 0;
            pBuffer->Lock(&pAudioData, nullptr, &cbBufferLength);
            outData.audioBuffer.insert(outData.audioBuffer.end(), pAudioData, pAudioData + cbBufferLength);
            pBuffer->Unlock();
        }
    }

    MFShutdown();
    return true;
}