#include "CWinnerResultUI.h"
#include "SpriteManager/CSpriteManager.h"
#include "System/XInput/CXInput.h"
#include <iostream>

struct Transform {
	float posX,posY;
	float sclX,sclY;

	//D3DXVECTOR3に変換する関数.
	D3DXVECTOR3 GetPos() const { return D3DXVECTOR3(posX, posY, 0.0f); }
	D3DXVECTOR3 GetScl() const { return D3DXVECTOR3(sclX, sclY, 0.0f); }
};

//あっぱれ文字
static constexpr Transform WINNER_FONT = { 145.f,90.f,300.f,90.f };

//色付きプレイヤーフォント１人目.
static constexpr Transform PLAYER_FIRST_FONT = { 165.f,180.f,135.f,43.f };
//色付きプレイヤーフォント２人目.
static constexpr Transform PLAYER_SECOND_FONT = { 304.f,166.f,135.f,43.f };
//色付きプレイヤーフォント3人目.
static constexpr Transform PLAYER_THIRD_FONT = { 443.f,152.f,135.f,43.f };
//色付きプレイヤーフォント4人目.
static constexpr Transform PLAYER_FOURTH_FONT = { 582.f,138.f,135.f,43.f };

//-単独1位-.
//プレイヤー壱
static constexpr Transform P1_SOLO_WIN = { 135.f, 90.f, 400.f, 630.f };
//プレイヤー弐
static constexpr Transform P2_SOLO_WIN = { 80.f, 90.f, 400.f, 630.f };
//プレイヤー参
static constexpr Transform P3_SOLO_WIN = { 100.f, 30.f, 460.f, 700.f };
//プレイヤー肆
static constexpr Transform P4_SOLO_WIN = { 135.f, 90.f, 400.f, 630.f };
//-2人同率-.
//プレイヤー壱と誰かが同率時のプレイヤー壱の立ち絵.
static constexpr Transform DUO_P1_ALL_P1 = { 0.f,0.f,0.f,0.f };
//プレイヤー弐と誰かが同率時のプレイヤー弐の立ち絵.
static constexpr Transform DUO_P2_ALL_P2 = { 0.f,0.f,0.f,0.f };
//誰かとプレイヤー参が同率時のプレイイヤー参の立ち絵
static constexpr Transform DUO_ALL_P3_P3 = { 0.f,0.f,0.f,0.f };
//誰かとプレイヤー肆が同率時のプレイイヤー肆の立ち絵
static constexpr Transform DUO_ALL_P4_P4 = { 0.f,0.f,0.f,0.f };
//プレイヤー壱とプレイヤー弐が同率時のプレイヤー弐の立ち絵.
static constexpr Transform DUO_P1_P2_P2 = {0.f,0.f,0.f,0.f};
//プレイヤー弐とプレイヤー参が同率時のプレイヤー参の立ち絵.
static constexpr Transform DUO_P2_P3_P2 = { 0.f,0.f,0.f,0.f };
//プレイヤー参とプレイヤー肆が同率時のプレイヤー参の立ち絵.
static constexpr Transform DUO_P3_P4_P3 = { 0.f,0.f,0.f,0.f };
//-3人同率-.
//プレイヤー壱と誰かと誰かが同率時のプレイヤー壱の立ち絵
static constexpr Transform TRIO_P1_ALL_ALL_P1 = { 0.f,0.f,0.f,0.f };
//誰かとプレイヤー弐と誰かが同率時のプレイヤー弐の立ち絵.
static constexpr Transform TRIO_ALL_P2_ALL_P2 = { 0.f,0.f,0.f,0.f };
//誰かと誰かとプレイヤー参が同率時のプレイヤー肆の立ち絵.
static constexpr Transform TRIO_ALL_ALL_P4_P4 = { 0.f,0.f,0.f,0.f };
//プレイヤー壱とプレイヤー弐とプレイヤー参が同率時のプレイヤー参の立ち絵
static constexpr Transform TRIO_P1_P2_P3_P3 = { 0.f,0.f,0.f,0.f };
//プレイヤー壱とプレイヤー参とプレイヤー肆が同率時のプレイヤー参の立ち絵
static constexpr Transform TRIO_P1_P3_P4_P3 = { 0.f,0.f,0.f,0.f };
//プレイヤー壱とプレイヤー参とプレイヤー肆が同率時のプレイヤー肆の立ち絵
static constexpr Transform TRIO_P1_P3_P4_P4 = { 0.f,0.f,0.f,0.f };
//レイヤー弐とプレイヤー参とプレイヤー肆が同率時のプレイヤー弐の立ち絵
static constexpr Transform TRIO_P2_P3_P4_P2 = { 0.f,0.f,0.f,0.f };
//レイヤー弐とプレイヤー参とプレイヤー肆が同率時のプレイヤー参の立ち絵
static constexpr Transform TRIO_P2_P3_P4_P3 = { 0.f,0.f,0.f,0.f };
//-4人同率-.
//プレイヤー全員が同率の時のプレイヤー壱の立ち絵
static constexpr Transform ALL_P1 = { 0.f,0.f,300.f,500.f };
//プレイヤー全員が同率の時のプレイヤー弐の立ち絵
static constexpr Transform ALL_P2 = { 100.f,0.f,300.f,500.f };
//プレイヤー全員が同率の時のプレイヤー参の立ち絵
static constexpr Transform ALL_P3 = { 200.f,0.f,300.f,500.f };
//プレイヤー全員が同率の時のプレイヤー肆の立ち絵
static constexpr Transform ALL_P4 = { 300.f,0.f,300.f,500.f };

//角度(5度傾ける).
static constexpr float RAD = -0.08726646;

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

CWinnerResultUI::CWinnerResultUI()
	:Win (0)
	, m_Decided  (false)
	, m_pController(nullptr)
{
	BackGround_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround);	//背景.
	Winner_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerText);		//あっぱれ.
	Player1_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);			//プレイヤー壱
	Player2_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);			//プレイヤー弐
	Player3_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);			//プレイヤー参
	Player4_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);			//プレイヤー肆
	for (int i = 0; i < FONT_MAM; i++)
	{
		PlayerFont_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerFont);	//色付きプレイヤーフォント
	}
	for (int i = 0; i < PARA_MAM; i++)
	{
		Para_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_Para);	//平行四辺形.
	}
	RoundRect_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_RoundRect);	//角丸四角形.
}

CWinnerResultUI::~CWinnerResultUI()
{
	BackGround_Img = nullptr;
	Winner_Img = nullptr;
	Player1_Img = nullptr;
	Player2_Img = nullptr;
	Player3_Img = nullptr;
	Player4_Img = nullptr;
	for (int i = 0; i < FONT_MAM; i++)
	{
		PlayerFont_Img[i] = nullptr;
	}
	for (int i = 0; i < PARA_MAM; i++)
	{
		Para_Img[i] = nullptr;
	}
	RoundRect_Img = nullptr;
}
	void CWinnerResultUI::Update()
{
#if 0
	//実行中に動かすやつ
		static float ananana = 10;
		static float ananana2 = 10;

		if (GetAsyncKeyState('W') & 0x8000)
		{
			ananana--;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			ananana++;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			ananana2++;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			ananana2--;
		}

		FILE* pFile;
		//stdout（標準出力）を新しく作成したコンソールにリダイレクト
		freopen_s(&pFile, "CONOUT$", "w", stdout);
		std::cout << ananana << std::endl;
	
#endif
		//if (m_pController == nullptr)
		//{
		//	return;
		//}
		//if (m_pController->IsConnect() == false)
		//{
		//	return;
		//}


		//Aボタンで決定.
		if (m_pController->IsDown(CXInput::A, true))
		{
			m_Decided = true;
		}

#if 1
		//仮で１５パターンん
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		Win = 0;
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		Win = 1;
	}
	if (GetAsyncKeyState('R') & 0x8000)
	{
		Win = 2;
	}
	if (GetAsyncKeyState('T') & 0x8000)
	{
		Win = 3;
	}
	if (GetAsyncKeyState('Y') & 0x8000)
	{
		Win = 4;
	}
	if (GetAsyncKeyState('U') & 0x8000)
	{
		Win = 5;
	}
	if (GetAsyncKeyState('I') & 0x8000)
	{
		Win = 6;
	}
	if (GetAsyncKeyState('O') & 0x8000)
	{
		Win = 7;
	}
	if (GetAsyncKeyState('P') & 0x8000)
	{
		Win = 8;
	}
	if (GetAsyncKeyState('F') & 0x8000)
	{
		Win = 9;
	}
	if (GetAsyncKeyState('G') & 0x8000)
	{
		Win = 10;
	}
	if (GetAsyncKeyState('H') & 0x8000)
	{
		Win = 11;
	}
	if (GetAsyncKeyState('J') & 0x8000)
	{
		Win = 12;
	}
	if (GetAsyncKeyState('K') & 0x8000)
	{
		Win = 13;
	}
	if (GetAsyncKeyState('L') & 0x8000)
	{
		Win = 14;
	}
#endif

}

void CWinnerResultUI::Draw()
{
	WinnerResultBackUI();
	WinnerTextUI();

#if 1
	//仮 2026.07.23
	if (Win == 0)
	{
		Player1WinnerUI();
		Player1FontUI();
		Shape();
	}
	else if (Win == 1)
	{
		Player2WinnerUI();
		Player2FontUI();
		Shape();
	}
	else if (Win == 2)
	{
		Player3WinnerUI();
		Player3FontUI();
		Shape();
	}
	else if (Win == 3)
	{
		Player4WinnerUI();
		Player4FontUI();
		Shape();
	}
	else if (Win == 4)
	{
		Player1AndPlayer2WinnerUI();
		Player1AndPlayer2FontUI();
	}
	else if (Win == 5)
	{
		Player1AndPlayer3WinnerUI();
		Player1AndPlayer3FontUI();
	}
	else if (Win == 6)
	{
		Player1AndPlayer4WinnerUI();
		Player1AndPlayer4FontUI();
	}
	else if (Win == 7)
	{
		Player2AndPlayer3WinnerUI();
		Player2AndPlayer3FontUI();
	}
	else if (Win == 8)
	{
		Player2AndPlayer4WinnerUI();
		Player2AndPlayer4FontUI();
	}
	else if (Win == 9)
	{
		Player3AndPlayer4WinnerUI();
		Player3AndPlayer4FontUI();
	}
	else if (Win == 10)
	{
		Player1AndPlayer2AndPlayer3WinnerUI();
		Player1AndPlayer2AndPlayer3FontUI();
	}
	else if (Win == 11)
	{
		Player1AndPlayer2AndPlayer4WinnerUI();
		Player1AndPlayer2AndPlayer4FontUI();
	}
	else if (Win == 12)
	{
		Player1AndPlayer3AndPlayer4WinnerUI();
		Player1AndPlayer3AndPlayer4FontUI();
	}
	else if (Win == 13)
	{
		Player2AndPlayer3AndPlayer4WinnerUI();
		Player2AndPlayer3AndPlayer4FontUI();
	}
	else if (Win == 14)
	{
		AllPlayerWinnerUI();
		AllPlayerFontUI();
	}
#endif


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
//あっぱれ.
void CWinnerResultUI::WinnerTextUI()
{
	Winner_Img->SetPosition(D3DXVECTOR3(WINNER_FONT.GetPos()));
	Winner_Img->SetScale(D3DXVECTOR3(WINNER_FONT.GetScl()));
	Winner_Img->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	Winner_Img->Render();
}

//プレイヤー壱が一人で一位の立ち絵.
void CWinnerResultUI::Player1WinnerUI()
{
	Player1_Img->SetPosition(P1_SOLO_WIN.GetPos());
	Player1_Img->SetScale(P1_SOLO_WIN.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();
}
//プレイヤー弐が一人で一位の立ち絵.
void CWinnerResultUI::Player2WinnerUI()
{
	Player2_Img->SetPosition(P1_SOLO_WIN.GetPos());
	Player2_Img->SetScale(P1_SOLO_WIN.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();
}
//プレイヤー参が一人で一位の立ち絵.
void CWinnerResultUI::Player3WinnerUI()
{
	Player3_Img->SetPosition(P1_SOLO_WIN.GetPos());
	Player3_Img->SetScale(P1_SOLO_WIN.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();
}
//プレイヤー肆が一人で一位の立ち絵.
void CWinnerResultUI::Player4WinnerUI()
{
	Player4_Img->SetPosition(P1_SOLO_WIN.GetPos());
	Player4_Img->SetScale(P1_SOLO_WIN.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
//プレイヤー壱とプレイヤー弐の立ち絵
void CWinnerResultUI::Player1AndPlayer2WinnerUI()
{
	Player1_Img->SetPosition(DUO_P1_ALL_P1.GetPos());
	Player1_Img->SetScale(DUO_P1_ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player2_Img->SetPosition(DUO_P1_P2_P2.GetPos());
	Player2_Img->SetScale(DUO_P1_P2_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();
}
//プレイヤー壱とプレイヤー参の立ち絵
void CWinnerResultUI::Player1AndPlayer3WinnerUI()
{
	Player1_Img->SetPosition(DUO_P1_ALL_P1.GetPos());
	Player1_Img->SetScale(DUO_P1_ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player2_Img->SetPosition(DUO_ALL_P3_P3.GetPos());
	Player2_Img->SetScale(DUO_ALL_P3_P3.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();
}
//プレイヤー壱とプレイヤー肆の立ち絵
void CWinnerResultUI::Player1AndPlayer4WinnerUI()
{
	Player1_Img->SetPosition(DUO_P1_ALL_P1.GetPos());
	Player1_Img->SetScale(DUO_P1_ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player4_Img->SetPosition(DUO_ALL_P4_P4.GetPos());
	Player4_Img->SetScale(DUO_ALL_P4_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
//プレイヤー弐とプレイヤー参の立ち絵
void CWinnerResultUI::Player2AndPlayer3WinnerUI()
{
	Player2_Img->SetPosition(DUO_P2_ALL_P2.GetPos());
	Player2_Img->SetScale(DUO_P2_ALL_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();

	Player3_Img->SetPosition(DUO_ALL_P3_P3.GetPos());
	Player3_Img->SetScale(DUO_ALL_P3_P3.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();
}
//プレイヤー弐とプレイヤー肆の立ち絵
void CWinnerResultUI::Player2AndPlayer4WinnerUI()
{
	Player2_Img->SetPosition(DUO_P2_ALL_P2.GetPos());
	Player2_Img->SetScale(DUO_P2_ALL_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();

	Player4_Img->SetPosition(DUO_ALL_P4_P4.GetPos());
	Player4_Img->SetScale(DUO_ALL_P4_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
//プレイヤー参とプレイヤー肆の立ち絵
void CWinnerResultUI::Player3AndPlayer4WinnerUI()
{
	Player3_Img->SetPosition(DUO_P3_P4_P3.GetPos());
	Player3_Img->SetScale(DUO_P3_P4_P3.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();

	Player4_Img->SetPosition(DUO_ALL_P4_P4.GetPos());
	Player4_Img->SetScale(DUO_ALL_P4_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー参の立ち絵
void CWinnerResultUI::Player1AndPlayer2AndPlayer3WinnerUI()
{
	Player1_Img->SetPosition(TRIO_P1_ALL_ALL_P1.GetPos());
	Player1_Img->SetScale(TRIO_P1_ALL_ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player2_Img->SetPosition(TRIO_ALL_P2_ALL_P2.GetPos());
	Player2_Img->SetScale(TRIO_ALL_P2_ALL_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();

	Player3_Img->SetPosition(TRIO_P1_P2_P3_P3.GetPos());
	Player3_Img->SetScale(TRIO_P1_P2_P3_P3.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー肆の立ち絵
void CWinnerResultUI::Player1AndPlayer2AndPlayer4WinnerUI()
{
	Player1_Img->SetPosition(TRIO_P1_ALL_ALL_P1.GetPos());
	Player1_Img->SetScale(TRIO_P1_ALL_ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player2_Img->SetPosition(TRIO_ALL_P2_ALL_P2.GetPos());
	Player2_Img->SetScale(TRIO_ALL_P2_ALL_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();

	Player4_Img->SetPosition(TRIO_ALL_ALL_P4_P4.GetPos());
	Player4_Img->SetScale(TRIO_ALL_ALL_P4_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
// プレイヤー壱とプレイヤー参とプレイヤー肆の立ち絵
void CWinnerResultUI::Player1AndPlayer3AndPlayer4WinnerUI()
{
	Player1_Img->SetPosition(TRIO_P1_ALL_ALL_P1.GetPos());
	Player1_Img->SetScale(TRIO_P1_ALL_ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player3_Img->SetPosition(TRIO_P1_P3_P4_P3.GetPos());
	Player3_Img->SetScale(TRIO_P1_P3_P4_P3.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();

	Player4_Img->SetPosition(TRIO_P1_P3_P4_P4.GetPos());
	Player4_Img->SetScale(TRIO_P1_P3_P4_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();

}
// プレイヤー弐とプレイヤー参とプレイヤー肆の立ち絵
void CWinnerResultUI::Player2AndPlayer3AndPlayer4WinnerUI()
{
	Player2_Img->SetPosition(TRIO_P2_P3_P4_P2.GetPos());
	Player2_Img->SetScale(TRIO_P2_P3_P4_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();

	Player3_Img->SetPosition(TRIO_P2_P3_P4_P3.GetPos());
	Player3_Img->SetScale(TRIO_P2_P3_P4_P3.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();

	Player4_Img->SetPosition(TRIO_ALL_ALL_P4_P4.GetPos());
	Player4_Img->SetScale(TRIO_ALL_ALL_P4_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー参とプレイヤー肆の立ち絵
void CWinnerResultUI::AllPlayerWinnerUI()
{
	Player1_Img->SetPosition(ALL_P1.GetPos());
	Player1_Img->SetScale(ALL_P1.GetScl());
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();

	Player2_Img->SetPosition(ALL_P2.GetPos());
	Player2_Img->SetScale(ALL_P2.GetScl());
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();

	Player3_Img->SetPosition(ALL_P3.GetPos());
	Player3_Img->SetScale(ALL_P3.GetScl());
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();

	Player4_Img->SetPosition(ALL_P4.GetPos());
	Player4_Img->SetScale(ALL_P4.GetScl());
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}


//プレイヤー壱のプレイヤー文字
void CWinnerResultUI::Player1FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FIRST_FONT.GetPos()));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FIRST_FONT.GetScl()));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();
}
//プレイヤー弐のプレイヤー文字
void CWinnerResultUI::Player2FontUI()
{
	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FIRST_FONT.GetPos()));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FIRST_FONT.GetScl()));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();
}
//プレイヤー参のプレイヤー文字
void CWinnerResultUI::Player3FontUI()
{
	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FIRST_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FIRST_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();
}
//プレイヤー肆のプレイヤー文字
void CWinnerResultUI::Player4FontUI()
{
	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FIRST_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FIRST_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
// プレイヤー壱とプレイヤー弐のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer2FontUI()
{
	Player1FontUI();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();
}
//プレイヤー壱とプレイヤー参のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer3FontUI()
{
	Player1FontUI();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();
}
//プレイヤー壱とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer4FontUI()
{
	Player1FontUI();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
//プレイヤー弐とプレイヤー参のプレイヤー文字.
void CWinnerResultUI::Player2AndPlayer3FontUI()
{
	Player2FontUI();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();
}
//プレイヤー弐とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player2AndPlayer4FontUI()
{
	Player2FontUI();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
//プレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player3AndPlayer4FontUI()
{
	Player3FontUI();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー参のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer2AndPlayer3FontUI()
{
	Player1FontUI();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_THIRD_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_THIRD_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

}
// プレイヤー壱とプレイヤー弐とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer2AndPlayer4FontUI()
{
	Player1FontUI();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_THIRD_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();

}
// プレイヤー壱とプレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer3AndPlayer4FontUI()
{
	Player1FontUI();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_THIRD_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();

}
// プレイヤー弐とプレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player2AndPlayer3AndPlayer4FontUI()
{
	Player2FontUI();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_THIRD_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::AllPlayerFontUI()
{
	Player1FontUI();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_SECOND_FONT.GetPos()));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_SECOND_FONT.GetScl()));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_THIRD_FONT.GetPos()));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_THIRD_FONT.GetScl()));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FOURTH_FONT.GetPos()));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FOURTH_FONT.GetScl()));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}

//緑の平行四辺形１
void CWinnerResultUI::GPara1()
{
	Para_Img[0]->SetPosition(D3DXVECTOR3(GPARA1.GetPos()));
	Para_Img[0]->SetScale(D3DXVECTOR3(GPARA1.GetScl()));
	Para_Img[0]->SetRotation(D3DXVECTOR3(0, 0, RAD));
	Para_Img[0]->SetPatternNo(0, 0);
	Para_Img[0]->Render();
}
//緑の平行四辺形２
void CWinnerResultUI::GPara2()
{
	Para_Img[1]->SetPosition(D3DXVECTOR3(GPARA2.GetPos()));
	Para_Img[1]->SetScale(D3DXVECTOR3(GPARA2.GetScl()));
	Para_Img[1]->SetRotation(D3DXVECTOR3(0, 0, 0));
	Para_Img[1]->SetPatternNo(0, 0);
	Para_Img[1]->Render();
}
//緑の平行四辺形３
void CWinnerResultUI::GPara3()
{
	Para_Img[2]->SetPosition(D3DXVECTOR3(GPARA3.GetPos()));
	Para_Img[2]->SetScale(D3DXVECTOR3(GPARA3.GetScl()));
	Para_Img[2]->SetPatternNo(0, 0);
	Para_Img[2]->Render();
}
//薄緑の平行四辺形
void CWinnerResultUI::UGPara()
{
	Para_Img[3]->SetPosition(D3DXVECTOR3(UGPARA.GetPos()));
	Para_Img[3]->SetScale(D3DXVECTOR3(UGPARA.GetScl()));
	Para_Img[3]->SetPatternNo(0, 1);
	Para_Img[3]->Render();
}
//角丸四角形.
void CWinnerResultUI::RoundRect()
{
	RoundRect_Img->SetPosition(D3DXVECTOR3(ROUNDRECT.GetPos()));
	RoundRect_Img->SetScale(D3DXVECTOR3(ROUNDRECT.GetScl()));
	RoundRect_Img->Render();
}
//図形まとめ
void CWinnerResultUI::Shape()
{
	GPara1();
	GPara2();
	GPara3();
	UGPara();
	RoundRect();
}


