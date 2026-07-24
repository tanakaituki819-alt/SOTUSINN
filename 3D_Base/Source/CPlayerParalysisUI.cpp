#include "CPlayerParalysisUI.h"

CPlayerParalysisUI::CPlayerParalysisUI()
{
	m_pSprite = CSpriteManager::GetSprite3D(CSpriteManager::enImagList::Img_ButtonPush);
}

CPlayerParalysisUI::~CPlayerParalysisUI()
{
}

void CPlayerParalysisUI::Update()
{
	m_pSprite->SetPatternNo(0.f, 0.f);
}

void CPlayerParalysisUI::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	m_pSprite->SetBillboard(true);
	m_pSprite->Render(View, Proj);
	m_pSprite->SetBillboard(false);
}

void CPlayerParalysisUI::Paralysis(D3DXVECTOR3 Position)
{
	m_pSprite->SetPosition(D3DXVECTOR3(Position.x + 0.5f,Position.y,Position.z + 0.5f));
}

