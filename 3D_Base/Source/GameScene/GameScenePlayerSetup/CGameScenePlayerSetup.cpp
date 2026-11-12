#include "CGameScenePlayerSetup.h"
#include "Game/CGame.h"
#include "GameObject/UIObject/CUIObject.h"
#include "CharacterUI/CCharacterUI.h"
#include "PlayerSetupUI/CPlayerSetupUI.h"
#include "GameRdyUI/CGameRdyUI.h"

namespace {
	constexpr float BGM_VOLUME = 0.5f;	//BGMの音量.
}

CGameScenePlayerSetup::CGameScenePlayerSetup(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	: CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pBackImg(std::make_unique<CUIObject>())
	, m_pCharacterUI(std::make_unique<CCharacterUI>())
	, m_pPlayerSetupUI(std::make_unique<CPlayerSetupUI>())
	, m_pGameRdyUI(std::make_unique<CGameRdyUI>())
{
	//和室の背景を、画面全体に表示する.
	m_pBackImg->AttachSprite(*CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround));
	m_pBackImg->SetPosition(0, 0, 0);
	m_pBackImg->SetScale(WND_W, WND_H, 0);

	//BGM再生.
	const SoundHandleX bgmHandle = XAudio2::GetInstance()->BGMPlayHandle(XAudio2::BGMList::PlayerSetUp);
	XAudio2::GetInstance()->SetSoundVolume(bgmHandle, BGM_VOLUME);
}

CGameScenePlayerSetup::~CGameScenePlayerSetup()
{
	XAudio2::GetInstance()->AllStopBGM();
	XAudio2::GetInstance()->AllStopSE();
}

void CGameScenePlayerSetup::StartFinalSetup()
{
	//各UIにコントローラーを渡す.
	for (int i = 0; i < Controller_Max; i++) {
		m_pPlayerSetupUI->SetXInput(m_pController[i], i);
		m_pCharacterUI->SetXInput(m_pController[i], i);
		m_pGameRdyUI->SetXInput(m_pController[i], i);
	}
}

void CGameScenePlayerSetup::Update()
{
	for (int i = 0; i < Controller_Max; i++) {
		if (m_pController[i] != nullptr) {
			m_pController[i]->Update();
		}
	}

	//シーン切り替えを始めたら、それ以降の操作は受け付けない.
	if (m_IsSceneChanging) return;

	//前のフレームでUIが出した切り替えの合図を確認する.
	if (m_pGameRdyUI->GetMainSceneChangeflag()) {
		ChangeToGameMain();
		return;
	}
	if (m_pGameRdyUI->GetTitleSceneChangeflag()) {
		ChangeToTitle();
		return;
	}

	m_pGameRdyUI->Update();
	m_pPlayerSetupUI->Update();
	m_pCharacterUI->Update();
}

void CGameScenePlayerSetup::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効(UIは描いた順に重なる).
	m_pBackImg->Draw();			//和室の背景.
	m_pPlayerSetupUI->Draw();	//プレイヤーの背景・名前・コントローラーアイコン.
	m_pCharacterUI->Draw();		//キャラクター.
	m_pGameRdyUI->Draw();		//準備完了とタイトルに戻る画面(一番手前).
	m_pDx11->SetDepth(true);	//深度テスト有効.
}

void CGameScenePlayerSetup::PlayerControllerSet(CGame* pGame)
{
	m_pGame = pGame;
	for (int i = 0; i < Controller_Max; i++) {
		m_pController[i] = pGame->m_pController[i];
	}
}

//ゲームメインへ切り替える.
void CGameScenePlayerSetup::ChangeToGameMain()
{
	m_IsSceneChanging = true;
	SenenChang(enScene::GameMain, CSceneChange::TransitionType::FUSUMA, 60, 60);
	XAudio2::GetInstance()->AllStopBGM();
	RemoveDisconnectedPlayers();	//参加しないプレイヤーは、ゲームメインに持ち込まない.
}

//タイトルへ切り替える.
void CGameScenePlayerSetup::ChangeToTitle()
{
	m_IsSceneChanging = true;
	SenenChang(enScene::Title, CSceneChange::TransitionType::FUSUMA, 60, 120, 120);
	XAudio2::GetInstance()->AllStopBGM();
}

//未接続のプレイヤーを削除する(1Pは必ず残す).
void CGameScenePlayerSetup::RemoveDisconnectedPlayers()
{
	if (m_pGame == nullptr) return;

	for (int i = 1; i < Controller_Max; i++) {	//1P(0番)は対象外なので1から.
		const bool isConnected = (m_pController[i] != nullptr && m_pController[i]->IsConnect());
		if (isConnected) continue;
		SAFE_DELETE(m_pGame->m_pPlayer[i]);
	}
}