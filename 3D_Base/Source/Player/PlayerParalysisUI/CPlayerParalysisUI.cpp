#include "CPlayerParalysisUI.h"

CPlayerParalysisUI::CPlayerParalysisUI()
{
	m_pSprite = CSpriteManager::GetSprite2D(CSpM::enImagList::Img_ButtonPush);
}

CPlayerParalysisUI::~CPlayerParalysisUI()
{
}

void CPlayerParalysisUI::Update()
{
	//UIが変更前であれば.
	if (!m_UIChange) {
		m_pSprite->SetPatternNo(0.f, 0.f);
		m_UIChangeTimer += 1.0f;
		if (m_UIChangeTimer > 3.0f) {
			m_UIChange = true;
		}
	}
	//変更後であれば.
	else {
		m_pSprite->SetPatternNo(0.f, 2.f);
		m_UIChangeTimer -= 1.0f;
		if (m_UIChangeTimer < 0.f) { 
			m_UIChange = false;
		}
	}
}

void CPlayerParalysisUI::Draw()
{
	m_pSprite->SetPosition(m_Position);
	m_pSprite->SetScale(D3DXVECTOR3(100.f, 130.f, 1.f));
	m_pSprite->Render();
}
//プレイヤーのポジションを受け取る.
void CPlayerParalysisUI::Paralysis(D3DXVECTOR3 Position)
{
	m_Position= (D3DXVECTOR3(Position.x + 50.f, Position.y - 60.0f, Position.z));
	
}

