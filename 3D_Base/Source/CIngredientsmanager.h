#pragma once
#include "CIngredients.h"
//食材のの作成、管理クラス
class CIngredientsmanager
{
private:

public:
	CIngredientsmanager();
	~CIngredientsmanager();
	void Update();
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera);

	void Create();
	void UpdateBSpherePos();
	void SetNabe(CNabe* Class) { Nabe = Class; };
	
	void AllIngredientsUpdate();
	std::vector<CIngredients*>& GetIngredients();
private:
	std::vector<CIngredients*> m_pIngredients;
	CNabe* Nabe;
};
