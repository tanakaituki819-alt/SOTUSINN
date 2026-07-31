#include "CTimer.h"

#include <algorithm> // std::max を使うため
CTimer::CTimer()
	:m_TimeMAX(0)
	, m_TimeCount(0)
	, m_pbefore(nullptr)
	, AlphaC(0)
{
	m_pbefore = CSpM::GetSprite2D(CSpMLstiImg::Img_RED);
	m_Position = { WND_W / 2,75,0 };
	m_pSprite = CSpM::GetSprite2D(CSpMLstiImg::Img_Timer);
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
	m_Rotation.z = D3DXToRadian(0);
	m_pSprite->SetScale({ 150,150,0 });
	m_pSprite->SetPosition(m_Position);
	m_pSprite->SetRotation(m_Rotation);

	
	m_Rotation.z = D3DXToRadian(180);
	m_pbefore->SetScale({ 104,104,0});
	m_pbefore->SetPosition({ m_Position .x,m_Position .y+static_cast<FLOAT>(150.0*0.06),m_Position .z});
	m_pbefore->SetRotation(m_Rotation);

	float Alpha = 1;
	double T = static_cast<float>(m_TimeCount) / static_cast<float>(m_TimeMAX);//正規化した残り時間
	if (T<0.2) {
		m_pbefore->SetPatternNo(2, 0);
	AlphaC += 3.0/60.0;
		Alpha = Quadraticfunction(AlphaC);
	}
	else if (T<0.6) {
		m_pbefore->SetPatternNo(1, 0);
		AlphaC +=1.0/60.0;
		Alpha = Quadraticfunction(AlphaC);
	}
	else {
		m_pbefore->SetPatternNo(0, 0);
	
	}
	if (AlphaC>2) {
		AlphaC = 0;
	}
	m_pbefore->SetAlpha(0.5+0.35*Alpha);
	
	m_pSprite->Render();
	m_pbefore->Render2(T);

}

float CTimer::Quadraticfunction(float i)
{
	float result = 0;
	result = -4 * ((i - 0.5) * (i - 0.5)) + 1;
	// 例：最大輝度を 1.0 から 2.0（または 200%）に上げたい場合
	//result = (-4 * ((i - 0.5) * (i - 0.5)) + 1) * 2.0f;
	// 例：i = 0.8 のときに明るさのピークを持ってきたい場合
	//result = -4.0f * ((i - 0.8f) * (i - 0.8f)) + 1.0f
	result =std::max(0.0f,result);
	return result;
}
