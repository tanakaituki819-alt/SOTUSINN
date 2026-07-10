#include "CPlayer.h"

CPlayer::CPlayer()
{
	m_pMesh =CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::ROBO);
	m_Rotation.y = D3DXToRadian(90);
	m_Cousor = CSpriteManager::GetSprite3D(CSpriteManager::enImagList::Img_Cusoru);
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
		m_Position.z += Speed;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		m_Position.z -= Speed;
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
	//CStaticMeshObjObject::Draw(View, Proj, Light, Camera);
	m_Position.y = 1;
	m_Cousor->SetPosition(m_Position);
	m_Cousor->SetRotation({ D3DXToRadian(90),0,0});
	m_Cousor->SetScale(m_Scale);
	m_Cousor->Render(View, Proj);

}
