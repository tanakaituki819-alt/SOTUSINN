#include "CTitleChopsticks.h"

CTitleChopsticks::CTitleChopsticks()
{
	for (int i = 0; i < Chopsticks_Max; i++) {
		m_pChopsticks[i] = new CStaticMeshObjObject();
		m_pChopsticks[i]->AttachMesh(*CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::Chopsticks2));	//お箸(メッシュ).
	}
	m_Position = { 0,0,0 };
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
	if (m_AttackMoveNow) {
		m_Move += 0.05f;
	}
	else {
		m_Move -= 0.05f;
	}
	if (m_Move > 1.6f) {
		m_AttackMoveNow = false;
	}

	
		//if (GetAsyncKeyState('W') & 0x8000) {
		//	m_Position.y += 0.1f;
		//}
		//if (GetAsyncKeyState('S') & 0x8000) {
		//	m_Position.y -= 0.1f;
		//}
		//if (GetAsyncKeyState('A') & 0x8000) {
		//	m_Position.x -= 0.1f;
		//}
		//if (GetAsyncKeyState('D') & 0x8000) {
		//	m_Position.x += 0.1f;
		//}
		//if (GetAsyncKeyState('Q') & 0x8000) {
		//	m_Position.z -= 0.1f;
		//}
		//if (GetAsyncKeyState('E') & 0x8000) {
		//	m_Position.z += 0.1f;
		//}

}
void CTitleChopsticks::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//[0]右上.
	m_pChopsticks[0]->SetPosition(m_PosX + - m_Move, m_PosY + - (m_Move / 1.5f), m_PosZ);
	m_pChopsticks[0]->SetRotation(m_Pitch, m_Yaw, m_Roll);

	//[1]左上.
	m_pChopsticks[1]->SetPosition(-m_PosX, m_PosY, m_PosZ);
	m_pChopsticks[1]->SetRotation(m_Pitch, -m_Yaw, -m_Roll);

	//[2]右下. 
	m_pChopsticks[2]->SetPosition(m_PosX, -m_PosY, m_PosZ);
	m_pChopsticks[2]->SetRotation(-m_Pitch, m_Yaw, -m_Roll);

	//[3]左下. 
	m_pChopsticks[3]->SetPosition(-m_PosX + m_Move, -m_PosY + (m_Move / 1.5f), m_PosZ);
	m_pChopsticks[3]->SetRotation(-m_Pitch, -m_Yaw, m_Roll);

	//描画処理.
	for (int i = 0; i < Chopsticks_Max; i++) {
		m_pChopsticks[i]->SetScale(2.f, 2.f, 2.f);
		m_pChopsticks[i]->Draw(View, Proj, Light, Camera);
	}
}
