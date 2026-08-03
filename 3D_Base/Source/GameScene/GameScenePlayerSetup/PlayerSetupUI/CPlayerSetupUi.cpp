#include "CPlayerSetupUI.h"
#include "SpriteManager/CSpriteManager.h"
#include <iostream>

namespace {
	//プレイヤーの背景UIのポジション.
	const D3DXVECTOR3 PLAYER1_UI = { 8.f	, 0.f, 0.f };	//プレイヤー1の背景UI.
	const D3DXVECTOR3 PLAYER2_UI = { 332.8f	, 0.f, 0.f };	//プレイヤー2の背景UI.
	const D3DXVECTOR3 PLAYER3_UI = { 652.8f	, 0.f, 0.f };	//プレイヤー3の背景UI.
	const D3DXVECTOR3 PLAYER4_UI = { 972.8f	, 0.f, 0.f };	//プレイヤー4の背景UI.
	//プレイヤーコントローラーUIのポジション.
	const D3DXVECTOR3 PLAYER1_CONTROLLER_UI = { 154.f, 100.f, 0.f };	//コントローラーUIのポジション.
	const D3DXVECTOR3 PLAYER2_CONTROLLER_UI = { 480.f, 100.f, 0.f };	//コントローラーUIのポジション.
	const D3DXVECTOR3 PLAYER3_CONTROLLER_UI = { 800.f, 100.f, 0.f };	//コントローラーUIのポジション.
	const D3DXVECTOR3 PLAYER4_CONTROLLER_UI = { 1120.f, 100.f, 0.f };	//コントローラーUIのポジション.
	//巻物UIポジション.
	const D3DXVECTOR3 SCROLL_UI_POS	 = { 788.f,683.f,0.f };				//スクロールUIポジション.
	const D3DXVECTOR3 SCROLL_UI2_POS = { 1082.f,683.f,0.f };			//スクロールUIポジション.
	//テキストUIのポジション.
	const D3DXVECTOR3 TEXT_UI_POS = { 700.f, 638.f, 0.f };				//テキストUIのポジション.
	//準備完了キャンセルテキストのポジション.
	const D3DXVECTOR3 TEXT_UI2_POS = { 928.f, 638.f, 0.f };				//準備完了UIのポジション.
	//プレイヤーの背景UIのスケール.
	const D3DXVECTOR3 PLAYER_SCALE				= { 300.f, 630.f, 0.f };				//プレイヤー背景UIのスケール.
	const D3DXVECTOR3 PLAYER_NAME_SCALE			= { 300.f, 75.f, 0.f };					//プレイヤーネームのスケール.
	const D3DXVECTOR3 PLAYER_CONTROLLER_SCALE	= { 150.f, 75.f, 0.f };					//プレイヤーネームのスケール.
	const D3DXVECTOR3 TEXT_UI_SCALE				= { 300.f, 100.f, 0.f };				//テキストUIのスケール.
	const D3DXVECTOR3 SCROLL_UI_SCALE			= { 210.f, 100.f, 0.f };
	const D3DXVECTOR3 SCROLL_UI2_SCALE			= { 380.f, 100.f, 0.f };
	//ローテーション.
	const D3DXVECTOR3 ROTATION_NONE = { 0.f, 0.f, 0.f };		//回転が必要ないときのローテーション.

	//アルファ.
	constexpr float	ALPHA_OFF		= 0.2f;		//接続時アルファ.
	constexpr float	ALPHA_CONNECTED = 1.0f;		//接続時アルファ.

}


CPlayerSetupUI::CPlayerSetupUI()
{
	//3つのスプライトを設定.
	for (int i = 0; i < Ui_Max; i++) {
		m_pPlayerSetUpUI[i]		 = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);	//背景.
		m_pPlayerNameUI[i]		 = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Playerfont);			//プレイヤー名前.
		m_pPlayercontrollerUI[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_controller);			//コントローラーUI.
	}
	//テキストスプライトを設定.
	for (int i = 0; i < Text_Max; i++) {
		m_pTextUI[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);						//テキスト(文字).
		m_pScrollUI[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);						//巻物.
	}
}

CPlayerSetupUI::~CPlayerSetupUI()
{
}

void CPlayerSetupUI::Update()
{
}

void CPlayerSetupUI::Draw()
{

	PlayerBackUI();
	PlayerNameUI();
	ControllerUI();
	ScrollUI();
	TextUI();
}

//プレイヤーの背景UI.
void CPlayerSetupUI::PlayerBackUI()
{
	//プレイヤー1の背景.
	m_pPlayerSetUpUI[0]->SetPosition(D3DXVECTOR3(PLAYER1_UI));
	m_pPlayerSetUpUI[0]->SetScale(D3DXVECTOR3(PLAYER_SCALE));
	m_pPlayerSetUpUI[0]->SetPatternNo(0.f, 0.f);
	m_pPlayerSetUpUI[0]->Render();
	//プレイヤー2の背景.
	m_pPlayerSetUpUI[1]->SetPosition(D3DXVECTOR3(PLAYER2_UI));
	m_pPlayerSetUpUI[1]->SetScale(D3DXVECTOR3(PLAYER_SCALE));
	m_pPlayerSetUpUI[1]->SetPatternNo(1.f, 0.f);
	m_pPlayerSetUpUI[1]->Render();
	//プレイヤー3の背景.
	m_pPlayerSetUpUI[2]->SetPosition(D3DXVECTOR3(PLAYER3_UI));
	m_pPlayerSetUpUI[2]->SetScale(D3DXVECTOR3(PLAYER_SCALE));
	m_pPlayerSetUpUI[2]->SetPatternNo(2.f, 0.f);
	m_pPlayerSetUpUI[2]->Render();
	//プレイヤー4の背景.
	m_pPlayerSetUpUI[3]->SetPosition(D3DXVECTOR3(PLAYER4_UI));
	m_pPlayerSetUpUI[3]->SetScale(D3DXVECTOR3(PLAYER_SCALE));
	m_pPlayerSetUpUI[3]->SetPatternNo(3.f, 0.f);
	m_pPlayerSetUpUI[3]->Render();

}
//プレイヤーの名前UI.
void CPlayerSetupUI::PlayerNameUI()
{
	//プレイヤー1の名前.
	m_pPlayerNameUI[0]->SetPosition(D3DXVECTOR3(PLAYER1_UI));
	m_pPlayerNameUI[0]->SetScale(D3DXVECTOR3(PLAYER_NAME_SCALE));
	m_pPlayerNameUI[0]->SetPatternNo(0.f, 0.f);
	m_pPlayerNameUI[0]->Render();
	//プレイヤー2の名前.
	m_pPlayerNameUI[1]->SetPosition(D3DXVECTOR3(PLAYER2_UI));
	m_pPlayerNameUI[1]->SetScale(D3DXVECTOR3(PLAYER_NAME_SCALE));
	m_pPlayerNameUI[1]->SetPatternNo(0.f, 1.f);
	m_pPlayerNameUI[1]->Render();
	//プレイヤー3の名前.
	m_pPlayerNameUI[2]->SetPosition(D3DXVECTOR3(PLAYER3_UI));
	m_pPlayerNameUI[2]->SetScale(D3DXVECTOR3(PLAYER_NAME_SCALE));
	m_pPlayerNameUI[2]->SetPatternNo(0.f, 2.f);
	m_pPlayerNameUI[2]->Render();
	//プレイヤー4の名前.
	m_pPlayerNameUI[3]->SetPosition(D3DXVECTOR3(PLAYER4_UI));
	m_pPlayerNameUI[3]->SetScale(D3DXVECTOR3(PLAYER_NAME_SCALE));
	m_pPlayerNameUI[3]->SetPatternNo(0.f, 3.f);
	m_pPlayerNameUI[3]->Render();
}
//プレイヤーのコントローラーUI.
void CPlayerSetupUI::ControllerUI()
{
	//プレイヤー1.
	m_pPlayercontrollerUI[0]->SetAlpha(ALPHA_OFF);
	m_pPlayercontrollerUI[0]->SetPosition(D3DXVECTOR3(PLAYER1_CONTROLLER_UI));
	m_pPlayercontrollerUI[0]->SetScale(D3DXVECTOR3(PLAYER_CONTROLLER_SCALE));
	//コントローラーが接続されているなら.
	if (m_pMyController[0]->IsConnect()) {
		m_pPlayercontrollerUI[0]->SetAlpha(ALPHA_CONNECTED);
	}

	m_pPlayercontrollerUI[0]->Render();
	//プレイヤー2.
	m_pPlayercontrollerUI[1]->SetAlpha(ALPHA_OFF);
	m_pPlayercontrollerUI[1]->SetPosition(D3DXVECTOR3(PLAYER2_CONTROLLER_UI));
	m_pPlayercontrollerUI[1]->SetScale(D3DXVECTOR3(PLAYER_CONTROLLER_SCALE));
	//コントローラーが接続されているなら.
	if (m_pMyController[1]->IsConnect()) {
		m_pPlayercontrollerUI[1]->SetAlpha(ALPHA_CONNECTED);
	}
	m_pPlayercontrollerUI[1]->Render();
	//プレイヤー3.
	m_pPlayercontrollerUI[2]->SetAlpha(ALPHA_OFF);
	m_pPlayercontrollerUI[2]->SetPosition(D3DXVECTOR3(PLAYER3_CONTROLLER_UI));
	m_pPlayercontrollerUI[2]->SetScale(D3DXVECTOR3(PLAYER_CONTROLLER_SCALE));
	//コントローラーが接続されているなら.
	if (m_pMyController[2]->IsConnect()) {
		m_pPlayercontrollerUI[2]->SetAlpha(ALPHA_CONNECTED);
	}
	m_pPlayercontrollerUI[2]->Render();
	//プレイヤー4.
	m_pPlayercontrollerUI[3]->SetAlpha(ALPHA_OFF);
	m_pPlayercontrollerUI[3]->SetPosition(D3DXVECTOR3(PLAYER4_CONTROLLER_UI));
	m_pPlayercontrollerUI[3]->SetScale(D3DXVECTOR3(PLAYER_CONTROLLER_SCALE));
	//コントローラーが接続されているなら.
	if (m_pMyController[3]->IsConnect()) {
		m_pPlayercontrollerUI[3]->SetAlpha(ALPHA_CONNECTED);
	}
	m_pPlayercontrollerUI[3]->Render();
}
//テキストUI.
void CPlayerSetupUI::TextUI()
{	
	//タイトルに戻るUI.
	m_pTextUI[0]->SetPosition(D3DXVECTOR3(TEXT_UI_POS));
	m_pTextUI[0]->SetScale(D3DXVECTOR3(TEXT_UI_SCALE));
	m_pTextUI[0]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
	m_pTextUI[0]->SetPatternNo(0.f, 2.f);
	m_pTextUI[0]->Render();
	//準備完了キャンセルUI.
	m_pTextUI[1]->SetPosition(D3DXVECTOR3(TEXT_UI2_POS));
	m_pTextUI[1]->SetScale(D3DXVECTOR3(TEXT_UI_SCALE));
	m_pTextUI[1]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
	m_pTextUI[1]->SetPatternNo(0.f, 1.f);
	m_pTextUI[1]->Render();
}
//巻物UI.
void CPlayerSetupUI::ScrollUI()
{
	//タイトルに戻るUI.
	m_pScrollUI[0]->SetPosition(D3DXVECTOR3(SCROLL_UI_POS));
	m_pScrollUI[0]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
	m_pScrollUI[0]->SetScale(D3DXVECTOR3(SCROLL_UI_SCALE));
	m_pScrollUI[0]->Render();
	//準備完了キャンセルUI.
	m_pScrollUI[1]->SetPosition(D3DXVECTOR3(SCROLL_UI2_POS));
	m_pScrollUI[1]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
	m_pScrollUI[1]->SetScale(D3DXVECTOR3(SCROLL_UI2_SCALE));
	m_pScrollUI[1]->Render();
}
