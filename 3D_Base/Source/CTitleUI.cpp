#include "CTitleUI.h"
#include "CSpriteManager.h"
#include "CXInput.h"
#include <iostream>

//しきい値.
static constexpr SHORT STICK_THRESHOLD = 16000;	//constexpr:コンパイル時に確定する定数
//タイトル
static constexpr float TITLE_POS_X = 20;
static constexpr float TITLE_POS_Y = 80;
static constexpr float TITLE_SCL_X = 1055;
static constexpr float TITLE_SCL_Y = 180;
//開始.
static constexpr float START_POS_X = 955;
static constexpr float START_POS_Y = 390;
static constexpr float START_SCL_X = 800;
static constexpr float START_SCL_Y = 135;
//終了
static constexpr float FINISH_POS_X = 950;
static constexpr float FINISH_POS_Y = 500;
static constexpr float FINISH_SCL_X = 800;
static constexpr float FINISH_SCL_Y = 135;
//鍋
static constexpr float NABE_POS_X = 190;
static constexpr float NABE_POS_Y = 260;
static constexpr float NABE_SCL_X = 630;
static constexpr float NABE_SCL_Y = 680;

//選択肢
static constexpr float WARIBASHI_POS_X = 760;
static constexpr float WARIBASHI_START_POS_Y = 355;
static constexpr float WARIBASHI_FIN_POS_Y = 465;
static constexpr float WARIBASHI_SCL = 200;


CTitleUI::CTitleUI()
	: m_Select			( enSelect::Start )
	, m_Decided			( false )
	, m_SticTitltOld	( false )
	, m_pController		( nullptr )
{
	m_Title = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TitleBackground);	//背景.
	for (int i = 0; i < Max; i++)
	{
		m_TitleText[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TitleText);	//背景.
	}
	m_Nabe = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TitleNabe);	//鍋
	m_Waribashi = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_TitleWaribashi);	//鍋
}

CTitleUI::~CTitleUI()
{
	m_Title = nullptr;
	for (int i = 0; i < Max; i++)
	{
		m_TitleText[i] = nullptr;
	}
	m_Nabe = nullptr;

	m_Waribashi = nullptr;
}

void CTitleUI::Update()
{

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
			if (m_Select == enSelect::Start)
			{
				m_Select = enSelect::Fin;
			}
			else if (m_Select == enSelect::Fin)
			{
				m_Select = enSelect::Start;
			}
		}
		else if (y < -STICK_THRESHOLD)
		{
			if (m_Select == enSelect::Start)
			{
				m_Select = enSelect::Fin;
			}
			else if (m_Select == enSelect::Fin)
			{
				m_Select = enSelect::Start;
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

void CTitleUI::Draw()
{
	BackUI();
	Title();
	Start();
	Finish();
	Nabe();

	if (m_Select == enSelect::Start)
	{
		STARTWaribashi();
	}
	else if (m_Select == enSelect::Fin)
	{
		FINWaribashi();
	}
}

//背景
void CTitleUI::BackUI()
{
	m_Title->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_Title->SetScale(D3DXVECTOR3(WND_W, WND_H, 0));
	m_Title->Render();
}

//「お先にいただきます!」.
void CTitleUI::Title()
{
	m_TitleText[0]->SetPosition(D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0));
	m_TitleText[0]->SetScale(D3DXVECTOR3(TITLE_SCL_X, TITLE_SCL_Y, 0));
	m_TitleText[0]->SetPatternNo(0.f, 0.f);
	m_TitleText[0]->Render();
}
//「開始」.
void CTitleUI::Start()
{
	m_TitleText[1]->SetPosition(D3DXVECTOR3(START_POS_X, START_POS_Y, 0));
	m_TitleText[1]->SetScale(D3DXVECTOR3(START_SCL_X, START_SCL_Y, 0));
	m_TitleText[1]->SetPatternNo(0.f, 1.f);
	m_TitleText[1]->Render();
}
//「終了」.
void CTitleUI::Finish()
{
	m_TitleText[2]->SetPosition(D3DXVECTOR3(FINISH_POS_X, FINISH_POS_Y, 0));
	m_TitleText[2]->SetScale(D3DXVECTOR3(FINISH_SCL_X, FINISH_SCL_Y, 0));
	m_TitleText[2]->SetPatternNo(0.f, 2.f);
	m_TitleText[2]->Render();
}
//鍋の画像.
void CTitleUI::Nabe()
{
	m_Nabe->SetPosition(D3DXVECTOR3(NABE_POS_X, NABE_POS_Y, 0));
	m_Nabe->SetScale(D3DXVECTOR3(NABE_SCL_X, NABE_SCL_Y, 0));
	m_Nabe->Render();
}
//「開始」の時の選択肢の割りばし.
void CTitleUI::STARTWaribashi()
{
	m_Waribashi->SetPosition(D3DXVECTOR3(WARIBASHI_POS_X, WARIBASHI_START_POS_Y, 0));
	m_Waribashi->SetScale(D3DXVECTOR3(WARIBASHI_SCL, WARIBASHI_SCL, 0));
	m_Waribashi->Render();
}
//「終了」の時の選択肢の割りばし.
void CTitleUI::FINWaribashi()
{
	m_Waribashi->SetPosition(D3DXVECTOR3(WARIBASHI_POS_X, WARIBASHI_FIN_POS_Y, 0));
	m_Waribashi->SetScale(D3DXVECTOR3(WARIBASHI_SCL, WARIBASHI_SCL, 0));
	m_Waribashi->Render();
}
//初期化.
void CTitleUI::Init()
{
	m_Select = enSelect::Start;
	m_Decided = false;
	m_SticTitltOld = false;
}
