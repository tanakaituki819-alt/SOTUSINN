#pragma once

//食材クラス
#include "CStaticMeshObjObject.h"
#include "CNabe.h"
enum class Ingredients {
	none,
	NIKU,
	NINZIN,
	DAIKON,
	MAX
};


class CIngredients
	:public  CStaticMeshObjObject
{
private:
	struct IngredientsSetting
	{
		CStaticObjMesh* Mesh=nullptr;//つかうモデルの
		float HitSize=0;//当たり判定サイズ//０なら自動設定
		D3DXVECTOR3 Size = { 0,0,0 };//モデルサイズ
		int Score =0;//
	};
public:
	CIngredients();
	~CIngredients();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
	Ingredients GetIngredientsNo() const{ return IngredientsNo; };
	int GetScore() const{return Score;}
	void SetNabe(CNabe* CNabe) { Nabe = CNabe; }
	void SetIngredients(int i);
protected:
	D3DXVECTOR3 m_OldPosition;
	Ingredients IngredientsNo;
	int Score ;
	CNabe* Nabe;
	IngredientsSetting IngredientsSetting[static_cast<int>(Ingredients::MAX)];

};
