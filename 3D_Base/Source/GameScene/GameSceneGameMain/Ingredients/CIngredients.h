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
enum class enCharStatus {
	Standby,
	Live,
	Dying,
	Dead,
};

struct IngredientsSetting
{
	CStaticObjMesh* Mesh = nullptr;//つかうモデルの
	float probability = 1.0f;//出現確率
	float HitSize = 0;//当たり判定サイズ//０なら自動設定
	D3DXVECTOR3 Size = { 0,0,0 };//モデルサイズ
	D3DXVECTOR3 Rotation = { 0,0,0 };//モデルローテーション

	int Score = 0;//手に入れたときに入るスコア
	float Cost = 1.0f;//残っている食材のコストの合計が３５（仮）を超えるなら食材がわかない
	float BoiledcTimu = 15;//食材が煮えるまでの時間(ランダムで少し増減する)
};

class CIngredients
	:public  CStaticMeshObjObject
{
private:
	static constexpr float gravity = 0.006;//重力
	static constexpr float buoyancy = 0.006;//浮力

public:
	CIngredients();
	~CIngredients();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
	Ingredients GetIngredientsNo() const{ return IngredientsNo; };
	int GetScore() const{return Score;}
	bool GetBoiledc() { return isBoiled; }
	bool GetCollecting() { return m_IsCollecting; }
	void SetNabe(CNabe* CNabe) { Nabe = CNabe; }
	void SetIngredients(IngredientsSetting* pIngredientsSetting,int i);//野菜の設定
	void IsCollecting();
	enCharStatus GetCharStatus() {return Status;}
	void DeadCharStatus() { Status = enCharStatus::Dead; }
	//重複して取ったら
	void m_IsCollectingEnd() { m_IsCollecting = false; }

	float GetCost() {return Cost;};
protected:
	D3DXVECTOR3 m_OldPosition;
	Ingredients IngredientsNo;
	int Score ;
	CNabe* Nabe;

	 float m_buoyancy;
	 bool isBoiled;//にえているか
	 float m_Boiledc;
	 float m_BoiledcMAX;
	 float Fallingforce;//落下力
	 bool	m_IsCollecting = false;	//回収中.
	 float Cost;//食材のコスト
	 bool IsInTheWater;//水に入っているか
	 int EffC;
	 int EffC1;
	 enCharStatus Status;
};
