#pragma once
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"

#include <random>//偏りのある乱数を作るのに必要


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
	int GetIngredientsliveing();//野菜の生存数を返す

private:
	IngredientsSetting IngredientsSetting[static_cast<int>(Ingredients::MAX)];//野菜の設定
	std::vector<CIngredients*> m_pIngredients;

	CNabe* Nabe;

	std::mt19937 gen;
	std::discrete_distribution<int> Normaldist;
	std::discrete_distribution<int> Raredist;

	std::discrete_distribution<int> Gacha;
	float TimuC;
	bool StartSettingis;//開始時の配置中
	float IngredientsSettingTimu;
};
