#pragma once
#pragma once
#include"GameScene/CGameScene.h"
#include"Player/CPlayer.h"
#include"GameScene/GameSceneGameMain/Ingredients/CIngredients.h"
#include "GameScene/GameSceneGameMain/PauseUI/CPauseUI.h"
#include "GameScene/GameSceneGameMain/CollisionManager/CCollisionManager.h"
#include"GameScene/GameSceneGameMain/Timer/CTimer.h"

#include "GameScene/GameSceneGameMain/Ingredientsmanager/CIngredientsmanager.h"
#include "GameScene/GameSceneGameMain/Nabe/CNabe.h"
class CGameSceneGameMain
	:public CGameScene
{
private:
	static constexpr int PlayerMax = 4;;
public:
	CGameSceneGameMain(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameSceneGameMain()override;
	void Update()override;
	void Draw()override;
private:
	//当たり判定の更新
	void UpdateBSpherePos();
	//当たり判定確認
	void CheckCollision();

	CXInput* CONTROLA[PlayerMax];

	CStaticMesh* m_pStaticMeshBSphere;	//バウンディングスフィア(当たり判定用).
	CNabe* m_pGround;
	CPlayer* m_pPlayer[PlayerMax];

	CIngredients* m_pCing;

	CTimer* m_pTimer;

	//当たり判定クラス.
	CCollisionManager* m_pCollisionManager;

	//ポーズ関連.
	CPauseUI* m_pPauseUI;
	bool m_Pause;
	CSprite2D* m_pBackGround;
	CIngredientsmanager* m_pCingM;
};


