#pragma once
#include"GameScene/CGameScene.h"
#include "GameScene/GameSceneWinnerResult/WinnerResultUI/CWinnerResultUI.h"

class CGameSceneWinnerResult
	:public CGameScene
{
public:
	CGameSceneWinnerResult(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameSceneWinnerResult()override;

	void Update()override;
	void Draw()override;

private:
	CWinnerResultUI* m_pWinnerResultUI;
};
