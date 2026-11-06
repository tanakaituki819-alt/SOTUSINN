#include "GameStartIntro.h"

GameStartIntro::GameStartIntro()
{
	m_pKONDATE = CSpM::GetSprite2D(CSpMLstiImg::IMG_KONDATE);
	m_pKONDATE->SetScale({ SCX, WND_H - offSetY * 2,0.1 });
	OSAKINIITADAKIMASU= CSpM::GetSprite2D(CSpMLstiImg::IMG_OSAKIText);
	OSAKINIITADAKIMASU->SetPatternNo(0,0);
	OSAKINIITADAKIMASU->SetScale({0,0,0});
}

GameStartIntro::~GameStartIntro()
{
}

void GameStartIntro::Update()
{

	switch (m_steat)
	{
	case GameStartIntro::steat::close:
		if (Timu < Waitingtime) {
			Timu++;
			return;
		}
		C ++;
		if (C >= Closingtime) {
			C = Closingtime;
			
		}
		
		if (180 * R(C / Closingtime)>90) {
			L_Position = R_Position;
			L_Rotation.y = D3DXToRadian(180+180 * R(C / Closingtime));
			L_RotPot = { -0.5,0.5,0 };
			LPatternNo = { 0,1 };
		}
		else {
			L_Rotation.y = D3DXToRadian(180 * R(C / Closingtime));
		}
	
		if (1<= C / Closingtime) {
			m_steat = steat::miseru;
			C = 0;
			RPatternNo = {0,1};
		}
		break;
	case GameStartIntro::steat::miseru:
		C++;
		R_Position.x = WND_W / 2 - SCX/2.0 * (C / miserutime);
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
		End = true;
		REINVO+=0.02;
		OSAKINIITADAKIMASU_T += 1.0 / 180.0;
		if (OSAKINIITADAKIMASU_T>1) {
			m_steat = steat::AllEND;
		}

		break;
	default:
		break;
	}

}

void GameStartIntro::Draw()
{
	m_pKONDATE->SetScale({ SCX, WND_H - offSetY * 2,0.1 });

	m_pKONDATE->SetRotPot({-0.5,0.5,0});
	m_pKONDATE->SetPatternNo(RPatternNo.x, RPatternNo.y);
	m_pKONDATE->SetPosition(R_Position);
	m_pKONDATE->SetRotation(R_Rotation);
	m_pKONDATE->Render();
	switch (m_steat)
	{
	case GameStartIntro::steat::close:
		m_pKONDATE->SetPatternNo(LPatternNo.x, LPatternNo.y);
		m_pKONDATE->SetRotPot(L_RotPot);
		m_pKONDATE->SetPosition(L_Position);
		m_pKONDATE->SetRotation(L_Rotation);
		m_pKONDATE->Render();
		break;
		case  GameStartIntro::steat::END:
			float W =	1260 * Quadraticfunction(OSAKINIITADAKIMASU_T);
			float H= 560 * Quadraticfunction(OSAKINIITADAKIMASU_T);
			float t = std::min(Quadraticfunction(OSAKINIITADAKIMASU_T) / 0.8f, 1.0f);
			float eased = 1.0f - (1.0f - t) * (1.0f - t);   // 最初は速く、0.8 に近づくほどゆっくり
			OSAKINIITADAKIMASU->SetRotationZ(D3DXToRadian((360*4-35) * eased));
			OSAKINIITADAKIMASU->SetPosition({WND_W/2-W/2,WND_H/2-H/2,0});
			OSAKINIITADAKIMASU->SetScale({W,H,0});
			OSAKINIITADAKIMASU->Render3(REINVO);
			break;
	}



}
float GameStartIntro::R(float i)
{
	float in = std::clamp(i, 0.0f, 1.0f);
	float result = 0.0f;

	//std::pow や std::expでもいいしi*iでもいい
	// k は傾きの急鋭度（値を大きくするほど一気に上がる）
	const float k = 3.0f;
	// in = 0.0f のときに 0.0f、in = 1.0f のときに 1.0f になるよう正規化
	result = (std::exp(k * in) - 1.0f) / (std::exp(k) - 1.0f);

	//result = std::max(0.0f, result);
	//result = std::min(1.0f, result);
	return result;
}

float GameStartIntro::Quadraticfunction(float i)
{
	//ピークは0.5
	float result = 0;
	float w = 0.6;//幅
	float k = 4 / (w * w);

	result = -k * ((i - 0.5) * (i - 0.5)) + 1;
	// 例：最大輝度を 1.0 から 2.0（または 200%）に上げたい場合
	//result = (-4 * ((i - 0.5) * (i - 0.5)) + 1) * 2.0f;
	// 例：i = 0.8 のときに明るさのピークを持ってきたい場合
	//result = -4.0f * ((i - 0.8f) * (i - 0.8f)) + 1.0f
	result = std::max(0.0f, result);
	return result;
}