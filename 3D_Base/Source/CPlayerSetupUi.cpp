#include "CPlayerSetupUI.h"
#include "CSpriteManager.h"
#include <iostream>


CPlayerSetupUI::CPlayerSetupUI()
{	
	////画像読み込み、ポジションの設定、スケール設定.
	//m_pPlayerSetUpUI[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	//m_pPlayerSetUpUI[0]->SetPosition(D3DXVECTOR3(16.f , 32.f , 0.f));
	//m_pPlayerSetUpUI[0]->SetScale	(D3DXVECTOR3(300.f, 500.f, 0.f));
	//m_pPlayerSetUpUI[0]->SetPatternNo(0.f, 0.f);

	//m_pPlayerSetUpUI[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	//m_pPlayerSetUpUI[1]->SetPosition(D3DXVECTOR3(64.f * 5,	32.f , 0.f));
	//m_pPlayerSetUpUI[1]->SetScale	(D3DXVECTOR3(300.f,		500.f, 0.f));
	//m_pPlayerSetUpUI[1]->SetPatternNo(1.f, 0.f);

	//m_pPlayerSetUpUI[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	//m_pPlayerSetUpUI[2]->SetPosition(D3DXVECTOR3(64.f * 10, -24.f, 0.f));
	//m_pPlayerSetUpUI[2]->SetScale	(D3DXVECTOR3(350.f,	550.f, 0.f));
	//m_pPlayerSetUpUI[2]->SetPatternNo(2.f, 0.f);

	//m_pPlayerSetUpUI[3] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	//m_pPlayerSetUpUI[3]->SetPosition(D3DXVECTOR3(64.f * 10, -24.f, 0.f));
	//m_pPlayerSetUpUI[3]->SetScale(D3DXVECTOR3(350.f, 550.f, 0.f));
	//m_pPlayerSetUpUI[3]->SetPatternNo(3.f, 0.f);
}

CPlayerSetupUI::~CPlayerSetupUI()
{
}

void CPlayerSetupUI::Update()
{
}

void CPlayerSetupUI::Draw()
{

	//画像読み込み、ポジションの設定、スケール設定.
	m_pPlayerSetUpUI[0] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	m_pPlayerSetUpUI[0]->SetPosition(D3DXVECTOR3(16.f,0.f,0.f));
	m_pPlayerSetUpUI[0]->SetScale(D3DXVECTOR3(300.f, 500.f, 0.f));
	m_pPlayerSetUpUI[0]->SetPatternNo(0.f, 0.f);
	m_pPlayerSetUpUI[0]->Render();

	m_pPlayerSetUpUI[1] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	m_pPlayerSetUpUI[1]->SetPosition(D3DXVECTOR3(64.f * 5, 0.f, 0.f));
	m_pPlayerSetUpUI[1]->SetScale(D3DXVECTOR3(300.f, 500.f, 0.f));
	m_pPlayerSetUpUI[1]->SetPatternNo(1.f, 0.f);
	m_pPlayerSetUpUI[1]->Render();

	m_pPlayerSetUpUI[2] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	m_pPlayerSetUpUI[2]->SetPosition(D3DXVECTOR3(64.f * 10, 0.f, 0.f));
	m_pPlayerSetUpUI[2]->SetScale(D3DXVECTOR3(300.f, 500.f, 0.f));
	m_pPlayerSetUpUI[2]->SetPatternNo(2.f, 0.f);
	m_pPlayerSetUpUI[2]->Render();

	m_pPlayerSetUpUI[3] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
	m_pPlayerSetUpUI[3]->SetPosition(D3DXVECTOR3(64.f * 15, 0.f, 0.f));
	m_pPlayerSetUpUI[3]->SetScale(D3DXVECTOR3(300.f, 500.f, 0.f));
	m_pPlayerSetUpUI[3]->SetPatternNo(3.f, 0.f);
	m_pPlayerSetUpUI[3]->Render();

	

}
