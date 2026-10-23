#include "CWinnerResultUI.h"
#include "SpriteManager/CSpriteManager.h"
#include "System/XInput/CXInput.h"
#include "ResultDataManager/CResultDataManager.h"
#include <iostream>

struct Transform {
	float posX,posY;
	float sclX,sclY;

	//D3DXVECTOR3に変換する関数.
	D3DXVECTOR3 GetPos() const { return D3DXVECTOR3(posX, posY, 0.0f); }
	D3DXVECTOR3 GetScl() const { return D3DXVECTOR3(sclX, sclY, 0.0f); }
};

//文字を傾ける角度.
static constexpr float FONT_RAD = -0.1f;
//角度(5度傾ける).
static constexpr float RAD = -0.08726646;

//====================================================
//	単独1位の画面.
//====================================================
//あっぱれ文字
static constexpr Transform WINNER_FONT = { 145.f,90.f,300.f,90.f };
//色付きプレイヤーフォント.
static constexpr Transform PLAYER_FIRST_FONT = { 165.f,180.f,135.f,43.f };

//立ち絵.
static constexpr Transform SOLO_WIN[CWinnerResultUI::PLAYER_MAX] =
{
	{ 135.f, 90.f, 400.f, 630.f },	//プレイヤー壱.
	{  80.f, 90.f, 400.f, 630.f },	//プレイヤー弐.
	{ 100.f, 30.f, 460.f, 700.f },	//プレイヤー参.
	{ 135.f, 90.f, 400.f, 630.f },	//プレイヤー肆.
};

//緑の平行四辺形1.
static constexpr Transform GPARA1 = { 600.f,150.f,430.f,90.f };
//緑の平行四辺形2.
static constexpr Transform GPARA2 = { 580.f,280.f,420.f,60.f };
//緑の平行四辺形3.
static constexpr Transform GPARA3 = { 580.f,380.f,420.f,60.f };
//薄緑の平行四辺形.
static constexpr Transform UGPARA = { 545.f,480.f,450.f,90.f };
//角丸四角形.
static constexpr Transform ROUNDRECT = {1040.f,408.f,160.f,160.f};

//ポイントの文字.
static constexpr Transform FONT_GET			= { 656.f,162.f,303.f,56.f };	//かくとくポイント.
static constexpr Transform FONT_INGREDIENT	= { 610.f,290.f,217.f,40.f };	//具材ポイント.
static constexpr Transform FONT_SPECIAL		= { 605.f,390.f,217.f,40.f };	//高級具材ポイント.
static constexpr Transform FONT_TOTAL		= { 575.f,497.f,303.f,56.f };	//合計ポイント.

//ポイントの数字.
static constexpr Transform NUMBER_INGREDIENT	= { 955.f,290.f,24.f,40.f };	//具材ポイント.
static constexpr Transform NUMBER_SPECIAL		= { 955.f,390.f,24.f,40.f };	//高級具材ポイント.
static constexpr Transform NUMBER_TOTAL			= { 960.f,497.f,32.f,56.f };	//合計ポイント.

//お気に入り食材の文字.
static constexpr Transform FAVORITE_FONT = { 1050.f,420.f,140.f,27.f };
//お気に入り食材のポイント.
static constexpr Transform FAVORITE_NUMBER = { 1120.f,532.f,16.f,28.f };

//====================================================
//	同率1位の画面.
//====================================================
//あっぱれ文字.
static constexpr Transform TIE_WINNER_FONT = { 150.f,105.f,390.f,105.f };

//色付きプレイヤーフォント.
static constexpr Transform TIE_PLAYER_FONT[CWinnerResultUI::PLAYER_MAX] =
{
	{ 180.f,215.f,145.f,46.f },	//1人目.
	{ 330.f,200.f,145.f,46.f },	//2人目.
	{ 480.f,185.f,145.f,46.f },	//3人目.
	{ 630.f,170.f,145.f,46.f },	//4人目.
};

//立ち絵の大きさ.
static constexpr Transform TIE_PLAYER_SIZE[CWinnerResultUI::PLAYER_MAX] =
{
	{ 0.f,0.f,280.f,448.f },	//プレイヤー壱.
	{ 0.f,0.f,280.f,448.f },	//プレイヤー弐.
	{ 0.f,0.f,300.f,480.f },	//プレイヤー参.
	{ 0.f,0.f,280.f,448.f },	//プレイヤー肆.
};
//立ち絵の下端の位置.
static constexpr float TIE_PLAYER_BOTTOM = 735.f;

//立ち絵の中心のX座標.
static constexpr float TIE_CENTER_X_2[2] = { 330.f, 760.f };
static constexpr float TIE_CENTER_X_3[3] = { 200.f, 560.f, 920.f };
static constexpr float TIE_CENTER_X_4[4] = { 150.f, 455.f, 790.f, 1060.f };

//影のずらす量と濃さ.
static constexpr float SHADOW_OFFSET_X = 45.f;
static constexpr float SHADOW_OFFSET_Y = -12.f;
static constexpr float SHADOW_ALPHA = 0.35f;

//緑の平行四辺形.
static constexpr Transform TIE_GPARA = { 705.f,228.f,290.f,62.f };
//薄緑の平行四辺形.
static constexpr Transform TIE_UGPARA = { 782.f,273.f,445.f,88.f };
//かくとくポイントの文字.
static constexpr Transform TIE_FONT_GET = { 741.f,235.f,217.f,40.f };
//合計ポイントの文字.
static constexpr Transform TIE_FONT_TOTAL = { 820.f,289.f,282.f,52.f };
//合計ポイントの数字.
static constexpr Transform TIE_NUMBER_TOTAL = { 1170.f,289.f,32.f,55.f };

CWinnerResultUI::CWinnerResultUI()
	: m_Decided  (false)
	, m_pController(nullptr)
	, m_WinnerNo()
	, m_WinnerCount(0)
{
	BackGround_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround);	//背景.
	Winner_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerText);		//あっぱれ.
	Player_Img[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);		//プレイヤー壱
	Player_Img[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);		//プレイヤー弐
	Player_Img[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);		//プレイヤー参
	Player_Img[3] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);		//プレイヤー肆
	for (int i = 0; i < FONT_MAM; i++)
	{
		PlayerFont_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerFont);	//色付きプレイヤーフォント
	}
	for (int i = 0; i < PARA_MAM; i++)
	{
		Para_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_Para);	//平行四辺形.
	}
	RoundRect_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_RoundRect);	//角丸四角形.
	ResultFont_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_ResultFont);	//ポイントの文字.
	Number_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Digit0_9);				//数字.
	FavoriteFont_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_FavoriteFont);	//お気に入り食材の文字.

	//ゲームの結果から1位のプレイヤーを順番に集める.
	const CResultDataManager* pData = CResultDataManager::GetInstance();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		const SPlayerResultData& Data = pData->GetPlayerData(i);
		if (Data.m_bActive && Data.m_nRank == 1)
		{
			m_WinnerNo[m_WinnerCount] = i;
			m_WinnerCount++;
		}
	}
	//参加者がいない場合はプレイヤー壱を表示する.
	if (m_WinnerCount == 0)
	{
		m_WinnerNo[0] = 0;
		m_WinnerCount = 1;
	}
}

CWinnerResultUI::~CWinnerResultUI()
{
	BackGround_Img = nullptr;
	Winner_Img = nullptr;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Player_Img[i] = nullptr;
	}
	for (int i = 0; i < FONT_MAM; i++)
	{
		PlayerFont_Img[i] = nullptr;
	}
	for (int i = 0; i < PARA_MAM; i++)
	{
		Para_Img[i] = nullptr;
	}

	RoundRect_Img = nullptr;
	ResultFont_Img = nullptr;
	Number_Img = nullptr;
	FavoriteFont_Img = nullptr;
}

void CWinnerResultUI::Update()
{
	//Aボタンで決定.
	if (m_pController->IsDown(CXInput::A, true))
	{
		m_Decided = true;
	}
}

void CWinnerResultUI::Draw()
{
	WinnerResultBackUI();

	//単独1位の画面.
	if (m_WinnerCount == 1)
	{
		SoloWinnerUI();
	}
	//同率1位の画面.
	else
	{
		TieWinnerUI();
	}
}

void CWinnerResultUI::Init()
{
	m_Decided = false;
}

//背景.
void CWinnerResultUI::WinnerResultBackUI()
{
	BackGround_Img->SetPosition(D3DXVECTOR3(0, 0, 0));
	BackGround_Img->SetScale(D3DXVECTOR3(WND_W, WND_H, 0));
	BackGround_Img->Render();
}

//====================================================
//	単独1位の画面.
//====================================================
void CWinnerResultUI::SoloWinnerUI()
{
	SoloWinnerTextUI();
	SoloPlayerUI();
	SoloPlayerFontUI();
	SoloShape();
	SoloPointUI();
	SoloFavoriteUI();
}

//あっぱれ.
void CWinnerResultUI::SoloWinnerTextUI()
{
	Winner_Img->SetPosition(WINNER_FONT.GetPos());
	Winner_Img->SetScale(WINNER_FONT.GetScl());
	Winner_Img->SetRotation(D3DXVECTOR3(0, 0, FONT_RAD));
	Winner_Img->Render();
}

//1位のプレイヤーの立ち絵.
void CWinnerResultUI::SoloPlayerUI()
{
	int No = m_WinnerNo[0];
	Player_Img[No]->SetPosition(SOLO_WIN[No].GetPos());
	Player_Img[No]->SetScale(SOLO_WIN[No].GetScl());
	Player_Img[No]->SetRotation(D3DXVECTOR3(0, 0, 0));
	Player_Img[No]->SetisCOLOR(false);
	Player_Img[No]->SetAlpha(1.f);
	Player_Img[No]->SetPatternNo(1, 0);
	Player_Img[No]->Render();
}

//1位のプレイヤーの文字.
void CWinnerResultUI::SoloPlayerFontUI()
{
	int No = m_WinnerNo[0];
	PlayerFont_Img[No]->SetPosition(PLAYER_FIRST_FONT.GetPos());
	PlayerFont_Img[No]->SetScale(PLAYER_FIRST_FONT.GetScl());
	PlayerFont_Img[No]->SetRotation(D3DXVECTOR3(0, 0, FONT_RAD));
	PlayerFont_Img[No]->SetPatternNo(0, No);
	PlayerFont_Img[No]->Render();
}

//ポイントの枠.
void CWinnerResultUI::SoloShape()
{
	//緑の平行四辺形1.
	Para_Img[0]->SetPosition(GPARA1.GetPos());
	Para_Img[0]->SetScale(GPARA1.GetScl());
	Para_Img[0]->SetRotation(D3DXVECTOR3(0, 0, RAD));
	Para_Img[0]->SetPatternNo(0, 0);
	Para_Img[0]->Render();
	//緑の平行四辺形2.
	Para_Img[1]->SetPosition(GPARA2.GetPos());
	Para_Img[1]->SetScale(GPARA2.GetScl());
	Para_Img[1]->SetRotation(D3DXVECTOR3(0, 0, 0));
	Para_Img[1]->SetPatternNo(0, 0);
	Para_Img[1]->Render();
	//緑の平行四辺形3.
	Para_Img[2]->SetPosition(GPARA3.GetPos());
	Para_Img[2]->SetScale(GPARA3.GetScl());
	Para_Img[2]->SetRotation(D3DXVECTOR3(0, 0, 0));
	Para_Img[2]->SetPatternNo(0, 0);
	Para_Img[2]->Render();
	//薄緑の平行四辺形.
	Para_Img[3]->SetPosition(UGPARA.GetPos());
	Para_Img[3]->SetScale(UGPARA.GetScl());
	Para_Img[3]->SetRotation(D3DXVECTOR3(0, 0, 0));
	Para_Img[3]->SetPatternNo(0, 1);
	Para_Img[3]->Render();
}

//ポイントの文字と数字.
void CWinnerResultUI::SoloPointUI()
{
	const SPlayerResultData& Data = CResultDataManager::GetInstance()->GetPlayerData(m_WinnerNo[0]);

	//かくとくポイント.
	ResultFont_Img->SetPosition(FONT_GET.GetPos());
	ResultFont_Img->SetScale(FONT_GET.GetScl());
	ResultFont_Img->SetRotation(D3DXVECTOR3(0, 0, RAD));
	ResultFont_Img->SetPatternNo(0, 0);
	ResultFont_Img->Render();
	ResultFont_Img->SetRotation(D3DXVECTOR3(0, 0, 0));

	//具材ポイント.
	ResultFont_Img->SetPosition(FONT_INGREDIENT.GetPos());
	ResultFont_Img->SetScale(FONT_INGREDIENT.GetScl());
	ResultFont_Img->SetPatternNo(0, 1);
	ResultFont_Img->Render();
	DrawNumber(Data.m_nIngredientScore,
		NUMBER_INGREDIENT.posX - GetNumberWidth(Data.m_nIngredientScore, NUMBER_INGREDIENT.sclX),
		NUMBER_INGREDIENT.posY, NUMBER_INGREDIENT.sclX, NUMBER_INGREDIENT.sclY);

	//高級具材ポイント.
	ResultFont_Img->SetPosition(FONT_SPECIAL.GetPos());
	ResultFont_Img->SetScale(FONT_SPECIAL.GetScl());
	ResultFont_Img->SetPatternNo(0, 2);
	ResultFont_Img->Render();
	DrawNumber(Data.m_nSpecialScore,
		NUMBER_SPECIAL.posX - GetNumberWidth(Data.m_nSpecialScore, NUMBER_SPECIAL.sclX),
		NUMBER_SPECIAL.posY, NUMBER_SPECIAL.sclX, NUMBER_SPECIAL.sclY);

	//合計ポイント.
	ResultFont_Img->SetPosition(FONT_TOTAL.GetPos());
	ResultFont_Img->SetScale(FONT_TOTAL.GetScl());
	ResultFont_Img->SetPatternNo(0, 3);
	ResultFont_Img->Render();
	DrawNumber(Data.m_nScore,
		NUMBER_TOTAL.posX - GetNumberWidth(Data.m_nScore, NUMBER_TOTAL.sclX),
		NUMBER_TOTAL.posY, NUMBER_TOTAL.sclX, NUMBER_TOTAL.sclY);
}

//お気に入り食材の枠と文字と数字.
void CWinnerResultUI::SoloFavoriteUI()
{
	const SPlayerResultData& Data = CResultDataManager::GetInstance()->GetPlayerData(m_WinnerNo[0]);

	//角丸四角形.
	RoundRect_Img->SetPosition(ROUNDRECT.GetPos());
	RoundRect_Img->SetScale(ROUNDRECT.GetScl());
	RoundRect_Img->Render();

	//お気に入り食材の文字.
	FavoriteFont_Img->SetPosition(FAVORITE_FONT.GetPos());
	FavoriteFont_Img->SetScale(FAVORITE_FONT.GetScl());
	FavoriteFont_Img->Render();

	//お気に入り食材で獲得したポイント.
	DrawNumber(Data.m_nFavoriteScore,
		FAVORITE_NUMBER.posX - GetNumberWidth(Data.m_nFavoriteScore, FAVORITE_NUMBER.sclX) / 2.f,
		FAVORITE_NUMBER.posY, FAVORITE_NUMBER.sclX, FAVORITE_NUMBER.sclY);
}

//====================================================
//	同率1位の画面.
//====================================================
void CWinnerResultUI::TieWinnerUI()
{
	TieWinnerTextUI();
	TiePlayerUI();
	TiePlayerFontUI();
	TiePointUI();
}

//あっぱれ.
void CWinnerResultUI::TieWinnerTextUI()
{
	Winner_Img->SetPosition(TIE_WINNER_FONT.GetPos());
	Winner_Img->SetScale(TIE_WINNER_FONT.GetScl());
	Winner_Img->SetRotation(D3DXVECTOR3(0, 0, FONT_RAD));
	Winner_Img->Render();
}

//1位のプレイヤーたちの立ち絵.
void CWinnerResultUI::TiePlayerUI()
{
	for (int i = 0; i < m_WinnerCount; i++)
	{
		int No = m_WinnerNo[i];

		//人数に合わせて立ち絵の中心を決める.
		float CenterX = GetTieCenterX(i);

		float W = TIE_PLAYER_SIZE[No].sclX;
		float H = TIE_PLAYER_SIZE[No].sclY;
		float X = CenterX - W / 2.f;
		float Y = TIE_PLAYER_BOTTOM - H;

		Player_Img[No]->SetScale(D3DXVECTOR3(W, H, 0.f));
		Player_Img[No]->SetRotation(D3DXVECTOR3(0, 0, 0));
		Player_Img[No]->SetPatternNo(1, 0);	

		//影.
		Player_Img[No]->SetPosition(D3DXVECTOR3(X + SHADOW_OFFSET_X, Y + SHADOW_OFFSET_Y, 0.f));
		Player_Img[No]->SetisCOLOR(true);
		Player_Img[No]->SetCOLOR(D3DXVECTOR3(0.f, 0.f, 0.f));
		Player_Img[No]->SetAlpha(SHADOW_ALPHA);
		Player_Img[No]->Render();

		//立ち絵.
		Player_Img[No]->SetPosition(D3DXVECTOR3(X, Y, 0.f));
		Player_Img[No]->SetisCOLOR(false);
		Player_Img[No]->SetAlpha(1.f);
		Player_Img[No]->Render();
	}
}

//1位のプレイヤーたちの文字.
void CWinnerResultUI::TiePlayerFontUI()
{
	for (int i = 0; i < m_WinnerCount; i++)
	{
		int No = m_WinnerNo[i];
		PlayerFont_Img[No]->SetPosition(TIE_PLAYER_FONT[i].GetPos());
		PlayerFont_Img[No]->SetScale(TIE_PLAYER_FONT[i].GetScl());
		PlayerFont_Img[No]->SetRotation(D3DXVECTOR3(0, 0, FONT_RAD));
		PlayerFont_Img[No]->SetPatternNo(0, No);
		PlayerFont_Img[No]->Render();
	}
}

//かくとくポイントと合計ポイント.
void CWinnerResultUI::TiePointUI()
{
	//同率なので1人目の合計ポイントを表示する.
	const SPlayerResultData& Data = CResultDataManager::GetInstance()->GetPlayerData(m_WinnerNo[0]);

	//緑の平行四辺形.
	Para_Img[0]->SetPosition(TIE_GPARA.GetPos());
	Para_Img[0]->SetScale(TIE_GPARA.GetScl());
	Para_Img[0]->SetRotation(D3DXVECTOR3(0, 0, RAD));
	Para_Img[0]->SetPatternNo(0, 0);
	Para_Img[0]->Render();
	//薄緑の平行四辺形.
	Para_Img[1]->SetPosition(TIE_UGPARA.GetPos());
	Para_Img[1]->SetScale(TIE_UGPARA.GetScl());
	Para_Img[1]->SetRotation(D3DXVECTOR3(0, 0, 0));
	Para_Img[1]->SetPatternNo(0, 1);
	Para_Img[1]->Render();

	//かくとくポイント.
	ResultFont_Img->SetPosition(TIE_FONT_GET.GetPos());
	ResultFont_Img->SetScale(TIE_FONT_GET.GetScl());
	ResultFont_Img->SetRotation(D3DXVECTOR3(0, 0, RAD));
	ResultFont_Img->SetPatternNo(0, 0);
	ResultFont_Img->Render();
	ResultFont_Img->SetRotation(D3DXVECTOR3(0, 0, 0));

	//合計ポイント.
	ResultFont_Img->SetPosition(TIE_FONT_TOTAL.GetPos());
	ResultFont_Img->SetScale(TIE_FONT_TOTAL.GetScl());
	ResultFont_Img->SetPatternNo(0, 3);
	ResultFont_Img->Render();
	DrawNumber(Data.m_nScore,
		TIE_NUMBER_TOTAL.posX - GetNumberWidth(Data.m_nScore, TIE_NUMBER_TOTAL.sclX),
		TIE_NUMBER_TOTAL.posY, TIE_NUMBER_TOTAL.sclX, TIE_NUMBER_TOTAL.sclY);
}

//i人目の立ち絵の中心のX座標.
float CWinnerResultUI::GetTieCenterX(int i) const
{
	if (m_WinnerCount == 2)		return TIE_CENTER_X_2[i];
	if (m_WinnerCount == 3)		return TIE_CENTER_X_3[i];
	return TIE_CENTER_X_4[i];
}

//====================================================
//	数字.
//====================================================
//数字と点を並べたときの幅.
float CWinnerResultUI::GetNumberWidth(int Score, float W)
{
	//桁数を数える.
	int Digit = 1;
	for (int S = Score; S >= 10; S /= 10)
	{
		Digit++;
	}
	//数字の桁数＋点の文字.
	return W * (Digit + 1);
}

//数字と点の表示.
void CWinnerResultUI::DrawNumber(int Score, float PosX, float PosY, float W, float H)
{
	if (Score < 0) Score = 0;

	//一番上の位を求める.
	int M = 1;
	while (Score / M >= 10)
	{
		M *= 10;
	}

	Number_Img->SetRotation(D3DXVECTOR3(0, 0, 0));
	Number_Img->SetScale(D3DXVECTOR3(W, H, 0.f));

	//上の位から1文字ずつ.
	int S = Score;
	float X = PosX;
	while (M > 0)
	{
		Number_Img->SetPosition(D3DXVECTOR3(X, PosY, 0.f));
		Number_Img->SetPatternNo(static_cast<SHORT>(S / M), 0);
		Number_Img->Render();

		S -= (S / M) * M;
		M /= 10;
		X += W;
	}

	//点.
	Number_Img->SetPosition(D3DXVECTOR3(X, PosY, 0.f));
	Number_Img->SetPatternNo(10, 0);
	Number_Img->Render();
}
