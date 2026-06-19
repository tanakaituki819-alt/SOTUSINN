#include "CExplosion.h"

CExplosion::CExplosion()

{
}

CExplosion::~CExplosion()
{
}

void CExplosion::Update()
{


	const POINTS PatternMax = m_pSprite->GetPatternMax();

#if 1
	m_AnimCnt++;
	if (m_AnimCnt % 10 == 0) {
		m_PatternNo.x = (m_AnimCnt / 10) % PatternMax.x;
		m_PatternNo.y = (m_AnimCnt / 10) / PatternMax.y;
	}
	if (m_AnimCnt / 10 >= PatternMax.x * PatternMax.y)
	{
		m_AnimCnt = 0;
	}
#else
	m_AnimCnt++;
	if (m_AnimCnt == 10) {
		m_AnimCnt = 0;
		m_PatternNo.x++;
		if (m_PatternNo.x >= PatternMax.x) {
			m_PatternNo.y++;
			m_PatternNo.x = 0;
			if (m_PatternNo.y >= PatternMax.y) {
				m_PatternNo.y = 0;
			}
		}
	}
#endif
}

void CExplosion::Draw( D3DXMATRIX& View, D3DXMATRIX& Proj )
{
	m_pSprite->SetPatternNo( m_PatternNo.x, m_PatternNo.y );

	m_pSprite->SetBillboard( true );

	CSpriteObject::Draw( View, Proj );

	m_pSprite->SetBillboard( false );
}
