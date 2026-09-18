#pragma once
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredientsmanager/CIngredientsmanager.h"
#include "GameScene/GameSceneGameMain/ButtonMashBattleUI/CButtonMashBattleUI.h"

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
	//連打対決UIセット(具材の取り合いが発生した時に対決を開始させるため).
	void SetButtonMashBattleUI(CButtonMashBattleUI* pButtonMashBattleUI) {
		m_pButtonMashBattleUI = pButtonMashBattleUI;
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
	//具材に最初に触れてから、対決相手の参加を待つ猶予フレーム数(60fps換算で約0.25秒).
	//完全に同じフレームで触れていなくても、この時間内に触れれば連打対決に参加できる.
	static const int GRACE_FRAMES = 15;

	//取り合いの決着がまだ確定していない(猶予期間中の)具材の情報.
	struct PendingClaim
	{
		CIngredients* pTarget = nullptr;		//対象の具材(nullptrならこの枠は未使用).
		CPlayer* Touchers[Player_Max] = {};	//猶予期間中に触れたプレイヤー(重複無し).
		int TouchCount = 0;						//現在集まっている人数.
		int GraceTimer = 0;						//残り猶予フレーム数.
	};

	//同時に猶予期間中にできる具材の数(最大でもプレイヤー数程度しか同時発生しないためPlayer_Max分確保).
	PendingClaim m_PendingClaims[Player_Max];

	//指定した具材の猶予枠を探す。無ければnullptr.
	PendingClaim* FindPendingClaim(CIngredients* pTarget);
	//空いている猶予枠を探す。無ければnullptr.
	PendingClaim* FindFreePendingClaimSlot();
	//猶予枠を確定させる(1人なら通常獲得、2人以上なら連打対決を開始する).
	void ResolvePendingClaim(PendingClaim& Claim);

	CPlayer*			 m_pPlayer[Player_Max];	//プレイヤー.
	CIngredientsmanager* m_pIngredientsManager;	//具材マネージャー.
	CButtonMashBattleUI* m_pButtonMashBattleUI = nullptr;	//連打対決UI(具材の取り合いが発生した時に使用).

	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVerticesPoly(
		LPD3DXMESH pMesh,
		DWORD dwPolyindex,
		D3DXVECTOR3* pVertices);	//(out)頂点情報.
};