#include "CWinnerResultUI.h"
#include "SpriteManager/CSpriteManager.h"
#include <iostream>

struct Transform {
	float posX,posY;
	float sclX,sclY;

	//D3DXVECTOR3に変換する関数.
	D3DXVECTOR3 GetPos() const { return D3DXVECTOR3(posX, posY, 0.0f); }
	D3DXVECTOR3 GetScl() const { return D3DXVECTOR3(sclX, sclY, 0.0f); }
};

//あっぱれ文字
static constexpr float WinnerPosX = 145;
static constexpr float WinnerPosY = 90;
static constexpr float WinnerSclX = 300;
static constexpr float WinnerSclY = 90;

//色付きプレイヤーフォントサイズ.
static constexpr float PLAYER_FONT_SCL_X = 135;
static constexpr float PLAYER_FONT_SCL_Y = 43;
//色付きプレイヤーフォント１人目.
static constexpr float PLAYER_FONT_FIRST_POS_X = 165;
static constexpr float PLAYER_FONT_FIRST_POS_Y = 180;
//色付きプレイヤーフォント２人目.
static constexpr float PLAYER_FONT_SECOND_POS_X = 304;
static constexpr float PLAYER_FONT_SECOND_POS_Y = 166;
//色付きプレイヤーフォント3人目.
static constexpr float PLAYER_FONT_THIRD_POS_X = 443;
static constexpr float PLAYER_FONT_THIRD_POS_Y = 152;
//色付きプレイヤーフォント4人目.
static constexpr float PLAYER_FONT_FOURTH_POS_X = 582;
static constexpr float PLAYER_FONT_FOURTH_POS_Y = 138;

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



CWinnerResultUI::CWinnerResultUI()
	:Win (0)
{
	BackGround_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround);	//背景.
	Winner_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerText);		//あっぱれ.
	Player1_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);			//プレイヤー壱
	Player2_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);			//プレイヤー弐
	Player3_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);			//プレイヤー参
	Player4_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);			//プレイヤー肆

	for (int i = 0; i < Font_Max; i++)
	{
		PlayerFont_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerFont);	//色付きプレイヤーフォント
	}


}

CWinnerResultUI::~CWinnerResultUI()
{
	BackGround_Img = nullptr;
	Winner_Img = nullptr;
	Player1_Img = nullptr;
	Player2_Img = nullptr;
	Player3_Img = nullptr;
	Player4_Img = nullptr;

	for (int i = 0; i < Font_Max; i++)
	{
		PlayerFont_Img[i] = nullptr;
	}
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
	}
	else if (Win == 1)
	{
		Player2WinnerUI();
		Player2FontUI();
	}
	else if (Win == 2)
	{
		Player3WinnerUI();
		Player3FontUI();
	}
	else if (Win == 3)
	{
		Player4WinnerUI();
		Player4FontUI();
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
	Winner_Img->SetPosition(D3DXVECTOR3(WinnerPosX, WinnerPosY, 0));
	Winner_Img->SetScale(D3DXVECTOR3(WinnerSclX, WinnerSclY, 0));
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
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();
}
//プレイヤー弐のプレイヤー文字
void CWinnerResultUI::Player2FontUI()
{
	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();
}
//プレイヤー参のプレイヤー文字
void CWinnerResultUI::Player3FontUI()
{
	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();
}
//プレイヤー肆のプレイヤー文字
void CWinnerResultUI::Player4FontUI()
{
	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
// プレイヤー壱とプレイヤー弐のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer2FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();
}
//プレイヤー壱とプレイヤー参のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer3FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();
}
//プレイヤー壱とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer4FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
//プレイヤー弐とプレイヤー参のプレイヤー文字.
void CWinnerResultUI::Player2AndPlayer3FontUI()
{
	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();
}
//プレイヤー弐とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player2AndPlayer4FontUI()
{
	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
//プレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player3AndPlayer4FontUI()
{
	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー参のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer2AndPlayer3FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_THIRD_POS_X, PLAYER_FONT_THIRD_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

}
// プレイヤー壱とプレイヤー弐とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer2AndPlayer4FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_THIRD_POS_X, PLAYER_FONT_THIRD_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();

}
// プレイヤー壱とプレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player1AndPlayer3AndPlayer4FontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_THIRD_POS_X, PLAYER_FONT_THIRD_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();

}
// プレイヤー弐とプレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::Player2AndPlayer3AndPlayer4FontUI()
{
	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_THIRD_POS_X, PLAYER_FONT_THIRD_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
// プレイヤー壱とプレイヤー弐とプレイヤー参とプレイヤー肆のプレイヤー文字.
void CWinnerResultUI::AllPlayerFontUI()
{
	PlayerFont_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FIRST_POS_X, PLAYER_FONT_FIRST_POS_Y, 0));
	PlayerFont_Img[0]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[0]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[0]->SetPatternNo(0.f, 0.f);
	PlayerFont_Img[0]->Render();

	PlayerFont_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_FONT_SECOND_POS_X, PLAYER_FONT_SECOND_POS_Y, 0));
	PlayerFont_Img[1]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[1]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[1]->SetPatternNo(0.f, 1.f);
	PlayerFont_Img[1]->Render();

	PlayerFont_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_FONT_THIRD_POS_X, PLAYER_FONT_THIRD_POS_Y, 0));
	PlayerFont_Img[2]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[2]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[2]->SetPatternNo(0.f, 2.f);
	PlayerFont_Img[2]->Render();

	PlayerFont_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_FONT_FOURTH_POS_X, PLAYER_FONT_FOURTH_POS_Y, 0));
	PlayerFont_Img[3]->SetScale(D3DXVECTOR3(PLAYER_FONT_SCL_X, PLAYER_FONT_SCL_Y, 0));
	PlayerFont_Img[3]->SetRotation(D3DXVECTOR3(0, 0, -0.1));
	PlayerFont_Img[3]->SetPatternNo(0.f, 3.f);
	PlayerFont_Img[3]->Render();
}
