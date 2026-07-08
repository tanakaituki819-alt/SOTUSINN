#include "CCharacterUI.h"
#include "CSpriteManager.h"
#include <iostream>

CCharacterUI::CCharacterUI()
{

	//画像読み込み、ポジションの設定、スケール設定.
	//女将さん.
	m_pPlayerUI[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player1);
	m_pPlayerUI[0]->SetPosition	(D3DXVECTOR3(16.f, 48.f, 1.f)	);
	m_pPlayerUI[0]->SetScale	(D3DXVECTOR3(260.f, 450.f, 0.f)	);
	//男.
	m_pPlayerUI[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player2);
	m_pPlayerUI[1]->SetPosition	(D3DXVECTOR3(64.f * 4.5, 48.f, 1.f));
	m_pPlayerUI[1]->SetScale	(D3DXVECTOR3(260.f, 450.f, 0.f));
	//女子高生.
	m_pPlayerUI[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player3);
	m_pPlayerUI[2]->SetPosition	(D3DXVECTOR3(64.f * 10, 18.f, 1.f));
	m_pPlayerUI[2]->SetScale	(D3DXVECTOR3(280.f, 480.f, 0.f));
	//おばちゃん.
	m_pPlayerUI[3] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Player4);
	m_pPlayerUI[3]->SetPosition	(D3DXVECTOR3(64.f * 15, 28.f, 1.f));
	m_pPlayerUI[3]->SetScale	(D3DXVECTOR3(270.f, 470.f, 0.f));




}

CCharacterUI::~CCharacterUI()
{
}

void CCharacterUI::Update()
{
}

void CCharacterUI::Draw()
{
	static int x = 0;
	if (GetAsyncKeyState('W') & 0x8000) {
		x += 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		x -= 1;
	}
	//影.
	m_pPlayerUI[0]->SetPosition(D3DXVECTOR3(16.f + 44, 48.f, 1.f));
	m_pPlayerUI[1]->SetPosition(D3DXVECTOR3(64.f * 4.5 + 44, 48.f, 1.f));
	m_pPlayerUI[2]->SetPosition(D3DXVECTOR3(64.f * 10 + 44, 18.f, 1.f));
	m_pPlayerUI[3]->SetPosition(D3DXVECTOR3(64.f * 15 + 33, 28.f, 1.f));

	m_pPlayerUI[0]->SetCOLOR({1,0,0});
	m_pPlayerUI[1]->SetCOLOR({1,0,0});
	m_pPlayerUI[2]->SetCOLOR({1,0,0});
	m_pPlayerUI[3]->SetCOLOR({1,0,0});

	for (int i = 0; i < PlayerMax; i++)
	{
		m_pPlayerUI[i]->SetisCOLOR(true);
		m_pPlayerUI[i]->Render();
		m_pPlayerUI[i]->SetisCOLOR(false);
	}
	//おばちゃん本体.
	m_pPlayerUI[0]->SetPosition(D3DXVECTOR3(16.f, 48.f, 1.f));
	m_pPlayerUI[1]->SetPosition(D3DXVECTOR3(64.f * 4.5, 48.f, 1.f));
	m_pPlayerUI[2]->SetPosition(D3DXVECTOR3(64.f * 10, 18.f, 1.f));
	m_pPlayerUI[3]->SetPosition(D3DXVECTOR3(64.f * 15, 28.f, 1.f));
	std::cout << x << std::endl;

	for (int i = 0; i < PlayerMax; i++)
	{
		m_pPlayerUI[i]->Render();
	}
}
