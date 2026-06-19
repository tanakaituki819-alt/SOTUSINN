#pragma once
#include "CStaticMeshObjObject.h"
class CGround
	:public  CStaticMeshObjObject
{
private:
	static constexpr float GroundSize=1;//サイズ
	static constexpr int GroundRENZOKUSUU = 10;//一片の数
public:
	CGround();
	~CGround();
	 void Update() override;
	 void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
private:
	D3DXVECTOR3 m_Pos;
};

