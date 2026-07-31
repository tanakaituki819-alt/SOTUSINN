#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh		( nullptr )
	, m_pBSphere	( nullptr )
{
	m_pBSphere = new CBoundingSphere();
}

CStaticMeshObject::~CStaticMeshObject()
{
	SAFE_DELETE( m_pBSphere );
	DetachMesh();
}

void CStaticMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if( m_pMesh == nullptr ){
		return;
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition( m_Position );
	m_pMesh->SetRotation( m_Rotation );
	m_pMesh->SetScale( m_Scale );

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.Position );
}