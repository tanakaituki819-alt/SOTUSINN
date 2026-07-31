#include "CSpriteObject.h"

CSpriteObject::CSpriteObject()
	: m_pSprite		( nullptr )
	, m_PatternNo()
	, m_AnimCnt()
{
}

CSpriteObject::~CSpriteObject()
{
	DetachSprite();
}

void CSpriteObject::Update()
{
	if( m_pSprite == nullptr ){
		return;
	}
}

void CSpriteObject::Draw( D3DXMATRIX& View, D3DXMATRIX& Proj )
{
	if( m_pSprite == nullptr ){
		return;
	}

	//描画直前で座標や回転情報などを更新.
	m_pSprite->SetPosition( m_Position );
	m_pSprite->SetRotation( m_Rotation );
	m_pSprite->SetScale( m_Scale );

	//レンダリング.
	m_pSprite->Render( View, Proj );
}

void CSpriteObject::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	Draw( View, Proj );
}
