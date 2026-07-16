#include "CNabe.h"

CNabe::CNabe()
{
	m_Position = { 0,-0.1,0 };
	m_Scale = { NabeSize ,NabeSize/2,NabeSize };
	m_pMesh =CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::NABE);
	Water = CSpriteManager::GetSprite3D(CSpMLstiImg::Img_Water);
}

CNabe::~CNabe()
{

}

void CNabe::Update()
{


}

void CNabe::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
			
			CStaticMeshObjObject::Draw(View, Proj, Light, Camera);

}

void CNabe::DrawWater(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	Water->SetPosition({m_Position.x,m_Position.y+ NabeH,m_Position.z});
	Water->SetRotationX(D3DXToRadian(90));
	Water->SetAlpha(0.5);
	
	Water->SetScale({ m_Scale.x,m_Scale .z,m_Scale.y });
	
	Water->Render(View,Proj);
	Water->SetAlpha(1);
}
