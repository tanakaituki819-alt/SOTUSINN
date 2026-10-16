#include "IngredientsGetEffect.h"
#include <Player/CPlayer.h>
IngredientsGetEffect::IngredientsGetEffect()
{
}

IngredientsGetEffect::~IngredientsGetEffect()
{
}

void IngredientsGetEffect::Stato(CPlayer* Player, CIngredients* CIngredients, D3DXVECTOR3 toPosition)
{
	m_Player = Player;
	m_pIngredients = CIngredients;
	ToPosition = toPosition;
	isUsed = true;
	StartPosition.x = -1000;//おそらく入らない数字を入れる
	
}

void IngredientsGetEffect::Update()
{
	if (isUsed) {
		m_Timu++;
		if (m_Timu> Timu) {
			m_Player->GetIngredients(m_pIngredients);
			m_pIngredients = nullptr;
			m_Player = nullptr;
			isUsed = false;
			m_Timu = 0;
			EsHandle H = Effect2D::GetInstance()->Play(Effect2D::KANSEI, Position);
		
			Effect2D::GetInstance()->SetScale(H, { 15,15,15 });
		}
	}
}

void IngredientsGetEffect::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	if (!m_pIngredients&& !m_Player) {
		return;
	}
	if (StartPosition.x == -1000) {
		StartPosition = GetPosition2D(View, Proj, m_pIngredients->GetPosition());
	
	}
	float t = m_Timu / Timu;     // 0.0 → 1.0

	// X は等速
	Position.x = StartPosition.x + (ToPosition.x - StartPosition.x) * t;
	// （始点が放物線の頂点）
	static constexpr float Curve = 1.5f;   // 1.0=直線 ～ 2.0=放物線
	Position.y = StartPosition.y + (ToPosition.y - StartPosition.y) * powf(t, Curve);

	if (m_Timu%5==0) {
		float Size = 60;
		EsHandle H= Effect2D::GetInstance()->Play(Effect2D::PointGetEfect,Position);
		float EFSize = Size - Size * powf(t, 1.2);
		Effect2D::GetInstance()->SetScale(H,{ EFSize,EFSize,EFSize });
	}

}
