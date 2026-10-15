#pragma once
#include"IngredientsGetEffect.h"
class CPlayer;
class IngredientsGetEffectManager
{
public:
	IngredientsGetEffectManager();
	~IngredientsGetEffectManager();
	void Stato(CPlayer* Player, CIngredients* CIngredients, D3DXVECTOR3 toPosition);
	void Update();
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj);

private:
	std::vector<IngredientsGetEffect*> m_pIngredientsGetEffect;
};

