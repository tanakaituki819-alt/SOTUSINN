#pragma once

#include "CStaticMeshObject.h"

/**************************************************
*	弾クラス.
**/
class CShot
	: public CStaticMeshObject
{
public:
	CShot();
	virtual ~CShot() override;

	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	//弾を再設定.
	void Reload(const D3DXVECTOR3& Pos);

	void SetDisplay( bool disp ) { m_Display = disp; }

protected:
	bool	m_Display;	//表示切替.
};