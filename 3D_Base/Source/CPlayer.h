#pragma once
#include "CStaticMeshObjObject.h"
class CPlayer
	:public  CStaticMeshObjObject
{
private:
	static constexpr float Speed = 0.1;//サイズ

public:
	CPlayer();
	~CPlayer();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
private:
	D3DXVECTOR3 m_Pos;
};
