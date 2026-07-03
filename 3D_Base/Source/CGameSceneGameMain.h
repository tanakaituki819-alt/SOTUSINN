#pragma once
#pragma once
#include"CGameScene.h"
#include"CGround.h"
#include"CPlayer.h"

class CGameSceneGameMain
	:public CGameScene
{
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

	CXInput* CONTROLA[4];

	CStaticMesh* m_pStaticMeshBSphere;	//バウンディングスフィア(当たり判定用).
	CGround* m_pGround;
	CPlayer* m_pPlayer;

};


