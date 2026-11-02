#include "GameStartIntro.h"

GameStartIntro::GameStartIntro()
{
	m_pKONDATE = CSpM::GetSprite2D(CSpMLstiImg::IMG_KONDATE);
	m_pKONDATE->SetScale({ SCX, WND_H - offSetY * 2,0.1 });
}

GameStartIntro::~GameStartIntro()
{
}

void GameStartIntro::Update()
{
	if (GetAsyncKeyState('A')) {
		Timu = 0;
		C = 0;
		R_Rotation = {0,0,0};
		L_Rotation = { 0,0,0 };
		R_Position.y = offSetY;
		m_steat = steat::close;
		SCX = WND_W / 2 - offSetX;
		UpS = 1;
		LPatternNo = { 0,0 };
		RPatternNo = { 1,0 };
	}
	switch (m_steat)
	{
	case GameStartIntro::steat::close:
		if (Timu > Waitingtime) {
			Timu++;
			return;
		}
		C += 90 / Closingtime;
		if (C >= 90) {
			C = 90;
			
		}
		L_Rotation.y = D3DXToRadian(90.0 * R(C / Closingtime));
		R_Rotation.y = D3DXToRadian(89.0 * R(C / Closingtime));
		if (C==90) {
			m_steat = steat::miseru;
			C = 0;
			RPatternNo = {1,1};
		}
		break;
	case GameStartIntro::steat::miseru:
		C++;
		L_Rotation.y = D3DXToRadian(90.0 + 90*(C / miserutime));
		R_Rotation.y = D3DXToRadian(90.0 - 90*(C / miserutime));
		L_Rotation.x = D3DXToRadian(  15 * (C / miserutime));
		R_Rotation.x = D3DXToRadian( -15 * (C / miserutime));
		SCX = (WND_W / 2 - offSetX)- ((WND_W / 2 - offSetX)*0.5) *(C / miserutime);
		if (C > miserutime) {
			C = miserutime;
			m_steat = steat::Up;
		}
		break;
	case GameStartIntro::steat::Up:
		UpS += 0.3;
		R_Position.y -= UpS;
		if (R_Position.y+ WND_H - offSetY * 2<0) {
			m_steat = steat::END;
		}
		break;
	case GameStartIntro::steat::END:
		break;
	default:
		break;
	}

}

void GameStartIntro::Draw()
{
	m_pKONDATE->SetScale({ SCX, WND_H - offSetY * 2,0.1 });

	switch (m_steat)
	{
	case GameStartIntro::steat::close:
		m_pKONDATE->SetPatternNo(LPatternNo.x, LPatternNo.y);
		m_pKONDATE->SetRotPot({ 0.5,0.5,0 });
		m_pKONDATE->SetPosition(L_Position);
		m_pKONDATE->SetRotation(L_Rotation);
		m_pKONDATE->Render();
		break;
	default:
		break;
	}

	m_pKONDATE->SetRotPot({-0.5,0.5,0});
	m_pKONDATE->SetPatternNo(RPatternNo.x, RPatternNo.y);
	m_pKONDATE->SetPosition(R_Position);
	m_pKONDATE->SetRotation(R_Rotation);
	m_pKONDATE->Render();
}
float GameStartIntro::R(float i)
{
	float in = std::clamp(i, 0.0f, 1.0f);
	float result = 0.0f;

	//std::pow や std::expでもいいしi*iでもいい
	// k は傾きの急鋭度（値を大きくするほど一気に上がる）
	const float k = 12.0f;
	// in = 0.0f のときに 0.0f、in = 1.0f のときに 1.0f になるよう正規化
	result = (std::exp(k * in) - 1.0f) / (std::exp(k) - 1.0f);

	//result = std::max(0.0f, result);
	//result = std::min(1.0f, result);
	return result;
}