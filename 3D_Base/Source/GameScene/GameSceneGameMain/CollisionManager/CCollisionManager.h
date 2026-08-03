#pragma once
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredientsmanager/CIngredientsmanager.h"

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


	//レイとメッシュの当たり判定.
	bool IsHitForRay(
		const RAY& pRay,				//レイ構造体.
		 CStaticMeshObjObject* obj,//objMesh
		float* pDistance,				//(out)距離.
		D3DXVECTOR3* pIntersect,		//(out)交差点.
		D3DXVECTOR3* pNormal = nullptr);//(out)法線（ベクトル）.

private:
	CPlayer*			 m_pPlayer[Player_Max];	//プレイヤー.
	CIngredientsmanager* m_pIngredientsManager;	//具材マネージャー.

	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVerticesPoly(
		LPD3DXMESH pMesh,
		DWORD dwPolyindex,
		D3DXVECTOR3* pVertices);	//(out)頂点情報.
};