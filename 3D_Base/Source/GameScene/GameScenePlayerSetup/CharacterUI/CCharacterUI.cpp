#include "CCharacterUI.h"
#include "SpriteManager/CSpriteManager.h"
#include <iostream>

namespace {
	//プレイヤー1~4のポジション.
	const D3DXVECTOR3	PLAYER1_DEFAULT_POS = { 14.f, 178.f, 1.f };
	const D3DXVECTOR3	PLAYER2_DEFAULT_POS = { 300.8f,178.f, 1.f };
	const D3DXVECTOR3	PLAYER3_DEFAULT_POS = { 652.8, 148.f, 1.f };
	const D3DXVECTOR3	PLAYER4_DEFAULT_POS = { 972.8, 160.f, 1.f };
	//プレイヤー1~4の影のポジション.
	const D3DXVECTOR3	PLAYER1_SHADOW_POS = { 52.f	 , 178.f, 1.f };
	const D3DXVECTOR3	PLAYER2_SHADOW_POS = { 330.f , 178.f, 1.f };
	const D3DXVECTOR3	PLAYER3_SHADOW_POS = { 682.f , 150.f, 1.f };
	const D3DXVECTOR3	PLAYER4_SHADOW_POS = { 1008.f, 158.f, 1.f };
	//プレイヤー1~4のスケール.
	const D3DXVECTOR3	PLAYER1_DEFAULT_SCALE = { 260.f, 450.f, 0.f};
	const D3DXVECTOR3	PLAYER2_DEFAULT_SCALE = { 260.f, 450.f, 0.f};
	const D3DXVECTOR3	PLAYER3_DEFAULT_SCALE = { 280.f, 480.f, 0.f};
	const D3DXVECTOR3	PLAYER4_DEFAULT_SCALE = { 270.f, 470.f, 0.f};
	//透過値.
	constexpr float		ALPHA_CONNECTED		= 1.0f;				//接続時アルファ.
	constexpr float		ALPHA_DISCONNECTED  = 0.5f;				//非接続時アルファ（本体）.
	constexpr float		ALPHA_SHADOW_OFF	= 0.0f;				//非接続時アルファ（影）.
	const D3DXVECTOR3	SHADOW_COLOR		= { 0.f,0.f,0.f };	//カラー.
}

CCharacterUI::CCharacterUI()
{
	//女将さん.
	m_pPlayerUI[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);
	m_pPlayerUI[0]->SetScale(PLAYER1_DEFAULT_SCALE);
	//男.
	m_pPlayerUI[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);
	m_pPlayerUI[1]->SetScale(PLAYER2_DEFAULT_SCALE);
	//女子高生.
	m_pPlayerUI[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);
	m_pPlayerUI[2]->SetScale(PLAYER3_DEFAULT_SCALE);
	//おばちゃん.
	m_pPlayerUI[3] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);
	m_pPlayerUI[3]->SetScale(PLAYER4_DEFAULT_SCALE);
}

CCharacterUI::~CCharacterUI()
{
}

void CCharacterUI::Update()
{
}

void CCharacterUI::Draw()
{
	CharacterShadowDraw();	//キャラクターの影描画.
	CharacterDraw();		//キャラクターの描画.
}

void CCharacterUI::CharacterDraw()
{
	//本体.
	m_pPlayerUI[0]->SetAlpha(ALPHA_DISCONNECTED);
	m_pPlayerUI[1]->SetAlpha(ALPHA_DISCONNECTED);
	m_pPlayerUI[2]->SetAlpha(ALPHA_DISCONNECTED);
	m_pPlayerUI[3]->SetAlpha(ALPHA_DISCONNECTED);
	//ポジション.
	m_pPlayerUI[0]->SetPosition(PLAYER1_DEFAULT_POS);
	m_pPlayerUI[1]->SetPosition(PLAYER2_DEFAULT_POS);
	m_pPlayerUI[2]->SetPosition(PLAYER3_DEFAULT_POS);
	m_pPlayerUI[3]->SetPosition(PLAYER4_DEFAULT_POS);

	//コントローラーが接続されているなら透過解除.
	if (m_pMyController[0]->IsConnect()) {
		m_pPlayerUI[0]->SetAlpha(ALPHA_CONNECTED);
	}
	if (m_pMyController[1]->IsConnect()) {
		m_pPlayerUI[1]->SetAlpha(ALPHA_CONNECTED);
	}
	if (m_pMyController[2]->IsConnect()) {
		m_pPlayerUI[2]->SetAlpha(ALPHA_CONNECTED);
	}
	if (m_pMyController[3]->IsConnect()) {
		m_pPlayerUI[3]->SetAlpha(ALPHA_CONNECTED);
	}
	for (int i = 0; i < PlayerMax; i++) {
		m_pPlayerUI[i]->Render();
	}
}
//影.
void CCharacterUI::CharacterShadowDraw()
{
	//アルファ値.
	m_pPlayerUI[0]->SetAlpha(ALPHA_SHADOW_OFF);
	m_pPlayerUI[1]->SetAlpha(ALPHA_SHADOW_OFF);
	m_pPlayerUI[2]->SetAlpha(ALPHA_SHADOW_OFF);
	m_pPlayerUI[3]->SetAlpha(ALPHA_SHADOW_OFF);
	//ポジション.
	m_pPlayerUI[0]->SetPosition(PLAYER1_SHADOW_POS);
	m_pPlayerUI[1]->SetPosition(PLAYER2_SHADOW_POS);
	m_pPlayerUI[2]->SetPosition(PLAYER3_SHADOW_POS);
	m_pPlayerUI[3]->SetPosition(PLAYER4_SHADOW_POS);
	//色.
	m_pPlayerUI[0]->SetCOLOR({ SHADOW_COLOR });
	m_pPlayerUI[1]->SetCOLOR({ SHADOW_COLOR });
	m_pPlayerUI[2]->SetCOLOR({ SHADOW_COLOR });
	m_pPlayerUI[3]->SetCOLOR({ SHADOW_COLOR });
	//1Pのコントローラーが接続されているなら.
	if (m_pMyController[0]->IsConnect()) {
		m_pPlayerUI[0]->SetAlpha(ALPHA_CONNECTED);	//透過解除.
	}
	//2Pのコントローラーが接続されているなら.
	if (m_pMyController[1]->IsConnect()) {
		m_pPlayerUI[1]->SetAlpha(ALPHA_CONNECTED);	//透過解除.
	}
	//3Pのコントローラーが接続されているなら.
	if (m_pMyController[2]->IsConnect()) {
		m_pPlayerUI[2]->SetAlpha(ALPHA_CONNECTED);	//透過解除.
	}
	//4Pのコントローラーが接続されているなら.
	if (m_pMyController[3]->IsConnect()) {
		m_pPlayerUI[3]->SetAlpha(ALPHA_CONNECTED);	//透過解除.
	}
	//コントローラー最大数分.
	for (int i = 0; i < PlayerMax; i++) {
		m_pPlayerUI[i]->SetisCOLOR(true);
		m_pPlayerUI[i]->Render();
		m_pPlayerUI[i]->SetisCOLOR(false);
	}
}
