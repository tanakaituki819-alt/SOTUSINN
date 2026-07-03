#include "CCharacterUI.h"
#include "CSpriteManager.h"

CCharacterUI::CCharacterUI()
{
	//画像読み込み、ポジションの設定、スケール設定.
	m_pPlayerUI[0] = CSpriteManager::GetSprite2D(CSpMLstiImg::Img_Player1);
	m_pPlayerUI[0]->SetPosition	(D3DXVECTOR3(64.f, 64.f, 1.f)	);
	m_pPlayerUI[0]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f)	);

	m_pPlayerUI[1] = CSpriteManager::GetSprite2D(CSpMLstiImg::Img_Player2);
	m_pPlayerUI[1]->SetPosition	(D3DXVECTOR3(64.f * 5, 64.f, 1.f));
	m_pPlayerUI[1]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f));

	m_pPlayerUI[2] = CSpriteManager::GetSprite2D(CSpMLstiImg::Img_Player3);
	m_pPlayerUI[2]->SetPosition	(D3DXVECTOR3(64.f * 10, 64.f, 1.f));
	m_pPlayerUI[2]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f));

	m_pPlayerUI[3] = CSpriteManager::GetSprite2D(CSpMLstiImg::Img_Player4);
	m_pPlayerUI[3]->SetPosition	(D3DXVECTOR3(64.f * 15, 64.f, 1.f));
	m_pPlayerUI[3]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f));
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
