#pragma once
//// 重複定義やマクロ衝突（min/max）を予防するマクロ
//#ifndef NOMINMAX
//#define NOMINMAX
//#endif
//
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//
//// 1. Windowsのバージョンターゲットを「Windows 10以降」に強制指定するマクロ（これが超重要！）
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WIN10
//#endif
//
//#define _MMREG_ // mmreg.h の二重読み込み防止マクロ
//
//#include <windows.h>
//#include <wrl/client.h>
//// 2. Windows 10/11標準の XAudio2.9 を呼び出すためのヘッダー
//#include <xaudio2.h>
//#include <x3daudio.h>
//
//#include <d3dx10.h>
//#include <vector>
//#include <cstdint>
//
//#include "AudioLoaderMp3.h" // std::async マルチスレッド用のヘッダー
//
//
//#include <future>
//#include <atomic>              // std::atomic 用
//#include <thread>              // std::thread 用
//#include <mutex>               // std::mutex 用
//#include <condition_variable>  // std::condition_variable 用

#include"XAudio2Memo.h"

	// 外部に渡す軽量な音声ハンドル
	struct SoundHandleX {
		uint32_t id = 0;          // vectorのインデックス//uint32_tは符号なし３２ビット　-がない
		uint32_t generation = 0;  // 世代番号（再利用時の誤動作防止）
		bool IsValid() const { return id != 0 && generation != 0; }//両方０じゃないならtrueがかえってくる
	};


class XAudio2
{
public:
	//（音そのものの加工）の種類//ならす音を鳴らすイメージ
	enum class EffectType {
		Default,
		Underwater,  //水中
		Occlusion,//壁越し
		LargeCave,//広い洞窟
	};
	//（送り先のサブミックス）の種類//空間を変えるイメージ
	enum class SubmixTargetType {
		Default,		// 通常
		Reverb,			// 残響
		Echo,			// エコー
	};
private:
	//
	HRESULT CreateMasterVoice();//マスターボイスの作成
	HRESULT CreateSubmixVoice();//SE,BGMのサブミックスボイスの作成
	HRESULT CreateReverbVoice();//SE,BGMのリバーブ（残響）ボイスの作成
	HRESULT CreateEchoVoice();//SE,BGMのサブミックスボイスの作成
	// マスターボイス（最終出力スピーカー）を入れるポインタ
	IXAudio2MasteringVoice* m_pMasterVoice=nullptr;

	// 音量管理専用のサブミックスボイス ---
	IXAudio2SubmixVoice* m_pBGMSubmixVoice = nullptr;
	IXAudio2SubmixVoice* m_pSESubmixVoice = nullptr;

	// エフェクト（リバーブ）をかけるための中継ボイス
	IXAudio2SubmixVoice* m_pBGMReverbSubmixVoice = nullptr;
	IXAudio2SubmixVoice* m_pSEReverbSubmixVoice = nullptr;
	// エフェクト（リバーブ）をかけるための中継ボイス
	IXAudio2SubmixVoice* m_pBGMEchoSubmixVoice = nullptr;
	IXAudio2SubmixVoice* m_pSEEchoSubmixVoice = nullptr;
public:
	enum class SoundType {
		SE,//SEALLStopでつかう
		BGM,//BGMALLStopでつかう
		Echo
	};

	// 音声データのロード状態
	enum class LoadState {
		Unloaded,   // 未読み込み
		Loading,    // 読み込み中
		Loaded      // 読み込み完了
	};
	enum class BGMList {
		Title,
		PlayerSetUp,
		GameMain,
		Result,

		MAX,
	};
	enum class SEList {
		BAKUHATU,
		SYAKIN,
		DBIMU,
		SRASYU,
		GOGOGO,
		MOVE,
		KIDOU,
		MOVE2,
		SHOT,//ショット
		KAIHI,//回避音
		SATTA,//シャッター
		JANP,//ジャンプ
		EI,//エ゛イ゛
		MAX,
	};
	static XAudio2* GetInstance()
	{
		static XAudio2 s_Instance;
		return &s_Instance;
	}
	struct SourceVoice3D
	{
		IXAudio2SourceVoice* pSourceVoice;
		D3DXVECTOR3 position;
	};
	struct Sound {
		int No;
		TCHAR path[256];	//ファイルの名前(パス付)
	};
	// 内部ボイス管理用スロット
	struct VoiceSlot {
		IXAudio2SourceVoice* pVoice = nullptr;
		uint32_t generation = 1; // 再利用されるたびに加算
		SoundType Type = SoundType::SE;
		bool isActive = false;
		int SoundNo = -0;
	};

	XAudio2();
	~XAudio2();
	HRESULT Initialize();// XAudio2の初期化
	
	//別スレッド読み込み
	HRESULT LoadData();// 音声ファイルを読み込む関数
	

	HRESULT PlaySample(const SoundData& soundData,D3DXVECTOR3 position);
	void Update();


	//音を聞く人の位置
	X3DAUDIO_LISTENER m_listener;
	// 
	/*@brief   - ハンドルを返す方式に変更したBGM再生
	* @brief   - ReverbEffectactive: 音の鳴らす空間の種類
	* @brief   - isLoop: ループ生成するか？trueでループ
	 */
	SoundHandleX BGMPlayHandle(BGMList i, SubmixTargetType ReverbEffectactive = SubmixTargetType::Default, bool isLoop = true);
	/*
	* @brief   ハンドルを返す方式に変更したSE再生
	* @brief   - ReverbEffectactive: 音の鳴らす空間の種類
	* @brief   - isLoop: ループ生成するか？trueでループ
 */
	SoundHandleX SEPlayHandle(SEList i, SubmixTargetType ReverbEffectactive = SubmixTargetType::Default,bool isLoop=false);
	// ハンドル経由の安全な操作関数
	void StopSound(SoundHandleX handle);//音の停止
	void SetSoundVolume(SoundHandleX handle, float volume);//音量変更
	void SetSoundPitch(SoundHandleX handle, float pitchRatio);// ハンドル指定でのピッチ（再生速度）設定
	void SetSoundLoop(SoundHandleX handle, bool isLoop,float LoopBeginSecond=0, float LoopLengthSecond = 0);// ハンドル指定でのループ設定
	// 再生中のBGMの送り先をリアルタイムに変更する関数(エフェクトを切り替えられる)
	void SetSubmixEffect(SoundHandleX handle, SubmixTargetType Effectactive);
	
	//音のエフェクト本体
	void SetEffect(SoundHandleX handle, EffectType Effectactive);
	//音のループエンド音は止まらない
	void Exitloop(SoundHandleX handle);

	void StoporStratSound(SoundHandleX handle,bool isStop);//true音の一時停止、false再開

	void SetChannelVolumes(SoundHandleX handle, float leftVol, float rightVol);//チャンネル（L/R等）ごとの音量を個別指定します。1ch,2ch音源しか対応していない

	float GetRemainingTime(SoundHandleX handle);//残り時間の取得

	bool IsHandleValid(SoundHandleX handle) const;// ハンドルの有効判定


	// 再生中の音を止める関数
	HRESULT AllStopBGM();
	HRESULT	AllStopSE();

	HRESULT AllSetSEEffectActive(bool Effectactive);




	//全体の音量を設定する関数
	void SetMasterVolume(float volume);
	//BGMの音量を設定する関数
	void SetBGMVolume(float volume);
	//SEの音量を設定する関数
	void SetSEVolume(float volume);
private:


	void BGMLoad();//読み込み
	void SELoad();//

	void EnsureBGMLoaded(BGMList bgm);//BGMの読み込みを確認(読み込みが完了していないなら優先的にロードさせて)終わるまで待つ
	void EnsureSELoaded(SEList se);
	// 優先的にロードしてほしいBGMのID（要求がない場合は -1）
	std::atomic<int> m_priorityBGMIndex{ -1 };
	std::thread m_bgmThread;
	LoadState m_bgmLoadState[static_cast<int>(BGMList::MAX)] = { LoadState::Unloaded };
	bool m_stopBgmThread = false;
	std::mutex m_bgmMutex;//役割: 「データの排他制御（同時に触らせない仕組み）」
	std::condition_variable m_bgmCv;//役割: 「スレッド間の合図（通知）と待機」

	// 優先的にロードしてほしいBGMのID（要求がない場合は -1）
	std::atomic<int> m_prioritySEIndex{ -1 };//std::atomic複数のスレッドから同じ変数に対して同時に読み書きが発生すると、データ競合が生じてプログラムの挙動が未定義になるその対策
	std::thread m_SEThread;
	LoadState m_SELoadState[static_cast<int>(SEList::MAX)] = { LoadState::Unloaded };
	bool m_stopSEThread = false;//読み込みが終わってるか
	std::mutex m_SEMutex;//役割: 「データの排他制御（同時に触らせない仕組み）」
	std::condition_variable m_SECv;//役割: 「スレッド間の合図（通知）と待機」



	
	void AllStop();
	//  XAudio2エンジンのインスタンス（本体）を入れる変数
	Microsoft::WRL::ComPtr<IXAudio2> m_pXAudio2;


	X3DAUDIO_HANDLE m_x3dInstance;     // X3DAudioのインスタンスハンドル
	DWORD           m_channelMask;     // スピーカー構成のマスク
	UINT32          m_masterChannels;  // マスターボイスのチャンネル数




	//
	std::vector<SourceVoice3D*>  pVESourceVoice3D;
	// プレイヤー（リスナー）の位置。Update等で更新できるようにする

	// 動的拡張対応のボイススロット配列
	std::vector<VoiceSlot> m_voiceSlots;


	SoundData BGMData[static_cast<int>(BGMList::MAX)];
	SoundData SEData[static_cast<int>(SEList::MAX)];

	//別スレッドで実行中の非同期処理（タスク）の状態や完了を追跡・管理するためのクラスメンバ変数
	//これが破棄されると非同期処理が完了するまで止まる
	std::future<void> m_BGMTask;//
	std::future<void> m_SETask;
	//get()	非同期処理の完了を待って結果を受け取る（待機＋同期）
	//wait()非同期処理が完了するまで待機する（ブロック）
	//valid()この future オブジェクトが有効な状態（結果を待てる状態）かを判定する
	//wait_for()指定した時間（タイムアウト）だけ完了を待つ
		//wait_for()の戻り値
		//std::future_status::ready	完了している
		//std::future_status::timeout	まだ実行中（0秒で終わらなかった
		//std::future_status::deferred実行が保留されている
};

