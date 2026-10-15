#include "IngredientsGetEffectManager.h"

IngredientsGetEffectManager::IngredientsGetEffectManager()
{
}

IngredientsGetEffectManager::~IngredientsGetEffectManager()
{
	for (int i = 0; i < m_pIngredientsGetEffect.size(); i++) {
		SAFE_DELETE(m_pIngredientsGetEffect[i]);
	}
	m_pIngredientsGetEffect.clear();

}

void IngredientsGetEffectManager::Stato(CPlayer* Player, CIngredients* CIngredient, D3DXVECTOR3 toPosition)
{
	//再利用可能なら再利用
	for (int i = 0; i < m_pIngredientsGetEffect.size(); i++) {
		if (m_pIngredientsGetEffect[i]->GetisUsed() == false) {
			m_pIngredientsGetEffect[i]->Stato(Player, CIngredient, toPosition);
			return;
		}
	}
	IngredientsGetEffect* now = new IngredientsGetEffect;




	now->Stato(Player, CIngredient, toPosition);
	m_pIngredientsGetEffect.push_back(now);

}

void IngredientsGetEffectManager::Update()
{
	for (int i = 0; i < m_pIngredientsGetEffect.size(); i++) {
		m_pIngredientsGetEffect[i]->Update();
	}
}

void IngredientsGetEffectManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (int i = 0; i < m_pIngredientsGetEffect.size(); i++) {
		m_pIngredientsGetEffect[i]->Draw(View, Proj);
	}
}
