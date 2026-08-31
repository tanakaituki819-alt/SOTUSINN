#pragma once
#include"GameScene/CGameScene.h"
#include "GameScene/GameSceneResult/ResultUI/CResultUI.h"

class CGameSceneResult
	:public CGameScene
{
public:
	CGameSceneResult(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameSceneResult()override;

	void Update()override;
	void Draw()override;

private:
	CResultUI* m_pResultUI;
};
