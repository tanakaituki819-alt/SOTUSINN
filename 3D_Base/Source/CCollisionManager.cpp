#include "CCollisionManager.h"
#include <iostream>

CCollisionManager::CCollisionManager()
	: m_pPlayer()
	, m_pIngredientsManager()
{
}

CCollisionManager::~CCollisionManager()
{

	SAFE_DELETE(m_pIngredientsManager);
}

void CCollisionManager::Update()
{
	for (int i = 0; i < Player_Max; i++) {
		//nullptrなら次の配列番号へ.
		if (!m_pPlayer[i]) continue;
		if (!m_pIngredientsManager)continue;
		//プレイヤーにコントローラー接続がされているなら.
		if (!m_pPlayer[i]->GetConnected()) continue;
		{
			//プレイヤーが回収中なら.
			if (!m_pPlayer[i]->GetIsCollecting()) continue;
			//具材のサイズ読み込み.
			std::vector<CIngredients*>Ingredients = m_pIngredientsManager->GetIngredients();
			for (auto& j: Ingredients) {
				//プレイヤーと具材が接触する.
				if (m_pPlayer[i]->GetBSphere()->IsHit(*j->GetBSphere())) {
					m_pPlayer[i]->OnTouchRawIngredient();	//マヒ状態にする.
				}
			}
		}

	}
}

