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
		D3DXVECTOR2 VECT = {static_cast<FLOAT>( MyController->GetLThumbX()) ,static_cast<FLOAT>(MyController->GetLThumbY())};
		D3DXVec2Normalize(&VECT,&VECT);
		VECT *= Speed;
		m_Position.x += VECT.x;
		m_Position.z += VECT.y;
			
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
	
	//重なって見えなくならないようにずらす
	m_Position.y = 1+ MyController->GetPadID()*0.1;

	m_Cousor->SetPosition(m_Position);
	m_Cousor->SetRotation({ D3DXToRadian(90),0,0});
	m_Cousor->SetScale(m_Scale);
	m_Cousor->Render(View, Proj);

}
