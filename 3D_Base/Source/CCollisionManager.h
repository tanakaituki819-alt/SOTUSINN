#pragma once
#include "CPlayer.h"
#include "CIngredientsmanager.h"

/******************************************
*	当たり判定クラス.
***/
class CCollisionManager
{
public:
	/*******************************
	*	定数宣言.
	**/
	static const int Player_Max = 4;	//プレイヤー最大数.

public:
	CCollisionManager();
	~CCollisionManager();
	//プレイヤーセット.
	void SetPlyaer(CPlayer& Player,int i) { m_pPlayer[i] = &Player; }
	//具材マネージャーセット.
	void SetIngredients(CIngredientsmanager& Ingredients) {
		m_pIngredientsManager = &Ingredients;
	}

	void Update();

private:
	CPlayer*			 m_pPlayer[Player_Max];	//プレイヤー.
	CIngredientsmanager* m_pIngredientsManager;	//具材マネージャー.
};