#pragma once
#pragma once
#include"CGameScene.h"
#include"CGround.h"
#include"CPlayer.h"
#include"CIngredients.h"
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
	CGround* m_pGround;
	CPlayer* m_pPlayer[PlayerMax];

	CIngredients* m_pCing;
};


