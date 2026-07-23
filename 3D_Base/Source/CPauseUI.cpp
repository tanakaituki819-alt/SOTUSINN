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
static constexpr float ResumeGameTextPosY = 240;
//タイトルに戻るの選択肢画像.
static constexpr float ReturnToTitleTextPosX = 240;
static constexpr float ReturnToTitleTextPosY = 450;
//テキストスケール.
static constexpr float TextSclX = 840;
static constexpr float TextSclY = 250;
//ゲームに戻るの選択肢の枠画像.
static constexpr float ResumeGameFramePosX = 635;
static constexpr float ResumeGameFramePosY = 375;
//タイトルに戻るの選択肢の枠画像.
static constexpr float ReturnToTitleFramePosX = 635;
static constexpr float ReturnToTitleFramePosY = 588;
//選択肢の枠画像.
static constexpr float FrameSclX = 910;
static constexpr float FrameSclY = 280;

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

	//選択中の枠画像の取得.
	m_pPauseSelectionFrameImg = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_SelectionFrame);
	
	//背景
	m_pDimImg = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Fad);
}

CPauseUI::~CPauseUI()
{
	//画像はここでは初期化しない.
	m_pPauseImg	= nullptr;
	for (int i = 0; i < Option_MAX; i++)
	{
		m_pPauseOptionImg[i] = nullptr;
	}
	m_pPauseSelectionFrameImg = nullptr;
	m_pDimImg = nullptr;
}

void CPauseUI::Update()
{

#if 1
	//実行中に動かすやつ
	static float ananana=0;
	static float ananana2=0;

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
	std::cout << ananana<< std::endl;

#endif

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

	}
		m_SticTitltOld = StickTiltNow;

	//Aボタンで決定.
	if (m_pController->IsDown(CXInput::A, true))
	{
		m_Decided = true;
	}


}

void CPauseUI::Draw()
{
	//読み込めてないなら消え失せろ
	if (m_pPauseImg == nullptr || m_pPauseOptionImg[0] == nullptr || m_pPauseOptionImg[1] == nullptr|| m_pPauseSelectionFrameImg == nullptr)
	{
		return;
	}

	//ポーズ中はゲームメインを暗くするための画像.
	m_pDimImg->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pDimImg->SetScale(D3DXVECTOR3(WND_W, WND_H, 0.f));
	m_pDimImg->SetisCOLOR(true);
	m_pDimImg->SetCOLOR({ 0.f, 0.f, 0.f });   //黒.
	m_pDimImg->SetAlpha(0.6f);
	m_pDimImg->Render();


	//箸休め中画像
	m_pPauseImg->SetPosition(D3DXVECTOR3(PausePosX, PausePosY, 1.f));
	m_pPauseImg->SetScale(D3DXVECTOR3(PauseSclX, PauseSclY, 1.f));
	m_pPauseImg->Render();

	//ゲームに戻るテキスト画像
	m_pPauseOptionImg[0]->SetPosition(D3DXVECTOR3(ResumeGameTextPosX, ResumeGameTextPosY, 1.f));
	m_pPauseOptionImg[0]->SetScale(D3DXVECTOR3(TextSclX, TextSclY, 1.f));
	m_pPauseOptionImg[0]->SetPatternNo(0.f, 0.f);
	m_pPauseOptionImg[0]->Render();

	//タイトルに戻るテキスト画像
	m_pPauseOptionImg[1]->SetPosition(D3DXVECTOR3(ReturnToTitleTextPosX, ReturnToTitleTextPosY, 1.f));
	m_pPauseOptionImg[1]->SetScale(D3DXVECTOR3(TextSclX, TextSclY, 1.f));
	m_pPauseOptionImg[1]->SetPatternNo(0.f, 1.f);
	m_pPauseOptionImg[1]->Render();

	//選択肢の表示.
	if (m_Select == enSelect::ResumeGame)
	{
		m_pPauseSelectionFrameImg->SetPosition(D3DXVECTOR3(ResumeGameFramePosX, ResumeGameFramePosY, 1.f));
		m_pPauseSelectionFrameImg->SetScale(D3DXVECTOR3(FrameSclX, FrameSclY, 1.f));
		m_pPauseSelectionFrameImg->Render();
	}
	else if (m_Select == enSelect::ReturnToTitle)
	{
		m_pPauseSelectionFrameImg->SetPosition(D3DXVECTOR3(ReturnToTitleFramePosX, ReturnToTitleFramePosY, 1.f));
		m_pPauseSelectionFrameImg->SetScale(D3DXVECTOR3(FrameSclX, FrameSclY, 1.f));
		m_pPauseSelectionFrameImg->Render();
	}

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
