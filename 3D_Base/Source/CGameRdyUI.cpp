#include "CGameRdyUI.h"


CGameRdyUI::CGameRdyUI()
	: m_pRightHand()
	, m_pLeftHand()
{
	for (int i = 0; i < hand_Max; i++)
	{
		m_pRightHand[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);
		m_pLeftHand[i]  = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);
	}
}

CGameRdyUI::~CGameRdyUI()
{
}

void CGameRdyUI::Draw()
{
	RightHand();
	LeftHand();
}

void CGameRdyUI::RightHand()
{
	//プレイヤー1.
	m_pRightHand[0]->SetPosition(D3DXVECTOR3(8 + 70.f, WND_H - 230.f, 0.f));
	m_pRightHand[0]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pRightHand[0]->SetPatternNo(0.f,1.f);
	m_pRightHand[0]->Render();
	//プレイヤー2.
	m_pRightHand[1]->SetPosition(D3DXVECTOR3(64.f * 5.2f + 70.f, WND_H - 230.f, 0.f));
	m_pRightHand[1]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pRightHand[1]->SetPatternNo(0.f, 1.f);
	m_pRightHand[1]->Render();
	//プレイヤー3.
	m_pRightHand[2]->SetPosition(D3DXVECTOR3(64.f * 10.2f + 70.f, WND_H - 230.f, 0.f));
	m_pRightHand[2]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pRightHand[2]->SetPatternNo(0.f, 1.f);
	m_pRightHand[2]->Render();
	//プレイヤー4.
	m_pRightHand[3]->SetPosition(D3DXVECTOR3(64.f * 15.2 + 70.f, WND_H - 230.f, 0.f));
	m_pRightHand[3]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pRightHand[3]->SetPatternNo(0.f, 1.f);
	m_pRightHand[3]->Render();
}

void CGameRdyUI::LeftHand()
{
	static int x = 0;
	if (GetAsyncKeyState('W') & 0x8000) {
		x += 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		x -= 1;
	}
	//プレイヤー1.
	m_pLeftHand[0]->SetPosition(D3DXVECTOR3(8 + 71.f, WND_H - 230.f, 0.f));
	m_pLeftHand[0]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pLeftHand[0]->SetPatternNo(0.f, 0.f);
	m_pLeftHand[0]->Render();
	//プレイヤー2.
	m_pLeftHand[1]->SetPosition(D3DXVECTOR3(64.f * 5.2f + 71.f, WND_H - 230.f, 0.f));
	m_pLeftHand[1]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pLeftHand[1]->SetPatternNo(0.f, 0.f);
	m_pLeftHand[1]->Render();
	//プレイヤー3.
	m_pLeftHand[2]->SetPosition(D3DXVECTOR3(64.f * 10.2f + 71.f, WND_H - 230.f, 0.f));
	m_pLeftHand[2]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pLeftHand[2]->SetPatternNo(0.f, 0.f);
	m_pLeftHand[2]->Render();
	//プレイヤー4.
	m_pLeftHand[3]->SetPosition(D3DXVECTOR3(64.f * 15.2 + 71.f, WND_H - 230.f, 0.f));
	m_pLeftHand[3]->SetScale(D3DXVECTOR3(150, 170.f, 0.f));
	m_pLeftHand[3]->SetPatternNo(0.f, 0.f);
	m_pLeftHand[3]->Render();
}
