#pragma once

//食材クラス
#include "CStaticMeshObjObject.h"
enum class Ingredients {
	none,
	Onion,
};


class CIngredients
	:public  CStaticMeshObjObject
{
public:
	CIngredients();
	~CIngredients();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
protected:
	Ingredients IngredientsNo;


};
