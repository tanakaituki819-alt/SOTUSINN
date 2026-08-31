#pragma once

//食材クラス
#include "GameObject/StaticMeshObjObject/CStaticMeshObjObject.h"
#include "GameScene/GameSceneGameMain/Nabe/CNabe.h"
enum class Ingredients {
	none,
	NIKU,
	NINZIN,
	DAIKON,
	ENOKI,//エノキ
	HAKUSAI1,//白菜
	HAKUSEI2,//白菜
	KANI,//かに
	KUMANOTE,//クマの手
	NEGI,//ねぎ
	ROBUSTER,//ロブスター
	SAKANA,//リュウグウノツカイ
	SITAKE,//シイタケ
	TAI,//タイ
	TARA,//タラ
	TOUHU,//豆腐
	UINNER,//ウインナー
	MAX
};


class CIngredients
	:public  CStaticMeshObjObject
{
private:
	static constexpr float gravity = 0.006;//重力
	static constexpr float buoyancy = 0.006;//浮力
	struct IngredientsSetting
	{
		CStaticObjMesh* Mesh=nullptr;//つかうモデルの
		float HitSize=0;//当たり判定サイズ//０なら自動設定
		D3DXVECTOR3 Size = { 0,0,0 };//モデルサイズ
		D3DXVECTOR3 Rotation = { 0,0,0 };//モデルローテーション
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
	 float m_buoyancy;
	 bool isBoiled;//にえているか
	 float m_Boiledc;
	 float m_BoiledcMAX;
	 float Fallingforce;//落下力
};
