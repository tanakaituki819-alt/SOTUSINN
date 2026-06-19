#include "CShot.h"

CShot::CShot()
	: m_Display		( false )
{
}

CShot::~CShot()
{
}

void CShot::Update()
{
	if (m_Display == true) {
		m_Position.z += 0.1f;
		if (m_Position.z >= 30.f) {
			//見えない所に置いておく.
			m_Position = D3DXVECTOR3(0.f, -10.f, 0.f);
			m_Display = false;
		}
	}
}

void CShot::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (m_Display == true) {
		CStaticMeshObject::Draw(View, Proj, Light, Camera);
	}
}

void CShot::Reload(const D3DXVECTOR3& Pos)
{
	if (m_Display == false) {
		m_Position = Pos;
		m_Display = true;
	}
}
