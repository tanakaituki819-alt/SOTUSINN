#pragma once
#include <windows.h>
#include <wrl/client.h>
// 2. Windows 10/11標準の XAudio2.9 を呼び出すためのヘッダー
#include <xaudio2.h>
#include <x3daudio.h>

#include <d3dx10.h>
#include <vector>
#include <cstdint>

#include "AudioLoaderMp3.h" // std::async マルチスレッド用のヘッダー


#include <future>
#include <atomic>              // std::atomic 用
#include <thread>              // std::thread 用
#include <mutex>               // std::mutex 用
#include <condition_variable>  // std::condition_variable 用

#include <xaudio2fx.h> 
#include <xapofx.h>
#include <cstdio>
#pragma comment(lib, "xaudio2.lib")

//------------------------
//ここにメモを書いていく
//ここの名前を使えばカーソルを合わせるだけで説明が出る
//-------------------------

namespace AudioNameSp {

    /**
     * @brief XAudio2 ネイティブリバーブ / エコーパラメータ（調整用メモ）
     *
     * 【基本・出力バランス】
     * - WetDryMix          : 残響音(Wet)と原音(Dry)の比率 (0.0% [全原音] ～ 100.0% [全残響])
     *
     * 【初期反射音 (Reflections / Early Echoes)】
     * - ReflectionsDelay   : 原音から最初の壁反射までの遅延時間 (0 ～ 300 ms) ※200ms以上でエコー感
     * - ReflectionsGain    : 初期反射音の音量ゲイン (-100.0 ～ 20.0 dB / 0.0dBが標準)
     *
     * 【後続残響音 (Late Reverb / Tail)】
     * - ReverbDelay        : 初期反射から後続残響が開始するまでの遅延 (0 ～ 85 ms)
     * - ReverbGain         : 後続残響音の音量ゲイン (-100.0 ～ 20.0 dB)
     * - DecayTime          : 音が消えるまでの時間 (0.1 ～ 20.0 秒) ※長めにするとやまびこ感
     * - Density            : 残響音の密集度 (0.0% [スカスカ・離れた壁] ～ 100.0% [密閉空間])
     * - RoomSize           : 仮想部屋のサイズ (1.0 ～ 100.0 フィート)
     *
     * 【エコー質感・音の拡がり (Diffusion)】
     * - EarlyDiffusion     : 初期反射の分散度 (0 [輪郭をそのまま跳ね返す/エコー] ～ 15 [拡散/リバーブ])
     * - LateDiffusion      : 後続残響の分散度 (0 [個別の音が分離] ～ 15 [滑らかな残響])
     *
     * 【周波数・トーン調整 (Filter & EQ)】
     * - LowEQGain          : 低音域ゲイン (0 ～ 12 / デフォルト8)
     * - LowEQCutoff        : 低音域カットオフ周波数 (0 ～ 9)
     * - HighEQGain         : 高音域ゲイン (0 ～ 12 / デフォルト8)
     * - HighEQCutoff       : 高音域カットオフ周波数 (0 ～ 14)
     * - RoomFilterFreq     : ルームフィルター基準周波数 (20.0 ～ 20000.0 Hz)
     * - RoomFilterMain     : ルームフィルター主ゲイン (-100.0 ～ 0.0 dB)
     * - RoomFilterHF       : ルームフィルター高周波ゲイン (-100.0 ～ 0.0 dB)
     *
     * 【サラウンド・位置制御 (Positioning)】
     * - RearDelay          : リア(後方)スピーカーへの遅延時間 (0 ～ 5 ms)
     * - PositionLeft       : 左チャンネル空間配置 (0 ～ 30)
     * - PositionRight      : 右チャンネル空間配置 (0 ～ 30)
     * - PositionMatrixLeft : 左マトリクス位置 (0 ～ 30)
     * - PositionMatrixRight: 右マトリクス位置 (0 ～ 30)
     */
	using Xaudio2fx_reverb_parameters = XAUDIO2FX_REVERB_PARAMETERS;
    /**
     * @brief I3DL2リバーブパラメータ（調整用メモ）
     *
     * @brief   - Room: 部屋全体の音量 (-10000～0 mB / 0で最大)
     * @brief   - DecayTime: 残響時間 (0.1～20.0秒 / 3.0秒以上で洞窟風)
     * @brief   - ReflectionsDelay: 初期反射の遅延 (0.0～0.3秒 / 0.2秒以上でエコー感)
     * @brief   - Reflections: 最初の壁反射音量 (下げると混ざりがスッキリする)
     * @brief   - Reverb: 後続残響音量
     * @brief   - Diffusion: 拡散度 (100%で滑らか / 0%でクリアな跳ね返り)
     */
	using xAUDIO2FX_REVERB_I3DL2_PARAMETERS = XAUDIO2FX_REVERB_I3DL2_PARAMETERS;
}