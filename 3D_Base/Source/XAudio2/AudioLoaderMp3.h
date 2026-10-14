#pragma once


#include <windows.h>
#include <vector>

// MP3読み込み結果を格納する構造体
struct SoundData {
    WAVEFORMATEX wfx;               // 音声のフォーマット
    std::vector<BYTE> audioBuffer;  // 生の音声データ（PCM）
};

// MP3ファイルを読み込んでPCMに変換する関数
bool LoadMP3(const wchar_t* filename, SoundData& outData);