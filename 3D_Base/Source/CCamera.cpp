#include "CCamera.h"

CCamera::CCamera()
{
	m_Camera.Look = { 0,0,1, };
	m_Camera.Position = { 0,0,0 };
	Vec = { 0.0f, 1.0f, 0.0f };
}

CCamera::~CCamera()
{
}

void CCamera::Cmera()
{
	D3DXVECTOR3 cam_pos = m_Camera.Position;
	D3DXVECTOR3 cam_look = m_Camera.Look;
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);	//上方（ベクトル）.

	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&cam_pos, &cam_look, &vUpVec);
}

void CCamera::Update()
{
}

