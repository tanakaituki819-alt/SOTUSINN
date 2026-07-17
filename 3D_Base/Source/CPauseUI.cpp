#include "CPauseUI.h"
#include "CSpriteManager.h"
#include "CXInput.h"
#include <iostream>

//しきい値.
static constexpr SHORT STICK_THRESHOLD = 16000;	//constexpr:コンパイル時に確定する定数
//箸休め画像.
static constexpr float PausePosX = 245;
static constexpr float PausePosY = 10;
static constexpr float PauseSclX = 780;
static constexpr float PauseSclY = 200;
//ゲームに戻るの選択肢画像.
static constexpr float ResumeGameTextPosX = 360;
static constexpr float ResumeGameTextPosY = 310;
static constexpr float ResumeGameTextSclX = 840;
static constexpr float ResumeGameTextSclY = 250;
//タイトルに戻るの選択肢画像.
static constexpr float ReturnToTitleTextPosX = 240;
static constexpr float ReturnToTitleTextPosY = 500;
static constexpr float ReturnToTitleTextSclX = 840;
static constexpr float ReturnToTitleTextSclY = 250;

CPauseUI::CPauseUI()
	:m_pPauseImg		( nullptr )
	, m_pPauseOptionImg	(  )
	, m_Select			( enSelect::ResumeGame )
	, m_Decided			( false )
	, m_SticTitltOld	( false )
	,m_pController		( nullptr )
{
	//箸休め画像の取得.
	m_pPauseImg = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Pause);

	//箸休め中の選択肢画像の取得.
	for (int i = 0; i < Option_MAX; i++)
	{
		m_pPauseOptionImg[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_PauseOptions);
	}
}

CPauseUI::~CPauseUI()
{
	//画像はここでは初期化しない.
	m_pPauseImg			= nullptr;
	for (int i = 0; i < Option_MAX; i++)
	{
		m_pPauseOptionImg[i] = nullptr;
	}
}

void CPauseUI::Update()
{

	static float ananana=0;
	static float ananana2=0;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		ananana++;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		ananana--;
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
	std::cout << ananana2<< std::endl;




	if (m_pController == nullptr)
	{
		return;
	}
	if (m_pController->IsConnect() == false)
	{
		return;
	}

	//左スティックの縦方向.
	SHORT y = m_pController->GetLThumbY();
	
	//しきい値を超えて倒れているかどうか
	bool StickTiltNow = (y > STICK_THRESHOLD) || (y < -STICK_THRESHOLD);

	//エッジ検出
	if (StickTiltNow && m_SticTitltOld == false)
	{
		//上に倒した.
		if (y > STICK_THRESHOLD)
		{
			if (m_Select == enSelect::ResumeGame)
			{
				m_Select = enSelect::ReturnToTitle;
			}
			else if (m_Select == enSelect::ReturnToTitle)
			{
				m_Select = enSelect::ResumeGame;
			}

		}
		else if(y < -STICK_THRESHOLD)
		{
			if (m_Select == enSelect::ResumeGame)
			{
				m_Select = enSelect::ReturnToTitle;
			}
			else if (m_Select == enSelect::ReturnToTitle)
			{
				m_Select = enSelect::ResumeGame;
			}
		}

		m_SticTitltOld = StickTiltNow;
	}

	//Aボタンで決定.
	if (m_pController->IsDown(CXInput::A, true))
	{
		m_Decided = true;
	}


}

void CPauseUI::Draw()
{
	if (m_pPauseImg == nullptr || m_pPauseOptionImg[0] == nullptr || m_pPauseOptionImg[1] == nullptr)
	{
		return;
	}

	m_pPauseImg->SetPosition(D3DXVECTOR3(PausePosX, PausePosY, 1.f));
	m_pPauseImg->SetScale(D3DXVECTOR3(PauseSclX, PauseSclY, 1.f));
	m_pPauseImg->Render();

	m_pPauseOptionImg[0]->SetPosition(D3DXVECTOR3(ResumeGameTextPosX, ResumeGameTextPosY, 1.f));
	m_pPauseOptionImg[0]->SetScale(D3DXVECTOR3(ResumeGameTextSclX, ResumeGameTextSclY, 1.f));
	m_pPauseOptionImg[0]->SetPatternNo(0.f, 0.f);
	m_pPauseOptionImg[0]->Render();

	m_pPauseOptionImg[1]->SetPosition(D3DXVECTOR3(ReturnToTitleTextPosX, ReturnToTitleTextPosY, 1.f));
	m_pPauseOptionImg[1]->SetScale(D3DXVECTOR3(ReturnToTitleTextSclX, ReturnToTitleTextSclY, 1.f));
	m_pPauseOptionImg[1]->SetPatternNo(0.f, 1.f);
	m_pPauseOptionImg[1]->Render();
}

//ポーズ画面を開くときのの初期化
void CPauseUI::OpenInit()
{
	m_Select = enSelect::ResumeGame;
	m_Decided = false;
	m_SticTitltOld = false;
}

//ポーズ画面を閉じるときの初期化
void CPauseUI::CloseInit()
{
	m_Decided = false;
}
