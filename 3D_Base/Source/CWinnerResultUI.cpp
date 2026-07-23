#include "CWinnerResultUI.h"
#include "CSpriteManager.h"
#include <iostream>

//あっぱれ文字
static constexpr float WinnerPosX = 145;
static constexpr float WinnerPosY = 90;
static constexpr float WinnerSclX = 300;
static constexpr float WinnerSclY = 90;
//プレイヤー壱が一人で一位の立ち絵.
static constexpr float Player1PosX = 135;
static constexpr float Player1PosY = 90;
static constexpr float Player1SclX = 400;
static constexpr float Player1SclY = 630;

CWinnerResultUI::CWinnerResultUI()
	:Win (0)
{
	BackGround_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround);	//背景.
	Winner_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerText);		//あっぱれ.
	Player1_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);			//プレイヤー壱
	Player2_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);			//プレイヤー弐
	Player3_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);			//プレイヤー参
	Player4_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);			//プレイヤー肆
}

CWinnerResultUI::~CWinnerResultUI()
{
	BackGround_Img = nullptr;
	Winner_Img = nullptr;
	Player1_Img = nullptr;
	Player2_Img = nullptr;
	Player3_Img = nullptr;
	Player4_Img = nullptr;
}

void CWinnerResultUI::Update()
{
#if 0
	//実行中に動かすやつ
	static float ananana = 0;
	static float ananana2 = 0;

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


	if (GetAsyncKeyState('Q') & 0x8000)
	{
		Win = 0;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		Win = 1;
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		Win = 2;
	}
	if (GetAsyncKeyState('R') & 0x8000)
	{
		Win = 3;
	}

}

void CWinnerResultUI::Draw()
{
	WinnerResultBackUI();
	WinnerTextUI();

	//仮 2026.07.23
	if (Win == 0)
	{
		Player1WinnerUI();
	}
	else if (Win == 1)
	{
		Player2WinnerUI();
	}
	else if (Win == 2)
	{
		Player3WinnerUI();
	}
	else if (Win == 3)
	{
		Player4WinnerUI();
	}
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
	Player1_Img->SetPosition(D3DXVECTOR3(Player1PosX, Player1PosY, 0));
	Player1_Img->SetScale(D3DXVECTOR3(Player1SclX, Player1SclY, 0));
	Player1_Img->SetPatternNo(1.f, 0.f);
	Player1_Img->Render();
}
//プレイヤー弐が一人で一位の立ち絵.
void CWinnerResultUI::Player2WinnerUI()
{
	Player2_Img->SetPosition(D3DXVECTOR3(Player1PosX, Player1PosY, 0));
	Player2_Img->SetScale(D3DXVECTOR3(Player1SclX, Player1SclY, 0));
	Player2_Img->SetPatternNo(1.f, 0.f);
	Player2_Img->Render();
}
//プレイヤー参が一人で一位の立ち絵.
void CWinnerResultUI::Player3WinnerUI()
{
	Player3_Img->SetPosition(D3DXVECTOR3(Player1PosX, Player1PosY, 0));
	Player3_Img->SetScale(D3DXVECTOR3(Player1SclX, Player1SclY, 0));
	Player3_Img->SetPatternNo(1.f, 0.f);
	Player3_Img->Render();
}
//プレイヤー肆が一人で一位の立ち絵.
void CWinnerResultUI::Player4WinnerUI()
{
	Player4_Img->SetPosition(D3DXVECTOR3(Player1PosX, Player1PosY, 0));
	Player4_Img->SetScale(D3DXVECTOR3(Player1SclX, Player1SclY, 0));
	Player4_Img->SetPatternNo(1.f, 0.f);
	Player4_Img->Render();
}
