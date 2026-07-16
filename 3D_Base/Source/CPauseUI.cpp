#include "CPauseUI.h"
#include "CSpriteManager.h"
#include "CXInput.h"
#include <iostream>

static constexpr SHORT STICK_THRESHOLD = 16000;	//しきい値

CPauseUI::CPauseUI()
	:m_pPauseImg(nullptr)
	, m_Select(enSelect::ResumeGame)
	, m_Decided(false)
	, m_SticTitltOld(false)
	,m_pController(nullptr)
{
	//箸休め画像の取得
	m_pPauseImg = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Pause);
	m_pPauseImg->SetPosition(D3DXVECTOR3(210, 1.f, 1.f));
	m_pPauseImg->SetScale(D3DXVECTOR3(840, 250, 1.f));

}

CPauseUI::~CPauseUI()
{
	//画像はここでは初期化しない.
	m_pPauseImg = nullptr;
}

void CPauseUI::Update()
{

	//static float ananana=0;
	//static float ananana2=0;

	//if (GetAsyncKeyState('W') & 0x8000)
	//{
	//	ananana++;
	//}
	//if (GetAsyncKeyState('S') & 0x8000)
	//{
	//	ananana--;
	//}
	//if (GetAsyncKeyState('D') & 0x8000)
	//{
	//	ananana2++;
	//}
	//if (GetAsyncKeyState('A') & 0x8000)
	//{
	//	ananana2--;
	//}

	//FILE* pFile;
	////stdout（標準出力）を新しく作成したコンソールにリダイレクト
	//freopen_s(&pFile, "CONOUT$", "w", stdout);
	//std::cout << ananana<< std::endl;


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
		else
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

		//Aボタンで決定.
		if (m_pController->IsDown(CXInput::A, true))
		{
			m_Decided = true;
		}

	}

}

void CPauseUI::Draw()
{
	if (m_pPauseImg == nullptr)
	{
		return;
	}

	m_pPauseImg->Render();
	

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
