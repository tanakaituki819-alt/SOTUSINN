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
	Water->SetCOLOR({140/256.0f,70/256.0f,20/256.0f });
	Water->SetisCOLOR(true);
	Water->SetScale({ m_Scale.x*2.2f,m_Scale .z*2.2f,m_Scale.y });
	static float i=0;
	i += 0.5;
	float MAX = 0.5;
	Water->Render2(View,Proj,{ sin(D3DXToRadian(i))/static_cast<float>(M_PI)* MAX,cos(D3DXToRadian(i)) / static_cast<float>(M_PI) * MAX });
	Water->SetisCOLOR(false);
	Water->SetAlpha(1);
}
