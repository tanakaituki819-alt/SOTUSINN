#pragma once
#include "CStaticMeshObjObject.h"
class CNabe
	:public  CStaticMeshObjObject
{
private:
	static constexpr float NabeSize=4;//サイズ

	static constexpr float NabeH = 0.5;
public:
	CNabe();
	~CNabe();
	 void Update() override;
	 void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
	 void DrawWater(D3DXMATRIX& View, D3DXMATRIX& Proj) ;
	 float GetSize() { return NabeSize; };
	 float GetNabeH() { return NabeH; };
private:

	CSprite3D*  Water;
};

