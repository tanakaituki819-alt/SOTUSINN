#pragma once
#pragma once
#include"CGameScene.h"
#include"CPlayer.h"
#include"CIngredients.h"
#include "CPauseUI.h"
#include "CCollisionManager.h"
#include"CTimer.h"

#include "CIngredientsmanager.h"
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
	CIngredientsmanager* m_pCingM;
};


