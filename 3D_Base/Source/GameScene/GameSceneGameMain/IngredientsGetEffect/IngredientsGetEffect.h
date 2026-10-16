#pragma once
#include"GameScene\GameSceneGameMain\Ingredients\CIngredients.h"
class CPlayer;
class IngredientsGetEffect
{
private:
	static constexpr float Timu = 1*60;//到着までの時間
public:
	IngredientsGetEffect();
	~IngredientsGetEffect();
	void Stato(CPlayer* Player, CIngredients* CIngredients, D3DXVECTOR3 toPosition);
	void Update();
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) ;
	bool GetisUsed() {return isUsed;
	}
private:

	D3DXVECTOR3 Position = { 0,0,0 };
	CPlayer* m_Player = nullptr;
	CIngredients* m_pIngredients = nullptr;
	D3DXVECTOR3 ToPosition;
	D3DXVECTOR3 StartPosition;
	bool isUsed = false;
	int m_Timu;
};

