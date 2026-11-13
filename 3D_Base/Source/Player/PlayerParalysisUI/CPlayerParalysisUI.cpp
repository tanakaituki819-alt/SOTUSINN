#include "CPlayerParalysisUI.h"

namespace {
	//---- 表示 ----
	const D3DXVECTOR3	UI_OFFSET	= { 50.f, -60.f, 0.f };		//プレイヤーの位置からのずらし量.
	const D3DXVECTOR3	UI_SCALE	= { 100.f, 130.f, 1.f };	//UIの大きさ.
	const D3DXVECTOR3	ROT_NONE	= { 0.f, 0.f, 0.f };		//回転なし.
	constexpr float	ALPHA_OPAQUE	= 1.0f;						//不透明.

	//---- アニメーション ----
	constexpr SHORT		PATTERN_X		 = 0;	//画像の横のパターン番号(常に0列目).
	constexpr SHORT		PATTERN_Y_FIRST  = 0;	//1枚目の絵(画像の0段目).
	constexpr SHORT		PATTERN_Y_SECOND = 2;	//2枚目の絵(画像の2段目).
	constexpr int		SWITCH_FRAME	 = 5;	//絵を切り替える間隔(フレーム).
}

CPlayerParalysisUI::CPlayerParalysisUI()
{
	m_pSprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_ButtonPush);
}

CPlayerParalysisUI::~CPlayerParalysisUI() = default;

void CPlayerParalysisUI::Update()
{
	m_AnimFrame = (m_AnimFrame + 1) % (SWITCH_FRAME * 2);
}

void CPlayerParalysisUI::Draw()
{
	//前半は1枚目、後半は2枚目の絵を使う.
	const SHORT patternY = (m_AnimFrame < SWITCH_FRAME) ? PATTERN_Y_FIRST : PATTERN_Y_SECOND;

	//同じ画像を他のプレイヤーのUIでも使うので、描画のたびに全部の設定をし直す.
	m_pSprite->SetPosition(m_Position);
	m_pSprite->SetScale(UI_SCALE);
	m_pSprite->SetRotation(ROT_NONE);
	m_pSprite->SetPatternNo(PATTERN_X, patternY);
	m_pSprite->SetAlpha(ALPHA_OPAQUE);
	m_pSprite->Render();
}

//表示の基準になるプレイヤーの位置を設定する.
void CPlayerParalysisUI::SetPlayerPosition(const D3DXVECTOR3& playerPos)
{
	m_Position = playerPos + UI_OFFSET;
}