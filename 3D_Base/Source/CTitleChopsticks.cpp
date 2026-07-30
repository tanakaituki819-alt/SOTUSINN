#include "CTitleChopsticks.h"

namespace
{
	constexpr float CHOPSTICKS_DEFAULT_SCALE	= 2.0f;		//お箸メッシュの初期スケール.
	constexpr float INITIAL_SPEED_X				= 0.08f;	//X軸の初期移動速度.
	constexpr float SPEED_Y_RATIO				= 1.5f;		//Y軸速度の計算比率 (SpeedX / 1.5).
	constexpr float INITIAL_SPEED_Y				= INITIAL_SPEED_X / SPEED_Y_RATIO;	//Y座標の初期移動速度.
	constexpr float REFLECT_BOUNCE_DECAY		= 1.5f;		//Caseでの跳ね返り減衰率.
	constexpr float IMPACT_THRESHOLD			= 1.65f;	//衝突判定位置 (X座標).
	constexpr float NEXT_PHASE_LIMIT			= -3.0f;	//次のフェーズへ移動する戻り限界値 (X座標).
}
CTitleChopsticks::CTitleChopsticks()
{
	for (int i = 0; i < Chopsticks_Max; i++) {
		m_pChopsticks[i] = new CStaticMeshObjObject();
		m_pChopsticks[i]->AttachMesh(*CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::Chopsticks2));	//お箸(メッシュ).
		m_pChopsticks[i]->SetScale(CHOPSTICKS_DEFAULT_SCALE, CHOPSTICKS_DEFAULT_SCALE, CHOPSTICKS_DEFAULT_SCALE);
	}
	//角度調整.
	m_pChopsticks[0]->SetRotation(m_Pitch, m_Yaw, m_Roll);
	m_pChopsticks[1]->SetRotation(m_Pitch, -m_Yaw, -m_Roll);
	m_pChopsticks[2]->SetRotation(-m_Pitch, m_Yaw, -m_Roll);
	m_pChopsticks[3]->SetRotation(-m_Pitch, -m_Yaw, m_Roll);
}

CTitleChopsticks::~CTitleChopsticks()
{
	//お箸最大数分.
	for (int i = 0; i < Chopsticks_Max; i++) {
		SAFE_DELETE(m_pChopsticks[i]);
	}
}

void CTitleChopsticks::Update()
{
	//デバッグ用.
	if (GetAsyncKeyState('Z') & 0x8000) {
		SetCaseId(1);
	}
	//ケースで移動処理の変更.
	switch (m_CaseId)
	{
	case 1:
		//ポジション設定.
		m_pChopsticks[0]->SetPosition(m_PosX + -m_MoveX, m_PosY + -m_MoveY, m_PosZ);
		m_pChopsticks[3]->SetPosition(-m_PosX + m_MoveX, -m_PosY + m_MoveY, m_PosZ);
		//移動させる.
		m_MoveX += m_SpeedX;
		m_MoveY += m_SpeedY;

		//中央付近で衝突(一致値を超えたらX座標を反転させる.
		if (m_AttackMoveNow && m_MoveX > IMPACT_THRESHOLD) {
			m_AttackMoveNow = false;				//反転.
			m_SpeedX = -m_SpeedX / SPEED_Y_RATIO;	//X軸の移動方向だけ反転.
		}
		//端まで戻ったら次のフェーズへ.
		if (!m_AttackMoveNow && m_MoveX < NEXT_PHASE_LIMIT) {
			SetCaseId(2);
		}
		break;

	case 2:
		//ポジション設定.
		m_pChopsticks[1]->SetPosition(-m_PosX - -m_MoveX, m_PosY - m_MoveY, m_PosZ);
		m_pChopsticks[2]->SetPosition(m_PosX + -m_MoveX, -m_PosY + m_MoveY, m_PosZ);
		//移動させる.
		m_MoveX += m_SpeedX;
		m_MoveY += m_SpeedY;

		//中央付近で衝突(一致値を超えたらX座標を反転させる.
		if (m_AttackMoveNow && m_MoveX > IMPACT_THRESHOLD) {
			m_AttackMoveNow = false;				//反転.
			m_SpeedX = -m_SpeedX / SPEED_Y_RATIO;	//X軸の移動方向だけ反転.
		}
		// 端まで戻ったら次のフェーズへ
		if (!m_AttackMoveNow && m_MoveX < NEXT_PHASE_LIMIT) {
			SetCaseId(3);
		}
		break;
	case 3:
		//ポジション設定.
		m_pChopsticks[0]->SetPosition(m_PosX + -m_MoveX, m_PosY + -m_MoveY, m_PosZ);
		m_pChopsticks[3]->SetPosition(-m_PosX + m_MoveX, -m_PosY + m_MoveY, m_PosZ);
		m_pChopsticks[1]->SetPosition(-m_PosX - (-m_MoveX), m_PosY - m_MoveY, m_PosZ);
		m_pChopsticks[2]->SetPosition(m_PosX + (-m_MoveX), -m_PosY + m_MoveY, m_PosZ);
		//移動させる.
		m_MoveX += m_SpeedX;
		m_MoveY += m_SpeedY;

		//中央付近で衝突(一致値を超えたらX座標を反転させる.
		if (m_AttackMoveNow && m_MoveX > IMPACT_THRESHOLD) {
			m_AttackMoveNow = false;				//反転.
			m_SpeedX = -m_SpeedX;					//X軸の移動方向だけ反転.
		}
		break;
	}
}
void CTitleChopsticks::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	switch (m_CaseId) {
	case 1:
		//[0]右上.
		m_pChopsticks[0]->Draw(View, Proj, Light, Camera);
		//[3]左下. 
		m_pChopsticks[3]->Draw(View, Proj, Light, Camera);
		break;
	case 2:
		//[1]左上.
		m_pChopsticks[1]->Draw(View, Proj, Light, Camera);
		//[2]右下. 
		m_pChopsticks[2]->Draw(View, Proj, Light, Camera);
		break;
	case 3:		
		//描画処理.
		for (int i = 0; i < Chopsticks_Max; i++) {
			m_pChopsticks[i]->Draw(View, Proj, Light, Camera);
		}
		break;
	}
}
// SetCaseId関数で初期速度をセットする
void CTitleChopsticks::SetCaseId(int nextCaseId)
{
	if (m_CaseId == nextCaseId) return;

	m_CaseId = nextCaseId;

	m_AttackMoveNow = true;

	//ケースごとの初期位置・速度リセット.
	m_MoveX = 0.0f;	
	m_MoveY = 0.0f;
	m_SpeedX = INITIAL_SPEED_X;			//X方向の速度.
	m_SpeedY = INITIAL_SPEED_Y;			//Y方向の速度.
}
