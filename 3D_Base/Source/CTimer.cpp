#include "CTimer.h"


CTimer::CTimer()
	:m_TimeMAX(0)
	, m_TimeCount(0)
	, m_pbefore(nullptr)
{
	m_pbefore = CSpM::GetSprite2D(CSpMLstiImg::Img_RED);
	m_Position = { WND_W / 2,50,0 };
}

CTimer::~CTimer()
{
}

void CTimer::Update()
{
	if (m_TimeCount>0) {
		m_TimeCount--;
	}
}

void CTimer::Draw()
{
	m_Rotation.z = D3DXToRadian(180);
	m_pbefore->SetScale({100,100,0});
	m_pbefore->SetPosition(m_Position);
	m_pbefore->SetRotation(m_Rotation);
	m_pbefore->SetAlpha(1);

	m_pbefore->Render2(static_cast<float>(m_TimeCount)/ static_cast<float>(m_TimeMAX));
}
