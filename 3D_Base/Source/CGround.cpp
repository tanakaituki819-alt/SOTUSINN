#include "CGround.h"

CGround::CGround()
{
	m_Pos = { 0,-0.1,0 };
	m_Scale = { 1 ,1,1};
	m_pMesh =CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::NABE);
}

CGround::~CGround()
{

}

void CGround::Update()
{


}

void CGround::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
			m_Position = { m_Pos.x, m_Pos.y, m_Pos.z };
			CStaticMeshObjObject::Draw(View, Proj, Light, Camera);

}
