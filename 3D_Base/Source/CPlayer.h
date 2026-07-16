#pragma once

#include "CStaticMeshObjObject.h"
#include <list>
#include "CIngredients.h"
class CPlayer
	:public  CStaticMeshObjObject
{
private:
	static constexpr float Speed = 0.1;//スピード

public:
	CPlayer();
	~CPlayer();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
	void SetXInput( CXInput* Input) { MyController = Input; }

	void GetIngredients(CIngredients*  YASAI);
	std::list<Ingredients>& GetAllIngredients() {return my_list;}
private:
	D3DXVECTOR3 m_Pos;
	CXInput* MyController;
	CSprite3D* m_Cousor;
	std::list<Ingredients> my_list;
	int Score;
};
