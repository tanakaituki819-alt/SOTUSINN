#include "CButtonMashBattleUI.h"
#include "SpriteManager/CSpriteManager.h"


CButtonMahBattleUI::CButtonMahBattleUI()
{
	//ゲージ画像の取得
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		m_pGaugeImg[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_Gage);
	}

}

CButtonMahBattleUI::~CButtonMahBattleUI()
{
	//ゲージ画像
	for (int i = GAUGE_MAX - 1; i >= 0; i--)
	{
		m_pGaugeImg[i] = nullptr;
	}
}

void CButtonMahBattleUI::Update()
{
}

void CButtonMahBattleUI::Draw()
{
}

//連打対決の初期化.
void CButtonMahBattleUI::InitMashBattle()
{
}

//プレイヤー壱のゲージの描画.
void CButtonMahBattleUI::RenderPlayer1Gauge()
{

	m_pGaugeImg[0]->SetScale(D3DXVECTOR3(10.f,10.f,0.f));
	m_pGaugeImg[0]->SetPatternNo(0, 0);
	m_pGaugeImg[0]->Render();
}
//プレイヤー弐のゲージの描画.
void CButtonMahBattleUI::RenderPlayer2Gauge()
{

	m_pGaugeImg[1]->SetScale(D3DXVECTOR3(10.f, 10.f, 0.f));
	m_pGaugeImg[1]->SetPatternNo(0, 1);
	m_pGaugeImg[1]->Render();
}
//プレイヤー参のゲージの描画.
void CButtonMahBattleUI::RenderPlayer3Gauge()
{

	m_pGaugeImg[2]->SetScale(D3DXVECTOR3(10.f, 10.f, 0.f));
	m_pGaugeImg[2]->SetPatternNo(0, 2);
	m_pGaugeImg[2]->Render();
}
//プレイヤー肆のゲージの描画.
void CButtonMahBattleUI::RenderPlayer4Gauge()
{

	m_pGaugeImg[3]->SetScale(D3DXVECTOR3(10.f, 10.f, 0.f));
	m_pGaugeImg[3]->SetPatternNo(0, 3);
	m_pGaugeImg[3]->Render();
}
