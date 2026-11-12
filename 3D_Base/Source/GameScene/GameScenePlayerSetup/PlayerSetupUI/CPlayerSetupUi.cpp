#include "CPlayerSetupUI.h"
#include "SpriteManager/CSpriteManager.h"

namespace {
	//画像1枚ぶんの表示設定.
	struct SpriteLayout
	{
		D3DXVECTOR3	pos;		//位置.
		D3DXVECTOR3	scale;		//大きさ.
		D3DXVECTOR3	rot;		//回転.
		SHORT		patternX;	//画像の何列目を使うか.
		SHORT		patternY;	//画像の何段目を使うか.
	};

	const D3DXVECTOR3 ROT_NONE = { 0.f, 0.f, 0.f };	//回転なし.

	//プレイヤーごとの位置(X座標)
	struct PlayerSlot
	{
		float panelX;			//背景と名前の位置.
		float controllerIconX;	//コントローラーアイコンの位置.
	};
	const PlayerSlot PLAYER_SLOTS[] = {
		//背景・名前	コントローラー
		{ 8.f,		154.f },	//1P.
		{ 332.8f,	480.f },	//2P.
		{ 652.8f,	800.f },	//3P.
		{ 972.8f,	1120.f },	//4P.
	};
	//X座標とパターン番号はプレイヤーごとに決めるので、ここでは0にしておく.
	const SpriteLayout BACKGROUND_LAYOUT		= { { 0.f, 0.f,   0.f },{ 300.f, 630.f, 0.f }, ROT_NONE, 0, 0 };	//画像の i 列目が iP 用.
	const SpriteLayout NAME_LAYOUT				= { { 0.f, 0.f,   0.f },{ 300.f, 75.f,  0.f }, ROT_NONE, 0, 0 };	//画像の i 段目が iP 用.
	const SpriteLayout CONTROLLER_ICON_LAYOUT	= { { 0.f, 100.f, 0.f },{ 150.f, 75.f,  0.f }, ROT_NONE, 0, 0 };

	//画面下のボタン案内(巻物の上にテキストを重ねる).
	const SpriteLayout GUIDE_SCROLL_LAYOUTS[] = {
		{ { 788.f,  683.f, 0.f },{ 210.f, 100.f, 0.f }, ROT_NONE, 0, 0 },	//[0]タイトルに戻る.
		{ { 1082.f, 683.f, 0.f },{ 380.f, 100.f, 0.f }, ROT_NONE, 0, 0 },	//[1]準備完了キャンセル.
	};
	const SpriteLayout GUIDE_TEXT_LAYOUTS[] = {
		{ { 700.f,  638.f, 0.f },{ 300.f, 100.f, 0.f }, ROT_NONE, 0, 2 },	//[0]タイトルに戻る.
		{ { 928.f,  638.f, 0.f },{ 300.f, 100.f, 0.f }, ROT_NONE, 0, 1 },	//[1]準備完了キャンセル.
	};

	//透明度.
	constexpr float ALPHA_OPAQUE		= 1.0f;		//不透明.
	constexpr float ALPHA_DISCONNECTED	= 0.2f;		//未接続のコントローラーアイコン.

	//同じ画像を他のUIでも使うので、描画のたびに全部の設定をし直す.
	void DrawSprite(CSprite2D* sprite, const SpriteLayout& layout, float alpha)
	{
		sprite->SetPosition(layout.pos);
		sprite->SetScale(layout.scale);
		sprite->SetRotation(layout.rot);
		sprite->SetPatternNo(layout.patternX, layout.patternY);
		sprite->SetAlpha(alpha);
		sprite->Render();
	}
}

CPlayerSetupUI::CPlayerSetupUI()
{
	m_pBackground		= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	m_pName				= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Playerfont);
	m_pControllerIcon	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_controller);
	m_pText				= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);
	m_pScroll			= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);
}

CPlayerSetupUI::~CPlayerSetupUI() = default;	//画像もコントローラーも借りているだけなので、ここでは解放しない.

void CPlayerSetupUI::Update()
{
}
void CPlayerSetupUI::Draw()
{
	DrawBackgrounds();
	DrawNames();
	DrawControllerIcons();
	DrawGuides();
}
//各プレイヤーの背景.
void CPlayerSetupUI::DrawBackgrounds()
{
	for (int i = 0; i < Ui_Max; i++) {
		SpriteLayout layout = BACKGROUND_LAYOUT;	//共通の設定をコピーして、プレイヤーごとの部分だけ変える.
		layout.pos.x = PLAYER_SLOTS[i].panelX;
		layout.patternX = static_cast<SHORT>(i);	//i列目の背景を使う.
		DrawSprite(m_pBackground, layout, ALPHA_OPAQUE);
	}
}
//各プレイヤーの名前(背景の上端に重ねる).
void CPlayerSetupUI::DrawNames()
{
	for (int i = 0; i < Ui_Max; i++) {
		SpriteLayout layout = NAME_LAYOUT;
		layout.pos.x = PLAYER_SLOTS[i].panelX;
		layout.patternY = static_cast<SHORT>(i);	//i段目の名前を使う.
		DrawSprite(m_pName, layout, ALPHA_OPAQUE);
	}
}
//各プレイヤーのコントローラーアイコン(未接続なら薄く表示).
void CPlayerSetupUI::DrawControllerIcons()
{
	for (int i = 0; i < Ui_Max; i++) {
		SpriteLayout layout = CONTROLLER_ICON_LAYOUT;
		layout.pos.x = PLAYER_SLOTS[i].controllerIconX;
		DrawSprite(m_pControllerIcon, layout, IsConnected(i) ? ALPHA_OPAQUE : ALPHA_DISCONNECTED);
	}
}
//画面下のボタン案内.
void CPlayerSetupUI::DrawGuides()
{
	//巻物を全部描いてから、その上にテキストを重ねる.
	for (const SpriteLayout& scroll : GUIDE_SCROLL_LAYOUTS) {
		DrawSprite(m_pScroll, scroll, ALPHA_OPAQUE);
	}
	for (const SpriteLayout& text : GUIDE_TEXT_LAYOUTS) {
		DrawSprite(m_pText, text, ALPHA_OPAQUE);
	}
}
bool CPlayerSetupUI::IsConnected(int player) const
{
	//SetXInput が呼ばれていない番号でも落ちないよう、nullptr を確認する.
	return m_pMyController[player] != nullptr && m_pMyController[player]->IsConnect();
}