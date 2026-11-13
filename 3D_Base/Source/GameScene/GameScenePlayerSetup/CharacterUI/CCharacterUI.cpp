#include "CCharacterUI.h"
#include "SpriteManager/CSpriteManager.h"

namespace {
	//プレイヤー1人ぶんの表示設定(データ).
	struct PlayerUIDef
	{
		CSpriteManager::enImagList	image;		//キャラ画像.
		D3DXVECTOR3					pos;		//本体の位置.
		D3DXVECTOR3					shadowPos;	//影の位置.
		D3DXVECTOR3					scale;		//大きさ(本体と影で共通).
	};
	//キャラの位置や大きさ.
	const PlayerUIDef PLAYER_UI_DEFS[] = {
		//画像									本体の位置					影の位置						大きさ
		{ CSpriteManager::enImagList::Img_Player1,{	14.f,	178.f, 1.f },{ 52.f,   178.f, 1.f },{ 260.f, 450.f, 0.f } },	//1P:女将さん.
		{ CSpriteManager::enImagList::Img_Player2,{ 300.8f, 178.f, 1.f },{ 330.f,  178.f, 1.f },{ 260.f, 450.f, 0.f } },	//2P:男.
		{ CSpriteManager::enImagList::Img_Player3,{ 652.8f, 148.f, 1.f },{ 682.f,  150.f, 1.f },{ 280.f, 480.f, 0.f } },	//3P:女子高生.
		{ CSpriteManager::enImagList::Img_Player4,{ 972.8f, 160.f, 1.f },{ 1008.f, 158.f, 1.f },{ 270.f, 470.f, 0.f } },	//4P:おばちゃん.
	};
	//見た目
	constexpr float		ALPHA_CONNECTED		= 1.0f;					//接続時アルファ.
	constexpr float		ALPHA_DISCONNECTED	= 0.5f;					//未接続時アルファ(本体を半透明にする).
	const D3DXVECTOR3	SHADOW_COLOR		= { 0.f, 0.f, 0.f };	//影の色(黒).
}

CCharacterUI::CCharacterUI()
{
	for (int i = 0; i < PlayerMax; i++) {
		m_pPlayerUI[i] = CSpriteManager::GetSprite2D(PLAYER_UI_DEFS[i].image);
	}
}

CCharacterUI::~CCharacterUI() = default;	//画像もコントローラーも借りているだけなので、ここでは解放しない.

void CCharacterUI::Update()
{
}

void CCharacterUI::Draw()
{
	//全員の影を先に描き、その上に本体を重ねる(隣のキャラの影が本体より手前に出ないようにするため).
	for (int i = 0; i < PlayerMax; i++) {
		DrawShadow(i);
	}
	for (int i = 0; i < PlayerMax; i++) {
		DrawBody(i);
	}
}

//影.
void CCharacterUI::DrawShadow(int index)
{
	//未接続のプレイヤーは影を出さない.
	if (!IsConnected(index)) return;

	CSprite2D* sprite		= m_pPlayerUI[index];
	const PlayerUIDef& def	= PLAYER_UI_DEFS[index];

	//同じ画像を本体と影で使い回すので、描画のたびに設定し直す.
	sprite->SetPosition(def.shadowPos);
	sprite->SetScale(def.scale);
	sprite->SetAlpha(ALPHA_CONNECTED);
	sprite->SetCOLOR(SHADOW_COLOR);
	sprite->SetisCOLOR(true);	//画像を影の色で塗りつぶして描画する.
	sprite->Render();
	sprite->SetisCOLOR(false);	//本体の描画に影響しないよう戻す.
}

//本体.
void CCharacterUI::DrawBody(int index)
{
	CSprite2D* sprite = m_pPlayerUI[index];
	const PlayerUIDef& def = PLAYER_UI_DEFS[index];

	sprite->SetPosition(def.pos);
	sprite->SetScale(def.scale);
	sprite->SetAlpha(IsConnected(index) ? ALPHA_CONNECTED : ALPHA_DISCONNECTED);	//未接続なら半透明.
	sprite->Render();
}

bool CCharacterUI::IsConnected(int index) const
{
	//SetXInput が呼ばれていない番号でも落ちないよう、nullptr を確認する.
	return m_pMyController[index] != nullptr && m_pMyController[index]->IsConnect();
}