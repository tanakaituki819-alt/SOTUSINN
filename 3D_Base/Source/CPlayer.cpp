#include "CPlayer.h"

CPlayer::CPlayer()
{
	m_pMesh =CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::ROBO);
	m_Rotation.y = D3DXToRadian(90);
}

CPlayer::~CPlayer()
{

}

void CPlayer::Update()
{
	if (GetAsyncKeyState('W') & 0x8000) {
		m_Position.y += Speed;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		m_Position.y -= Speed;
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		m_Position.x -= Speed;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		m_Position.x += Speed;
	}
}

void CPlayer::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObjObject::Draw(View, Proj, Light, Camera);
}
