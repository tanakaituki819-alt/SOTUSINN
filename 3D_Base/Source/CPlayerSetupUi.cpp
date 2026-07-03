#include "CPlayerSetupUi.h"
#include "CSpriteManager.h"

CPlayerSetupUi::CPlayerSetupUi()
{	
	//画像読み込み、ポジションの設定、スケール設定.
	m_pPlayerSetUpUI[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);
	m_pPlayerSetUpUI[0]->SetPosition(D3DXVECTOR3(16.f , 32.f , 1.f));
	m_pPlayerSetUpUI[0]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f));

	m_pPlayerSetUpUI[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);
	m_pPlayerSetUpUI[1]->SetPosition(D3DXVECTOR3(64.f * 5,	32.f , 1.f));
	m_pPlayerSetUpUI[1]->SetScale	(D3DXVECTOR3(300.f,		500.f, 0.f));

	m_pPlayerSetUpUI[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);
	m_pPlayerSetUpUI[2]->SetPosition(D3DXVECTOR3(64.f * 10, -24.f, 1.f));
	m_pPlayerSetUpUI[2]->SetScale	(D3DXVECTOR3(350.f,		550.f, 0.f));
}

CPlayerSetupUi::~CPlayerSetupUi()
{
}

void CPlayerSetupUi::Update()
{
}

void CPlayerSetupUi::Draw()
{
}
