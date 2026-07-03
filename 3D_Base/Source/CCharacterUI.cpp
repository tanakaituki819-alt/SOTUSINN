#include "CCharacterUI.h"
#include "CSpriteManager.h"

CCharacterUI::CCharacterUI()
{
	//画像読み込み、ポジションの設定、スケール設定.
	m_pPlayerUI[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);
	m_pPlayerUI[0]->SetPosition	(D3DXVECTOR3(16.f, 32.f, 1.f)	);
	m_pPlayerUI[0]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f)	);

	m_pPlayerUI[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);
	m_pPlayerUI[1]->SetPosition	(D3DXVECTOR3(64.f * 5, 32.f, 1.f));
	m_pPlayerUI[1]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f));

	m_pPlayerUI[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);
	m_pPlayerUI[2]->SetPosition	(D3DXVECTOR3(64.f * 10, -24.f, 1.f));
	m_pPlayerUI[2]->SetScale	(D3DXVECTOR3(350.f, 550.f, 0.f));

	m_pPlayerUI[3] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);
	m_pPlayerUI[3]->SetPosition	(D3DXVECTOR3(64.f * 15, 12.f, 1.f));
	m_pPlayerUI[3]->SetScale	(D3DXVECTOR3(320.f, 520.f, 0.f));
}

CCharacterUI::~CCharacterUI()
{
}

void CCharacterUI::Update()
{
}

void CCharacterUI::Draw()
{
	for (int i = 0; i < PlayerMax; i++)
	{
		m_pPlayerUI[i]->Render();
	}
	CUIObject::Draw();
}
