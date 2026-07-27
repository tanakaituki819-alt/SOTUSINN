#pragma once
#include"CGameScene.h"
#include"CUIObject.h";
#include "CTitleChopsticks.h"
class CGameSceneTitleSequence
	:public CGameScene
{
public:
	CGameSceneTitleSequence(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameSceneTitleSequence()override;
	void Update()override;
	void Draw()override;
private:
	CTitleChopsticks* m_pTitleChopsticks;
};

