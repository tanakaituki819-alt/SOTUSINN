#include "CGround.h"

CGround::CGround()
{
	m_Pos = { 0,-0.1,0 };
	m_Scale = { GroundSize+1 ,0.1f,GroundSize+1 };
	m_pMesh =CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::Grassland);
}

CGround::~CGround()
{

}

void CGround::Update()
{
	m_Pos.z -= 0.1f;
	if (-GroundSize > m_Pos.z) {
		m_Pos.z = GroundSize;
	}

}

void CGround::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	for (int x = -GroundRENZOKUSUU / 2; x < GroundRENZOKUSUU / 2;x++)
	{
		for (int z = -GroundRENZOKUSUU / 2; z < GroundRENZOKUSUU / 2;z++)
		{

			m_Position = { m_Pos.x+static_cast<float> (GroundSize) * static_cast<float>(x), m_Pos.y, m_Pos.z+ static_cast<float>(GroundSize) * static_cast<float>(z) };
			CStaticMeshObjObject::Draw(View, Proj, Light, Camera);
		}

	}
}
