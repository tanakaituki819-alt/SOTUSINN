#include "XAudio2.h"


//#include <xaudio2fx.h> 
//#include <xapofx.h>
//#include <cstdio>
//#pragma comment(lib, "xaudio2.lib")

HRESULT XAudio2::CreateMasterVoice()
{
    int i = 0;
    HRESULT hr = m_pXAudio2->CreateMasteringVoice(&m_pMasterVoice);
    if (FAILED(hr)) {
        m_pXAudio2.Reset();
        _ASSERT_EXPR(false, L"マスターボイスを作成失敗");
        return hr;
    }
    XAUDIO2_VOICE_DETAILS masterDetails;
    m_pMasterVoice->GetVoiceDetails(&masterDetails);

    m_masterChannels = masterDetails.InputChannels;

    if (m_masterChannels == 0) {
        m_masterChannels = 2;
    }

    if (m_masterChannels == 1) {
        m_channelMask = SPEAKER_MONO;
    }
    else if (m_masterChannels == 2) {
        m_channelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT; // ステレオ (0x3)
    }
    else if (m_masterChannels == 4) {
        // 4ch環境（前後4方向スピーカー）
        m_channelMask = SPEAKER_SURROUND;
    }
    else if (m_masterChannels == 6) {
        m_channelMask = SPEAKER_5POINT1; // 5.1ch Surround
    }
    else if (m_masterChannels == 8) {
        m_channelMask = SPEAKER_7POINT1; // 7.1ch Surround
    }
    else {
        // 想定外のマルチチャンネル環境時は標準的なステレオ設定に倒すか
        // またはSPEAKER_SURROUND などを指定
        m_channelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
    }

    X3DAudioInitialize(m_channelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dInstance);

    return hr;
}

HRESULT XAudio2::CreateSubmixVoice()
{

    HRESULT hr;


  // 追加ガード: 親サブミックスおよび XAudio2 インスタンスが未作成の場合は安全に弾く
    if (!m_pXAudio2 || !m_pMasterVoice) {
        return E_POINTER;
    }
  

    // マスターボイスへの送り先設定
    XAUDIO2_SEND_DESCRIPTOR masterSend = { 0, m_pMasterVoice };
    XAUDIO2_VOICE_SENDS masterSendList = { 1, &masterSend };

    // BGM管理用サブミックスの作成
    hr = m_pXAudio2->CreateSubmixVoice(&m_pBGMSubmixVoice, 2, 48000, 0, 1, &masterSendList, nullptr);
    if (FAILED(hr)) return hr;

    // SE管理用サブミックスの作成
    hr = m_pXAudio2->CreateSubmixVoice(&m_pSESubmixVoice, 2, 48000, 0, 1, &masterSendList, nullptr);
    if (FAILED(hr)) return hr;

    return hr;
}

HRESULT XAudio2::CreateReverbVoice()
{
    if (!m_pXAudio2 || !m_pMasterVoice || !m_pBGMSubmixVoice || !m_pSESubmixVoice) {
        return E_POINTER;
    }
    //エフェクトの内容
    AudioNameSp::Xaudio2fx_reverb_parameters reverbParams;
    {
        //  追記：リバーブのパラメータを強制的に「ハッキリ聴こえる設定」に変更する
       // XAPOのプリセット（例として分かりやすい大ホール「Default」や「Cave」など）
   
        // 1. まずI3DL2用のパラメータ構造体をプリセットマクロで初期化する
        XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Params = XAUDIO2FX_I3DL2_PRESET_CAVE;
        i3dl2Params.Room = 500; // 例: 元が-1000なら、数値を大きくすると音が大きくなる

        // XAudio2が用意している標準の部屋設定を流し込む
        ReverbConvertI3DL2ToNative(&i3dl2Params, &reverbParams);


    }

    HRESULT hr;
    IUnknown* pBGMXAPO = nullptr;
    hr = XAudio2CreateReverb(&pBGMXAPO);
    if (SUCCEEDED(hr))
    {
        XAUDIO2_EFFECT_DESCRIPTOR descriptor = { 0 };
        descriptor.pEffect = pBGMXAPO;
        descriptor.InitialState = TRUE;
        descriptor.OutputChannels = 2;

        XAUDIO2_EFFECT_CHAIN BGMchain = { 1, &descriptor };

        //  サブミックスに送る
        XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };
        sendDesc.pOutputVoice = m_pBGMSubmixVoice;
        XAUDIO2_VOICE_SENDS submixSendList = { 1, &sendDesc };

        hr = m_pXAudio2->CreateSubmixVoice(
            &m_pBGMReverbSubmixVoice,
            2,
            48000,
            0,
            0,
            &submixSendList,
            &BGMchain
        );

        if (FAILED(hr)) {
            m_pBGMReverbSubmixVoice = nullptr;
            _ASSERT_EXPR(false, L"リバーブサブミックスボイスの作成に失敗しました");
        }
        else {
 

            // サブミックスボイスに対して、0番目のエフェクト（リバーブ）にパラメータを適用
            m_pBGMReverbSubmixVoice->SetEffectParameters(0, &reverbParams, sizeof(reverbParams));

            m_pBGMReverbSubmixVoice->SetVolume(1.0f);
            float levelMatrix[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

            m_pBGMReverbSubmixVoice->SetOutputMatrix(m_pBGMSubmixVoice, 2, 2, levelMatrix);
            m_pBGMReverbSubmixVoice->SetVolume(1.0f);
        }
    pBGMXAPO->Release();
    }


    IUnknown* pSEXAPO = nullptr;
    hr = XAudio2CreateReverb(&pSEXAPO);
    if (SUCCEEDED(hr))
    {
        XAUDIO2_EFFECT_DESCRIPTOR descriptor = { 0 };
        descriptor.pEffect = pSEXAPO;
        descriptor.InitialState = TRUE;
        descriptor.OutputChannels = 2;

        XAUDIO2_EFFECT_CHAIN SEchain = { 1, &descriptor };

        //  修正：SE用のディスクリプタとリストを、セットで新しく組み直す！
        XAUDIO2_SEND_DESCRIPTOR seSendDesc = { 0 };
        seSendDesc.pOutputVoice = m_pSESubmixVoice; // ちゃんとSEの音量部屋を指定
        XAUDIO2_VOICE_SENDS seSubmixSendList = { 1, &seSendDesc };
        hr = m_pXAudio2->CreateSubmixVoice(
            &m_pSEReverbSubmixVoice,
            2,
            48000,
            0,
            0,
            &seSubmixSendList,
            &SEchain
        );
        if (FAILED(hr)) {
            m_pSEReverbSubmixVoice = nullptr;
            _ASSERT_EXPR(false, L"リバーブサブミックスボイスの作成に失敗しました");
        }
        else {

            // サブミックスボイスに対して、0番目のエフェクト（リバーブ）にパラメータを適用
            m_pSEReverbSubmixVoice->SetEffectParameters(0, &reverbParams, sizeof(reverbParams));
        }
    pSEXAPO->Release();
    }

    return hr;
}

HRESULT XAudio2::CreateEchoVoice()
{
    if (!m_pXAudio2 || !m_pBGMSubmixVoice || !m_pSESubmixVoice|| !m_pSESubmixVoice) {
        return E_POINTER;
    }
    //エフェクトの内容
    AudioNameSp::Xaudio2fx_reverb_parameters reverbParams;
    {
        // 路地（Alley）や山（Mountains）のプリセットをベースにする
         //    屋外の広い場所で音がやまびこのように返ってくる挙動に近いベース設定
        AudioNameSp::xAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Params = XAUDIO2FX_I3DL2_PRESET_ALLEY;

        // 音量をハッキリ聴こえる設定に変更
        i3dl2Params.Room = 1000;              // 部屋全体の残響音量（最大 0 mB）

        // エコー感を出すための微調整
        i3dl2Params.Reflections = -2000;   // 初期反射音量を極力下げて、ごちゃつき感をなくす
        i3dl2Params.ReflectionsDelay = 0.25f; // 反射の遅延時間（秒単位: 250ms）。長めにとることで「跳ね返り」を作る

        i3dl2Params.Reverb = 0;            // メインの跳ね返り音（ウェット音）の音量
        i3dl2Params.ReverbDelay = 0.10f;   // 反射からさらに遅れて響く時間（100ms）

        // XAudio2が用意している標準の部屋設定を流し込む
        ReverbConvertI3DL2ToNative(&i3dl2Params, &reverbParams);
        //「残響時間（DecayTime）」を長めに調整してエコーを引く
        // （ 2.5秒〜4.0秒程度に伸ばすと「ヤマビコ」のように残響が尾を引きます）
        reverbParams.DecayTime = 3.5f;
    }

    HRESULT hr;
    IUnknown* pBGMXAPO = nullptr;
    hr = XAudio2CreateReverb(&pBGMXAPO);
    if (SUCCEEDED(hr))
    {
        XAUDIO2_EFFECT_DESCRIPTOR descriptor = { 0 };
        descriptor.pEffect = pBGMXAPO;
        descriptor.InitialState = TRUE;
        descriptor.OutputChannels = 2;

        XAUDIO2_EFFECT_CHAIN BGMchain = { 1, &descriptor };

        //  サブミックスに送る
        XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };
        sendDesc.pOutputVoice = m_pBGMSubmixVoice;
        XAUDIO2_VOICE_SENDS submixSendList = { 1, &sendDesc };

        hr = m_pXAudio2->CreateSubmixVoice(
            &m_pBGMEchoSubmixVoice,
            2,
            48000,
            0,
            0,
            &submixSendList,
            &BGMchain
        );

        if (FAILED(hr)) {
            m_pBGMEchoSubmixVoice = nullptr;
            _ASSERT_EXPR(false, L"リバーブサブミックスボイスの作成に失敗しました");
        }
        else {


            // サブミックスボイスに対して、0番目のエフェクト（リバーブ）にパラメータを適用
            m_pBGMEchoSubmixVoice->SetEffectParameters(0, &reverbParams, sizeof(reverbParams));
         
        }
        pBGMXAPO->Release();
    }


    IUnknown* pSEXAPO = nullptr;
    hr = XAudio2CreateReverb(&pSEXAPO);
    if (SUCCEEDED(hr))
    {
        XAUDIO2_EFFECT_DESCRIPTOR descriptor = { 0 };
        descriptor.pEffect = pSEXAPO;
        descriptor.InitialState = TRUE;
        descriptor.OutputChannels = 2;

        XAUDIO2_EFFECT_CHAIN SEchain = { 1, &descriptor };

        //  修正：SE用のディスクリプタとリストを、セットで新しく組み直す！
        XAUDIO2_SEND_DESCRIPTOR seSendDesc = { 0 };
        seSendDesc.pOutputVoice = m_pSESubmixVoice; // ちゃんとSEの音量部屋を指定
        XAUDIO2_VOICE_SENDS seSubmixSendList = { 1, &seSendDesc };
        hr = m_pXAudio2->CreateSubmixVoice(
            &m_pSEEchoSubmixVoice,
            2,
            48000,
            0,
            0,
            &seSubmixSendList,
            &SEchain
        );
        if (FAILED(hr)) {
            m_pSEEchoSubmixVoice = nullptr;
            _ASSERT_EXPR(false, L"リバーブサブミックスボイスの作成に失敗しました");
        }
        else {

            // サブミックスボイスに対して、0番目のエフェクト（リバーブ）にパラメータを適用
            m_pSEEchoSubmixVoice->SetEffectParameters(0, &reverbParams, sizeof(reverbParams));

         
        }
        pSEXAPO->Release();
    }

    return hr;
}

XAudio2::XAudio2()
    :m_pXAudio2(nullptr)
    , m_x3dInstance()
	, m_channelMask(0)
	, m_masterChannels(0)
    , pVESourceVoice3D()
{
    RtlZeroMemory(&m_listener, sizeof(X3DAUDIO_LISTENER));
    
	m_listener.Position = { 0.0f, 0.0f, 0.0f };// リスナーの位置を原点に設定
	m_listener.OrientFront = { 0.0f, 0.0f, 1.0f };// リスナーの前方ベクトルをZ軸方向に設定
	m_listener.OrientTop = { 0.0f, 1.0f, 0.0f };// リスナーの上方向ベクトルをY軸方向に設定
    //(リスナーとは、音を聞く人の位置や向きを表すオブジェクト（プレイヤーのこと）)
    for (int i = 0; i < static_cast<int>(BGMList::MAX); i++) {
        BGMData[i] = {};
        
    }
    for (int i = 0; i < static_cast<int>(SEList::MAX); i++) {
        SEData[i] = {};
    }
}

XAudio2::~XAudio2()
{
    // 1. まずバックグラウンドスレッドを安全に停止・終了させる
    m_stopBgmThread = true;  // スレッドループの停止フラグを立てる
    m_bgmCv.notify_all();    // waitで停止している可能性があるため起こす

    if (m_bgmThread.joinable()) {
        m_bgmThread.join();  // スレッドが完全に終わるまで待機
    }
    m_stopSEThread = true;  // スレッドループの停止フラグを立てる
    m_SECv.notify_all();    // waitで停止している可能性があるため起こす

    if (m_SEThread.joinable()) {
        m_SEThread.join();  // スレッドが完全に終わるまで待機
    }

    AllStop();
    // 作ったエフェクト部屋・音量部屋もここで明示的に破棄する！
    if (m_pBGMEchoSubmixVoice) {
        m_pBGMEchoSubmixVoice->DestroyVoice();
        m_pBGMEchoSubmixVoice = nullptr;
    }
    if (m_pSEEchoSubmixVoice) {
        m_pSEEchoSubmixVoice->DestroyVoice();
        m_pSEEchoSubmixVoice = nullptr;
    }
    if (m_pBGMReverbSubmixVoice) {
        m_pBGMReverbSubmixVoice->DestroyVoice();
        m_pBGMReverbSubmixVoice = nullptr;
    }
    if (m_pSEReverbSubmixVoice) {
        m_pSEReverbSubmixVoice->DestroyVoice();
        m_pSEReverbSubmixVoice = nullptr;
    }
    if (m_pBGMSubmixVoice) {
        m_pBGMSubmixVoice->DestroyVoice();
        m_pBGMSubmixVoice = nullptr;
    }
    if (m_pSESubmixVoice) {
        m_pSESubmixVoice->DestroyVoice();
        m_pSESubmixVoice = nullptr;
    }

    // 最後にマスターボイスも明示的に破棄する
    if (m_pMasterVoice) {
        m_pMasterVoice->DestroyVoice();
        m_pMasterVoice = nullptr;
    }
    //CoUninitializeの前に必要
    m_pXAudio2.Reset();

    CoUninitialize();
}

HRESULT XAudio2::Initialize()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        _ASSERT_EXPR(false, L"COMライブラリを初期化失敗");
        return E_FAIL;
    }
    UINT32 flags = XAUDIO2_DEBUG_ENGINE;
    hr = XAudio2Create(&m_pXAudio2, flags, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        
        _ASSERT_EXPR(false, L"XAudio2エンジン失敗");
        return E_FAIL;
    }
    CreateMasterVoice();
    if (FAILED(hr)) {
        return E_FAIL;
    }

     CreateSubmixVoice();
    if (FAILED(hr)) {
        return E_FAIL;
    }
    CreateReverbVoice();
    if (FAILED(hr)) {
        return E_FAIL;
    }
    CreateEchoVoice();
    if (FAILED(hr)) {
        return E_FAIL;
    }



  

    return S_OK;
}

HRESULT XAudio2::LoadData()
{
    // BGMロード用の非同期タスク
      //m_BGMTask = std::async(std::launch::async, [this]() {
      //  HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);//ライブラリを初期化し、呼び出し元のスレッドを「MTA（マルチスレッドアパートメント）」として設定するため
      //  BGMLoad();
      //  CoUninitialize();
      //  });
      // BGM用バックグラウンドスレッドを立ち上げる
      m_bgmThread = std::thread(&XAudio2::BGMLoad, this);

   


    // SEロード用の非同期タスク
   m_SEThread = std::thread(&XAudio2::SELoad, this);

      //m_SETask = std::async(std::launch::async, [this]() {
      //  HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
      //  SELoad();
      //  CoUninitialize();
      //  });

    // メインスレッド側で両方のタスクが終わるのを待つ
     //bgmTask.wait();
     //seTask.wait();

	return S_OK;
}

HRESULT XAudio2::PlaySample(const SoundData& soundData, D3DXVECTOR3 position)
{
    //今回は使う予定がないのでSEList::SE固定
    HRESULT hr = S_OK;
    
    
    IXAudio2SourceVoice* pSourceVoice = nullptr;
    hr = m_pXAudio2->CreateSourceVoice(&pSourceVoice, &SEData[static_cast<int>(SEList::BAKUHATU)].wfx, XAUDIO2_VOICE_USEFILTER);
    if (FAILED(hr)) return hr;

   

    // 3D計算（X3DAudio）の実行
    X3DAUDIO_EMITTER emitter = { 0 };
    emitter.ChannelCount = 1; // 空間上の一点として計算
    emitter.Position = position;
    emitter.OrientFront = { 0.0f, 0.0f, 1.0f };
    emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
    emitter.CurveDistanceScaler = 1.0f;

    emitter.pChannelAzimuths = nullptr;
    emitter.pVolumeCurve = nullptr;
    emitter.pLFECurve = nullptr;
    emitter.pLPFDirectCurve = nullptr;
    emitter.pReverbCurve = nullptr;

    X3DAUDIO_DSP_SETTINGS dspSettings = { 0 };
    dspSettings.SrcChannelCount = 1;
    dspSettings.DstChannelCount = 2; // ステレオ(2ch)固定

    FLOAT32 matrixCoefficients[8] = { 0 };
    dspSettings.pMatrixCoefficients = matrixCoefficients;

    

    X3DAudioCalculate(m_x3dInstance, &m_listener, &emitter,
        X3DAUDIO_CALCULATE_MATRIX, &dspSettings);

    // 音声の送り先設定（マスターボイスのみに絞る）
    XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };
    sendDesc.pOutputVoice = m_pMasterVoice;
    XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };

    // もしリバーブサブミックスボイスが存在するなら、そこへの出力を手動で完全にゼロ（無音）にします。
    hr = pSourceVoice->SetOutputVoices(&sendList);
    if (FAILED(hr)) {
        pSourceVoice->DestroyVoice();
        return hr;
    }
    
   
    // 修正点：ボイスの「本当の入力チャンネル数」を調べてマトリックスを組み立てる
    XAUDIO2_VOICE_DETAILS voiceDetails;
    pSourceVoice->GetVoiceDetails(&voiceDetails);
    UINT32 srcChannels = voiceDetails.InputChannels; // 1か2が入る


    FLOAT32 finalMatrix[4] = { 0 };
    if (srcChannels == 2) {
        // 音源がステレオ(2ch)だった場合、計算されたL/Rを左右両方のデータに複製して適用する
        finalMatrix[0] = matrixCoefficients[0]; // 左入力 -> 出力L
        finalMatrix[1] = matrixCoefficients[1]; // 左入力 -> 出力R
        finalMatrix[2] = matrixCoefficients[0]; // 右入力 -> 出力L
        finalMatrix[3] = matrixCoefficients[1]; // 右入力 -> 出力R
    }
    else {
        // モノラル(1ch)ならそのまま
        finalMatrix[0] = matrixCoefficients[0];
        finalMatrix[1] = matrixCoefficients[1];
    }

    // 正しい srcChannels を渡して行列を適用
    pSourceVoice->SetOutputMatrix(m_pMasterVoice, srcChannels, 2, finalMatrix);

    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = (UINT32)SEData[static_cast<int>(SEList::BAKUHATU)].audioBuffer.size();
    buffer.pAudioData = SEData[static_cast<int>(SEList::BAKUHATU)].audioBuffer.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    hr = pSourceVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr)) {
        pSourceVoice->DestroyVoice();
        return hr;
    }

    hr = pSourceVoice->Start(0);
    if (FAILED(hr)) {
        pSourceVoice->DestroyVoice();
        return hr;
    }
    pVESourceVoice3D.push_back(new SourceVoice3D{ pSourceVoice, position });

    return hr;
}

void XAudio2::Update()
{
      
      if (m_stopBgmThread) {
          std::cout << "    BGM " << "true" ;
      }
      else {
          std::cout << "    BGM " << "false";
      }
      if (m_stopSEThread) {
          std::cout << "    SE " << "true";
      }
      else {
          std::cout << "    SE " << "false";
      }



    XAUDIO2_VOICE_STATE state;
    // ハンドル管理下のボイスの監視と自動解放
    for (size_t i = 0; i < m_voiceSlots.size(); ++i) {
        VoiceSlot& slot = m_voiceSlots[i];
        if (!slot.isActive || !slot.pVoice) continue;

        XAUDIO2_VOICE_STATE state;
        slot.pVoice->GetState(&state);

        // 再生が終了したら破棄
        if (state.BuffersQueued == 0) {
            slot.pVoice->DestroyVoice();
            slot.pVoice = nullptr;
            slot.isActive = false;
            slot.generation++; // 次回使い回し用に世代を進める
        }
    }

    //for (int i = 0; i < pVESourceVoice.size();) {
    //    pVESourceVoice[i]->GetState(&state);
    //    if (state.BuffersQueued == 0) {
    //        pVESourceVoice[i]->DestroyVoice();
    //        pVESourceVoice.erase(pVESourceVoice.begin() + i);
    //    }
    //    else {
    //        i++;
    //    }
    //}

    for (int i = 0; i < pVESourceVoice3D.size();) {
        pVESourceVoice3D[i]->pSourceVoice->GetState(&state);

        if (state.BuffersQueued == 0) {
            pVESourceVoice3D[i]->pSourceVoice->DestroyVoice();
            delete pVESourceVoice3D[i];
            pVESourceVoice3D.erase(pVESourceVoice3D.begin() + i);
        }
        else {
            X3DAUDIO_EMITTER emitter = { 0 };
            emitter.ChannelCount = 1;
            emitter.Position = pVESourceVoice3D[i]->position;
            emitter.OrientFront = { 0.0f, 0.0f, 1.0f };
            emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
            emitter.CurveDistanceScaler = 3.0f;

            emitter.pChannelAzimuths = nullptr;
            emitter.pVolumeCurve = nullptr;
            emitter.pLFECurve = nullptr;
            emitter.pLPFDirectCurve = nullptr;
            emitter.pReverbCurve = nullptr;

            X3DAUDIO_DSP_SETTINGS dspSettings = { 0 };
            dspSettings.SrcChannelCount = 1;
            dspSettings.DstChannelCount = 2; // ステレオ固定

            static FLOAT32 matrixCoefficients[8] = { 0.0f, 0.0f };
            // 毎回必ずゼロクリアする
            matrixCoefficients[0] = 0.0f;
            matrixCoefficients[1] = 0.0f;
            dspSettings.pMatrixCoefficients = matrixCoefficients;

            X3DAudioCalculate(m_x3dInstance, &m_listener, &emitter,
                X3DAUDIO_CALCULATE_MATRIX, &dspSettings);

            // ボイスごとの正しいチャンネル数にマトリックスを最適化する
            XAUDIO2_VOICE_DETAILS voiceDetails;
            pVESourceVoice3D[i]->pSourceVoice->GetVoiceDetails(&voiceDetails);
            UINT32 srcChannels = voiceDetails.InputChannels;

            FLOAT32 finalMatrix[4] = { 0 };
            if (srcChannels == 2) {
                finalMatrix[0] = matrixCoefficients[0];
                finalMatrix[1] = matrixCoefficients[1];
                finalMatrix[2] = matrixCoefficients[0];
                finalMatrix[3] = matrixCoefficients[1];
            }
            else {
                finalMatrix[0] = matrixCoefficients[0];
                finalMatrix[1] = matrixCoefficients[1];
            }

            pVESourceVoice3D[i]->pSourceVoice->SetOutputMatrix(
                m_pMasterVoice,
                srcChannels,
                2,
                finalMatrix
            );

            // デバッグログ
            char buf[256];
            sprintf_s(buf, "L: %.2f | R: %.2f | EmitPos: (%.1f, %.1f) | ListPos: (%.1f, %.1f)\n",
                matrixCoefficients[0], matrixCoefficients[1],
                emitter.Position.x, emitter.Position.z,
                m_listener.Position.x, m_listener.Position.z);
            OutputDebugStringA(buf);

            i++;
        }
    }

       


}



SoundHandleX XAudio2::BGMPlayHandle(BGMList i, SubmixTargetType Effectactive, bool isLoop)
{
    EnsureBGMLoaded(i);
    // 1.0fが普通。これを20倍～50倍にすると、音がバリバリに歪むよ！
    //pSourceVoice->SetVolume(100.0f);

    // ──────────────────────────────────────────────────
    // 内蔵ローパスフィルタを有効にする
    // ──────────────────────────────────────────────────
    //XAUDIO2_FILTER_PARAMETERS filterParams;
    //filterParams.Type = LowPassFilter; // ローパスフィルタを指定

    //// 💡 どこまでの高い音をカットするか（0.0f ～ 1.0f）
    //// 0.1f くらいにすると、めちゃくちゃ音がこもるよ！
    //filterParams.Frequency = 0.1f;

    //// 💡 音の「クセ（共振：レゾナンス）」（1.0f ～ 6.0f）
    //// 通常は 1.0f（クセなし）でOK。数字を上げると少しミョンミョンした音になる。
    //filterParams.OneOverQ = 1.0f;

    //// ソースボイスにフィルタを適用！
    //pSourceVoice->SetFilterParameters(&filterParams);

    uint32_t targetIndex = UINT32_MAX;

    // 1. 未使用のスロットを探す
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(m_voiceSlots.size()); ++idx) {
        if (!m_voiceSlots[idx].isActive) {
            targetIndex = idx;
            break;
        }
    }

    // 2. 空きがない場合はvectorを動的に拡張する
    if (targetIndex == UINT32_MAX) {
        targetIndex = static_cast<uint32_t>(m_voiceSlots.size());
        m_voiceSlots.push_back(VoiceSlot{});
    }

    VoiceSlot& slot = m_voiceSlots[targetIndex];

    // 3. ボイス生成
    HRESULT hr = m_pXAudio2->CreateSourceVoice(&slot.pVoice, &BGMData[static_cast<int>(i)].wfx, XAUDIO2_VOICE_USEFILTER);
    if (FAILED(hr)) return SoundHandleX{};

    // 4. 出力先サブミックスの設定
    XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };
    switch (Effectactive)
    {
    case SubmixTargetType::Default:
        sendDesc.pOutputVoice = m_pSESubmixVoice; // 通常SE用サブミックス
        break;
    case SubmixTargetType::Reverb:
        sendDesc.pOutputVoice = m_pSEReverbSubmixVoice; // リバーブ用サブミックス
        break;
    case SubmixTargetType::Echo:
        sendDesc.pOutputVoice = m_pSEEchoSubmixVoice; // リバーブ用サブミックス
        break;
    default:
        sendDesc.pOutputVoice = m_pSESubmixVoice; // 通常SE用サブミックス

        break;
    }
    XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };

    hr = slot.pVoice->SetOutputVoices(&sendList);
    if (FAILED(hr)) {
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
        return SoundHandleX{};
    }

    // 5. バッファ送信と再生スタート
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = (UINT32)BGMData[static_cast<int>(i)].audioBuffer.size();
    buffer.pAudioData = BGMData[static_cast<int>(i)].audioBuffer.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.PlayBegin = 0;
    buffer.PlayLength = 0;

        // --- ループ再生のための設定を追加 ---
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE; // 無限ループに設定（1以上を指定すればその回数だけループ）
        buffer.LoopBegin = BGMData[static_cast<int>(i)].wfx.nSamplesPerSec * 0;                     // ループを開始するサンプル位置（最初からなら0）wfx.nSamplesPerSec=1秒
        buffer.LoopLength = 0;                    // ループする長さ（0を指定するとファイル全体をループ）

        //エフェクトチェーンの作成
        {
            //エフェクト（XAPO）オブジェクトを作成
            IUnknown* pReverbEffect = nullptr;
            HRESULT hr = XAudio2CreateReverb(&pReverbEffect);
            if (FAILED(hr)) { /* エラー処理 */ }

            //エフェクト記述子（Descriptor）を設定
            XAUDIO2_EFFECT_DESCRIPTOR descriptor;
            descriptor.pEffect = pReverbEffect;
            descriptor.InitialState = TRUE;             // 最初から有効化しておく
            descriptor.OutputChannels = 2;              // 出力チャンネル数（例: ステレオ）

            //エフェクトチェーン構造体を構築
            XAUDIO2_EFFECT_CHAIN chain;
            chain.EffectCount = 1;                     // エフェクトの数
            chain.pEffectDescriptors = &descriptor;    // 記述子配列のアドレス

            // 4. Voiceにエフェクトチェーンをアタッチ
            hr = slot.pVoice->SetEffectChain(&chain);

            // 5. 不要になったCOMオブジェクトの参照を解放
            // (SetEffectChain 内部で AddRef されるため、呼び出し側は Release してOK)
            if (pReverbEffect) {
                pReverbEffect->Release();
            }

            if (FAILED(hr)) {
                // エラーハンドリング
            }
        }


        hr = slot.pVoice->Start(0);
        if (isLoop) {
            slot.pVoice->ExitLoop();
        }



    hr = slot.pVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr)) {
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
        slot.Type = SoundType::BGM;
        return SoundHandleX{};
    }

    hr = slot.pVoice->Start(0);
    if (FAILED(hr)) {
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
        slot.Type = SoundType::BGM;
        return SoundHandleX{};
    }
    slot.Type = SoundType::BGM;
    slot.isActive = true;
    slot.SoundNo = static_cast<int>(i);
    // 6. ハンドルを作成して返す
    SoundHandleX handle;
    handle.id = targetIndex;
    handle.generation = slot.generation;

    return handle;
}

SoundHandleX XAudio2::SEPlayHandle(SEList i, SubmixTargetType Effectactive, bool isLoop)
{
    EnsureSELoaded(i);

    uint32_t targetIndex = UINT32_MAX;

    // 1. 未使用のスロットを探す
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(m_voiceSlots.size()); ++idx) {
        if (!m_voiceSlots[idx].isActive) {
            targetIndex = idx;
            break;
        }
    }

    // 2. 空きがない場合はvectorを動的に拡張する
    if (targetIndex == UINT32_MAX) {
        targetIndex = static_cast<uint32_t>(m_voiceSlots.size());
        m_voiceSlots.push_back(VoiceSlot{});
    }

    VoiceSlot& slot = m_voiceSlots[targetIndex];

    // 3. ボイス生成
    HRESULT hr = m_pXAudio2->CreateSourceVoice(&slot.pVoice, &SEData[static_cast<int>(i)].wfx, XAUDIO2_VOICE_USEFILTER);
    if (FAILED(hr)) return SoundHandleX{};

    // 4. 出力先サブミックスの設定
    XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };
    switch (Effectactive)
    {
    case SubmixTargetType::Default:
        sendDesc.pOutputVoice = m_pSESubmixVoice; // 通常SE用サブミックス
        break;
    case SubmixTargetType::Reverb:
        sendDesc.pOutputVoice = m_pSEReverbSubmixVoice; // リバーブ用サブミックス
        break;
    case SubmixTargetType::Echo:
        sendDesc.pOutputVoice = m_pSEEchoSubmixVoice; // リバーブ用サブミックス
        break;
    default:
        sendDesc.pOutputVoice = m_pSESubmixVoice; // 通常SE用サブミックス

        break;
    }

    XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };

    hr = slot.pVoice->SetOutputVoices(&sendList);
    if (FAILED(hr)) {
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
        return SoundHandleX{};
    }

    // 5. バッファ送信と再生スタート
    XAUDIO2_BUFFER buffer = { 0 };

    buffer.AudioBytes = (UINT32)SEData[static_cast<int>(i)].audioBuffer.size();
    buffer.pAudioData = SEData[static_cast<int>(i)].audioBuffer.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.PlayBegin = 0;
    buffer.PlayLength = 0;

        // --- ループ再生のための設定を追加 ---
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE; // 無限ループに設定（1以上を指定すればその回数だけループ）
    buffer.LoopBegin = SEData[static_cast<int>(i)].wfx.nSamplesPerSec*0;                     // ループを開始するサンプル位置（最初からなら0）wfx.nSamplesPerSec=1秒
    buffer.LoopLength = 0;                    // ループする長さ（0を指定するとファイル全体をループ）
   


    hr = slot.pVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(hr)) {
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
        slot.Type = SoundType::SE;
        return SoundHandleX{};
    }
    //エフェクトチェーンの作成
    {
        //エフェクト（XAPO）オブジェクトを作成
        IUnknown* pReverbEffect = nullptr;
        HRESULT hr = XAudio2CreateReverb(&pReverbEffect);
        if (FAILED(hr)) { /* エラー処理 */ }

        //エフェクト記述子（Descriptor）を設定
        XAUDIO2_EFFECT_DESCRIPTOR descriptor;
        descriptor.pEffect = pReverbEffect;
        descriptor.InitialState = TRUE;             // 最初から有効化しておく
        descriptor.OutputChannels = 2;              // 出力チャンネル数（例: ステレオ）

        //エフェクトチェーン構造体を構築
        XAUDIO2_EFFECT_CHAIN chain;
        chain.EffectCount = 1;                     // エフェクトの数
        chain.pEffectDescriptors = &descriptor;    // 記述子配列のアドレス

        // 4. Voiceにエフェクトチェーンをアタッチ
        hr = slot.pVoice->SetEffectChain(&chain);

        // 5. 不要になったCOMオブジェクトの参照を解放
        // (SetEffectChain 内部で AddRef されるため、呼び出し側は Release してOK)
        if (pReverbEffect) {
            pReverbEffect->Release();
        }

        if (FAILED(hr)) {
            // エラーハンドリング
        }
    }
  

    hr = slot.pVoice->Start(0);
    if (isLoop) {
        slot.pVoice->ExitLoop();
    }
    if (FAILED(hr)) {
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
        slot.Type = SoundType::SE;
        return SoundHandleX{};
    }

    slot.isActive = true;
    slot.Type = SoundType::SE;
    slot.SoundNo = static_cast<int>(i);

    // 6. ハンドルを作成して返す
    SoundHandleX handle;
    handle.id = targetIndex;
    handle.generation = slot.generation;

    return handle;
}

void XAudio2::StopSound(SoundHandleX handle)
{
    if (!IsHandleValid(handle)) return;

    VoiceSlot& slot = m_voiceSlots[handle.id];
    if (slot.pVoice) {
        slot.pVoice->Stop(0);
        slot.pVoice->FlushSourceBuffers();
        slot.pVoice->DestroyVoice();
        slot.pVoice = nullptr;
    }

    slot.isActive = false;
    slot.generation++; // 世代を進めて古いハンドルを無効化
}

void XAudio2::SetSoundVolume(SoundHandleX handle, float volume)
{
    if (!IsHandleValid(handle)) return;
    m_voiceSlots[handle.id].pVoice->SetVolume(volume);
}

void XAudio2::SetSoundPitch(SoundHandleX handle, float pitchRatio)
{
    if (!IsHandleValid(handle)) return;
    m_voiceSlots[handle.id].pVoice->SetFrequencyRatio(pitchRatio);
}

void XAudio2::SetSoundLoop(SoundHandleX handle, bool isLoop, float LoopBeginSecond, float LoopLengthSecond)
{
    if (!IsHandleValid(handle)) return;

        if (!isLoop) {
            m_voiceSlots[handle.id].pVoice->ExitLoop();
            return;
        }

    // 1. 現在の再生位置を取得
    XAUDIO2_VOICE_STATE state;
    m_voiceSlots[handle.id].pVoice->GetState(&state);
    // state.SamplesPlayed には「これまで再生された総サンプル数」が入る

    //現在のバッファキューをクリア（これで一旦停止状態になる）
    m_voiceSlots[handle.id].pVoice->FlushSourceBuffers();

    // 総サンプル数を計算 (バイト数 ÷ 1サンプルあたりのバイト数)
    UINT32 totalSamples = static_cast<UINT32>(SEData[m_voiceSlots[handle.id].SoundNo].audioBuffer.size() / SEData[m_voiceSlots[handle.id].SoundNo].wfx.nBlockAlign);

    // 現在のファイル内サンプル位置（剰余で範囲内に収める）
    UINT32 currentSamplePos = 0;
    if (totalSamples > 0) {
        currentSamplePos = static_cast<UINT32>(state.SamplesPlayed % totalSamples);
    }

    // バッファ送信と再生スタート
    XAUDIO2_BUFFER buffer = { 0 };

    buffer.AudioBytes = (UINT32)SEData[m_voiceSlots[handle.id].SoundNo].audioBuffer.size();
    buffer.pAudioData = SEData[m_voiceSlots[handle.id].SoundNo].audioBuffer.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.PlayBegin = state.SamplesPlayed;
    buffer.PlayLength = 0;
    if (isLoop) {
        UINT32 sampleRate = SEData[m_voiceSlots[handle.id].SoundNo].wfx.nSamplesPerSec;
        // --- ループ再生のための設定を追加 ---
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE; // 無限ループに設定（1以上を指定すればその回数だけループ）
        buffer.LoopBegin = static_cast<UINT32>(sampleRate * LoopBeginSecond);                     // ループを開始するサンプル位置（最初からなら0）wfx.nSamplesPerSec=1秒
        buffer.LoopLength = static_cast<UINT32>(sampleRate * LoopLengthSecond);                    // ループする長さ（0を指定するとLoopBegin からバッファの最後までをループする）
        // 全体ループ（LoopLengthSecond <= 0）の場合は 0 を指定して末尾までループさせる
        if (LoopLengthSecond <= 0.0f) {
            buffer.LoopLength = 0;
        }
    }
    m_voiceSlots[handle.id].pVoice->SubmitSourceBuffer(&buffer);
    m_voiceSlots[handle.id].pVoice->Start(0);
}





void XAudio2::SetMasterVolume(float volume) {
    if (m_pMasterVoice) {
        m_pMasterVoice->SetVolume(volume);
    }

}

void XAudio2::AllStop()
{
    // 1. 通常のSEをすべて破棄してクリア
    for (auto& slot : m_voiceSlots) {
        if (slot.pVoice) {
            slot.pVoice->DestroyVoice();
            slot.pVoice = nullptr;
        }
        slot.isActive = false;
    }
 
    // 2. 3D音声をすべて破棄し、動的メモリを解放してクリア
    for (size_t i = 0; i < pVESourceVoice3D.size(); ++i) {
        if (pVESourceVoice3D[i]) {
            if (pVESourceVoice3D[i]->pSourceVoice) {
                pVESourceVoice3D[i]->pSourceVoice->DestroyVoice();
            }
            SAFE_DELETE(pVESourceVoice3D[i]);
        }
    }
    pVESourceVoice3D.clear(); // 一括クリア


}

void XAudio2::SetSubmixEffect(SoundHandleX handle, SubmixTargetType Effectactive)
{
    if (!IsHandleValid(handle)) return;
  
    XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0 };
    if (m_voiceSlots[handle.id].Type==SoundType::BGM) {
        // 4. 出力先サブミックスの設定

        switch (Effectactive)
        {
        case SubmixTargetType::Default:
            sendDesc.pOutputVoice = m_pBGMSubmixVoice; // 通常SE用サブミックス
            break;
        case SubmixTargetType::Reverb:
            sendDesc.pOutputVoice = m_pBGMReverbSubmixVoice; // リバーブ用サブミックス
            break;
        case SubmixTargetType::Echo:
            sendDesc.pOutputVoice = m_pBGMEchoSubmixVoice; // リバーブ用サブミックス
            break;
        default:
            sendDesc.pOutputVoice = m_pBGMSubmixVoice; // 通常SE用サブミックス

            break;
        }
    }
    if (m_voiceSlots[handle.id].Type == SoundType::SE) {
        // 4. 出力先サブミックスの設定
     
        switch (Effectactive)
        {
        case SubmixTargetType::Default:
            sendDesc.pOutputVoice = m_pSESubmixVoice; // 通常SE用サブミックス
            break;
        case SubmixTargetType::Reverb:
            sendDesc.pOutputVoice = m_pSEReverbSubmixVoice; // リバーブ用サブミックス
            break;
        case SubmixTargetType::Echo:
            sendDesc.pOutputVoice = m_pSEEchoSubmixVoice; // リバーブ用サブミックス
            break;
        default:
            sendDesc.pOutputVoice = m_pSESubmixVoice; // 通常SE用サブミックス

            break;
        }
    }

    XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };

    // 再生中のボイスに対して、リアルタイムに出力先を再設定する
    m_voiceSlots[handle.id].pVoice->SetOutputVoices(&sendList);
}

void XAudio2::SetEffect(SoundHandleX handle, EffectType Effectactive)
{
    if (!IsHandleValid(handle)) return;
   





    // リバーブパラメータ構造体を準備
    XAUDIO2FX_REVERB_PARAMETERS reverbParams = {};
    switch (Effectactive)
    {
    case XAudio2::EffectType::Default:
    {
        //I3DL2 のデフォルトプリセットを取得
        XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Default = XAUDIO2FX_I3DL2_PRESET_DEFAULT;
        //I3DL2 プリセットをネイティブ型に変換
        ReverbConvertI3DL2ToNative(&i3dl2Default, &reverbParams);
    }
        break;
    case XAudio2::EffectType::Underwater:
    {
        reverbParams.WetDryMix = 80.0f;  // 残響音を強めにして浮遊感を出す (80%)
        reverbParams.ReflectionsDelay = 15;     // 壁のハッキリした跳ね返りはほぼ無し (15ms)
        reverbParams.ReflectionsGain = -10.0f; // 初期反射は控えめ (-10dB)
        reverbParams.ReverbDelay = 10;     // 後続残響がすぐ始まる
        reverbParams.ReverbGain = 0.0f;
        reverbParams.DecayTime = 2.5f;   // 水中での音の余韻は少し長め (2.5秒)
        reverbParams.Density = 100.0f; // 密閉された空間感 (100%)
        reverbParams.RoomSize = 40.0f;

        //重要：水中特有の「こもり」を作るフィルター設定
        reverbParams.EarlyDiffusion = 15;     // 音の輪郭をボカして拡散させる
        reverbParams.LateDiffusion = 15;
        reverbParams.LowEQCutoff = 4;      // 低音域をしっかり残す
        reverbParams.LowEQGain = 10;     // 低音を少し強調 (10)
        reverbParams.HighEQCutoff = 0;      // 高音域カットオフを最低に
        reverbParams.HighEQGain = 0;      // 高音域を完全に消衰させてシャープさを奪う (0)
        reverbParams.RoomFilterMain = -2.0f;  // フィルター全体ゲイン
        reverbParams.RoomFilterHF = -30.0f; // 高周波数を大幅カット (-30dB)
    }
        break;
    case XAudio2::EffectType::Occlusion:
    {
        reverbParams.WetDryMix = 100.0f; // 原音の直接音は聞こえないためWet音メイン (100%)
        reverbParams.ReflectionsDelay = 40;     // 回り込んで届くため遅延を入れる (40ms)
        reverbParams.ReflectionsGain = -6.0f;  // 音量は壁で減衰する (-6dB)
        reverbParams.ReverbDelay = 20;
        reverbParams.ReverbGain = -4.0f;
        reverbParams.DecayTime = 1.2f;   // 部屋の響きは短め (1.2秒)
        reverbParams.Density = 80.0f;
        reverbParams.RoomSize = 30.0f;

        // ★重要：壁で遮断される高音域のカット
        reverbParams.EarlyDiffusion = 12;     // 壁を透過・回折するため輪郭は鈍る
        reverbParams.LateDiffusion = 12;
        reverbParams.LowEQCutoff = 6;      // 低音は壁を突き抜けるため残す
        reverbParams.LowEQGain = 8;
        reverbParams.HighEQCutoff = 2;      // 高音域カットオフを低めに設定
        reverbParams.HighEQGain = 2;      // 高音を抑制
        reverbParams.RoomFilterMain = -4.0f;
        reverbParams.RoomFilterHF = -45.0f; // ★壁による吸収で高音域を激しくカット (-45dB)
    }
        break;
    case XAudio2::EffectType::LargeCave:
    {
        reverbParams.WetDryMix = 50.0f;  // 原音と半々 (50%)
        reverbParams.ReflectionsDelay = 80;     // 壁が遠いため最初の反射が遅い (80ms)
        reverbParams.ReflectionsGain = 0.0f;   // 反射音はしっかり届く (0dB)
        reverbParams.ReverbDelay = 40;
        reverbParams.ReverbGain = 0.0f;
        reverbParams.DecayTime = 4.5f;   // 残響が長〜く残る (4.5秒)
        reverbParams.Density = 60.0f;  // ゴツゴツした岩肌感
        reverbParams.RoomSize = 100.0f; // 巨大な空間 (100ft)

        reverbParams.EarlyDiffusion = 4;      // 輪郭をやや残してパキッとした響きにする
        reverbParams.LateDiffusion = 8;
        reverbParams.HighEQCutoff = 10;     // 高音域も残して岩の反響感を出す
        reverbParams.HighEQGain = 8;
        reverbParams.RoomFilterHF = -4.0f;  // 高音も素直に残す (-4dB)
    }
        break;
    default:
    {
        //I3DL2 のデフォルトプリセットを取得
        XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Default = XAUDIO2FX_I3DL2_PRESET_DEFAULT;
        //I3DL2 プリセットをネイティブ型に変換
        ReverbConvertI3DL2ToNative(&i3dl2Default, &reverbParams);
    }

        break;
    }
    m_voiceSlots[handle.id].pVoice->SetEffectParameters(
        0,                                    // 0番目のエフェクト
        &reverbParams,                        // パラメータ構造体
        sizeof(XAUDIO2FX_REVERB_PARAMETERS),  // 構造体のサイズ
        XAUDIO2_COMMIT_NOW                    // 即座に反映
    );
}

void XAudio2::Exitloop(SoundHandleX handle)
{
    if (!IsHandleValid(handle)) return;
    m_voiceSlots[handle.id].pVoice->ExitLoop();
}

void XAudio2::StoporStratSound(SoundHandleX handle, bool isStop)
{
    if (!IsHandleValid(handle)) return;
    if (isStop) {
        m_voiceSlots[handle.id].pVoice->Stop();
    }
    else {
        m_voiceSlots[handle.id].pVoice->Start();
    }
    
}

void XAudio2::SetChannelVolumes(SoundHandleX handle, float leftVol, float rightVol)
{

    // 1. ハンドルおよびボイスの有効性チェック
    if (!IsHandleValid(handle)) return;

    auto& slot = m_voiceSlots[handle.id];
    if (!slot.pVoice) return;


    // 3. ボイスの詳細情報を取得して入力チャンネル数を調べる
    XAUDIO2_VOICE_DETAILS voiceDetails;
    slot.pVoice->GetVoiceDetails(&voiceDetails);

    UINT32 srcChannels = voiceDetails.InputChannels; // 入力ch数 (1 or 2)
    UINT32 destChannels = 2;                         // 出力ch数 (ステレオ2ch想定)

    // 4. チャンネル数に応じたマトリックス配列の構築
    if (srcChannels == 1)
    {

        float matrix[2] = { leftVol, rightVol };

        // pDestinationVoice に nullptr を指定するとデフォルト送信先が適用される
        slot.pVoice->SetOutputMatrix(nullptr, 1, destChannels, matrix);
    }
    else if (srcChannels == 2)
    {
        // --- 2ch (ステレオ) の場合 ---
        // 配列サイズ: 2 x 2 = 4要素 [InL->OutL, InL->OutR, InR->OutL, InR->OutR]
   
        float matrix[4] = {
            leftVol, 0.0f,      // 左入力  -> 出力Left, 出力Right
            0.0f,     rightVol  // 右入力  -> 出力Left, 出力Right
        };

        slot.pVoice->SetOutputMatrix(nullptr, 2, destChannels, matrix);
    }
    else
    {
        // 4chや5.1chなど、1ch/2ch以外の特殊音源の場合は全チャンネル均等出力などで安全に対処
        std::vector<float> matrix(srcChannels * destChannels, 1.0f);
        slot.pVoice->SetOutputMatrix(nullptr, srcChannels, destChannels, matrix.data());
    }

        
}

float XAudio2::GetRemainingTime(SoundHandleX handle)
{
    float Result = 0;
    // 1. ハンドルおよびボイスの有効性チェック
    if (!IsHandleValid(handle)) return Result;

    XAUDIO2_VOICE_STATE state;
    m_voiceSlots[handle.id].pVoice->GetState(&state);

    if (m_voiceSlots[handle.id].Type== SoundType::SE) {
        Result = state.BuffersQueued / SEData[m_voiceSlots[handle.id].SoundNo].wfx.nSamplesPerSec;
    }
    else {
        Result = state.BuffersQueued / BGMData[m_voiceSlots[handle.id].SoundNo].wfx.nSamplesPerSec;
    }

   

    return Result;
}

bool XAudio2::IsHandleValid(SoundHandleX handle) const
{
    if (handle.id >= m_voiceSlots.size()) return false;
    const VoiceSlot& slot = m_voiceSlots[handle.id];
    return slot.isActive && (slot.generation == handle.generation);
}

HRESULT XAudio2::AllStopBGM()
{
    for (auto& slot : m_voiceSlots) {
        if (slot.pVoice && slot.Type == SoundType::BGM) {
            slot.pVoice->DestroyVoice();
            slot.pVoice = nullptr;
        }
        slot.isActive = false;
    }
    return S_OK;
}
HRESULT XAudio2::AllStopSE()
{
    for (auto& slot : m_voiceSlots ) {
        if (slot.pVoice&& slot.Type == SoundType::SE) {
            slot.pVoice->DestroyVoice();
            slot.pVoice = nullptr;
        }
        slot.isActive = false;
    }
    return S_OK;
}

HRESULT XAudio2::AllSetSEEffectActive(bool Effectactive)
{
    HRESULT RESULT = S_OK;
    XAUDIO2_SEND_DESCRIPTOR sendDesc;
    if (Effectactive) {
        sendDesc = { 0, m_pSEReverbSubmixVoice }; // リバーブ部屋へ
    }
    else {
        sendDesc = { 0, m_pSESubmixVoice }; //
    }

    XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };
    for (auto& slot : m_voiceSlots) {
        // 再生中のボイスに対して、リアルタイムに出力先を再設定する
        if (slot.pVoice->SetOutputVoices(&sendList) == E_FAIL) {
            return E_FAIL;
        }
    }

    return S_OK;
}



//通常１
void XAudio2::SetBGMVolume(float volume)
{
    if (m_pBGMSubmixVoice) {
       m_pBGMSubmixVoice->SetVolume(volume);
    }
}

void XAudio2::SetSEVolume(float volume)
{
    if (m_pSESubmixVoice) {
        m_pSESubmixVoice->SetVolume(volume);
    }
}

void XAudio2::BGMLoad()
{

    HRESULT RE= CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    std::vector <Sound> BGMdata;
    BGMdata.push_back({ static_cast<int>(BGMList::BGM1), _T("Data\\Sound\\BGM\\1.mp3") });
    BGMdata.push_back({ static_cast<int>(BGMList::BGM2), _T("Data\\Sound\\BGM\\2.mp3") });
    BGMdata.push_back({ static_cast<int>(BGMList::BGM3), _T("Data\\Sound\\BGM\\3.mp3") });
    BGMdata.push_back({ static_cast<int>(BGMList::BGM4), _T("Data\\Sound\\BGM\\4.mp3") });
    BGMdata.push_back({ static_cast<int>(BGMList::BGM5), _T("Data\\Sound\\BGM\\5.mp3") });
    BGMdata.push_back({ static_cast<int>(BGMList::BGM6), _T("Data\\Sound\\BGM\\6.mp3") });
    BGMdata.push_back({ static_cast<int>(BGMList::BGM7), _T("Data\\Sound\\BGM\\7.mp3") });
    //    BGMdata.push_back();
    std::sort(BGMdata.begin(), BGMdata.end(), [](const auto& a, const auto& b) {
        return a.No < b.No;
        });

    int totalBGM = static_cast<int>(BGMdata.size());

    while (!m_stopBgmThread)
    {
        int targetIdx = -1;

        // 1. 優先ロードのリクエストが飛んできているか確認
        int priority = m_priorityBGMIndex.load();
        if (priority != -1 && m_bgmLoadState[priority] == LoadState::Unloaded) {
            targetIdx = priority;
        }
        else {
            // 2. 優先リクエストがない場合、順番に未ロードのBGMを探す
            for (int i = 0; i < totalBGM; ++i) {
                if (m_bgmLoadState[BGMdata[i].No] == LoadState::Unloaded) {
                    targetIdx = BGMdata[i].No;
                    break;
                }
            }
        }

        // 全てロード完了しているなら終了
        if (targetIdx == -1) {
            m_stopBgmThread = true;
            break;
        }
   // 対象BGMを「ロード中」に変更
        {
            std::lock_guard<std::mutex> lock(m_bgmMutex);
            m_bgmLoadState[targetIdx] = LoadState::Loading;
        }

        // ファイルロード（実際の重いデコード処理）
        LoadMP3(BGMdata[targetIdx].path, BGMData[targetIdx]);

        // ロード完了を通知
        {
            std::lock_guard<std::mutex> lock(m_bgmMutex);
            m_bgmLoadState[targetIdx] = LoadState::Loaded;
            if (m_priorityBGMIndex.load() == targetIdx) {
                m_priorityBGMIndex.store(-1); // 優先要求クリア
            }
        }
           
        m_bgmCv.notify_all(); // 待機中のメインスレッドを起こす
    }
    CoUninitialize();
}

void XAudio2::SELoad()
{
    HRESULT RE = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    std::vector <Sound> SEdata;
    SEdata.push_back({ static_cast<int>(SEList::EI),_T("Data\\Sound\\SE\\エ゛イ゛.mp3") });
    SEdata.push_back({ static_cast<int>(XAudio2::SEList::BAKUHATU),_T("Data\\Sound\\SE\\爆発2.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::SYAKIN),_T("Data\\Sound\\SE\\シャキーン2.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::DBIMU),_T("Data\\Sound\\SE\\気弾のせめぎ合い.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::SRASYU),_T("Data\\Sound\\SE\\剣で斬る6.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::GOGOGO),_T("Data\\Sound\\SE\\ゴゴゴゴ.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::MOVE),_T("Data\\Sound\\SE\\ロボット合体3.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::KIDOU),_T("Data\\Sound\\SE\\ロボットが腕を動かす1.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::MOVE2),_T("Data\\Sound\\SE\\巨大モーター起動.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::SHOT),_T("Data\\Sound\\SE\\ビーム砲1.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::KAIHI),_T("Data\\Sound\\SE\\キックの衣擦れ2.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::SATTA),_T("Data\\Sound\\SE\\巨大シャッターが開く.mp3") });
    SEdata.push_back({ static_cast<int>(SEList::JANP),_T("Data\\Sound\\SE\\ピューンと逃げる.mp3") });

    //SEdata.push_back();


    std::sort(SEdata.begin(), SEdata.end(), [](const auto& a, const auto& b) {
    return a.No < b.No;
    });

    int totalSE = static_cast<int>(SEdata.size());

    while (!m_stopSEThread)
    {
        int targetIdx = -1;

        // 1. 優先ロードのリクエストが飛んできているか確認
        int priority = m_prioritySEIndex.load();
        if (priority != -1 && m_SELoadState[priority] == LoadState::Unloaded) {
            targetIdx = priority;
        }
        else {
            // 2. 優先リクエストがない場合、順番に未ロードのSEを探す
            for (int i = 0; i < totalSE; ++i) {
                if (m_SELoadState[SEdata[i].No] == LoadState::Unloaded) {
                    targetIdx = SEdata[i].No;
                    break;
                }
            }
        }

        // 全てロード完了しているなら終了
        if (targetIdx == -1) {
            m_stopSEThread = true;
            break;
        }
        // 対象BGMを「ロード中」に変更
        {
            std::lock_guard<std::mutex> lock(m_SEMutex);//のスコープが終わるまで、m_SEMutex を使う他の処理を待たせる
            m_SELoadState[targetIdx] = LoadState::Loading;//このファイルがロード中だとわかるようにする
        }

        // ファイルロード（実際の重いデコード処理）
        LoadMP3(SEdata[targetIdx].path, SEData[targetIdx]);

        // ロード完了を通知
        {
            std::lock_guard<std::mutex> lock(m_SEMutex);
            m_SELoadState[targetIdx] = LoadState::Loaded;//このファイルがロード完了だとわかるようにする
            if (m_prioritySEIndex.load() == targetIdx) {
                m_prioritySEIndex.store(-1); // 優先要求クリア
            }
        }

        m_SECv.notify_all(); // 待機中のメインスレッドを起こす
    }
    CoUninitialize();

}

void XAudio2::EnsureBGMLoaded(BGMList bgm)
{
    int idx = static_cast<int>(bgm);

    // すでに完了しているなら何もしない
    if (m_bgmLoadState[idx] == LoadState::Loaded) return;

    // まだロードされていない場合は、優先ロード対象に設定
    if (m_bgmLoadState[idx] == LoadState::Unloaded) {
        m_priorityBGMIndex.store(idx);
    }

    // ワーカースレッドがロードを完了させて Loaded になるまで安全に待機
    std::unique_lock<std::mutex> lock(m_bgmMutex);
    m_bgmCv.wait(lock, [this, idx]() {
        return m_bgmLoadState[idx] == LoadState::Loaded;
        });
}

void XAudio2::EnsureSELoaded(SEList se)
{
    int idx = static_cast<int>(se);

    // すでに完了しているなら何もしない
    if (m_SELoadState[idx] == LoadState::Loaded) return;

    // まだロードされていない場合は、優先ロード対象に設定
    if (m_SELoadState[idx] == LoadState::Unloaded) {
        m_prioritySEIndex.store(idx);
    }

    // ワーカースレッドがロードを完了させて Loaded になるまで安全に待機
    std::unique_lock<std::mutex> lock(m_SEMutex);
    m_SECv.wait(lock, [this, idx]() {
        return m_SELoadState[idx] == LoadState::Loaded;
        });
}
