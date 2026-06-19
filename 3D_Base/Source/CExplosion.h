#pragma once
#include "CSpriteObject.h"

/**************************************************
*	爆発クラス.
**/
class CExplosion
	: public CSpriteObject	//スプライトオブジェクトクラスを継承.
{
public:
	CExplosion();
	virtual ~CExplosion() override;

	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj ) override;


protected:

};
