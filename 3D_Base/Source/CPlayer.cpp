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
	if (MyController->IsConnect()==true) {
		D3DXVECTOR2 VECT = {static_cast<FLOAT>( MyController->GetRThumbX()) ,static_cast<FLOAT>(MyController->GetRThumbY())};
		D3DXVec2Normalize(&VECT,&VECT);
		m_Position.x += VECT.x;
		m_Position.z += m_Position.y;
			
		MyController->IsDown(CXInput::A, true);


	}
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
	m_pMesh->SetisCOLOR(true);
	static float r, g, b = 0;
	r += rand() % 100 / 1000.f;
	g += rand() % 100 / 1000.f;
	b += rand() % 100 / 1000.f;
	if (r>1) {
		r--;
	}
	if (g>1) {
		g--;
	}
	if (b>1) {
		b--;
	}

	m_pMesh->SetLightCOLOR({r,g,b});
	CStaticMeshObjObject::Draw(View, Proj, Light, Camera);
}
