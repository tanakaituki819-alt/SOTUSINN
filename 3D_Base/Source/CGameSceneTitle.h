#pragma once
#include"CGameScene.h"
#include"CUIObject.h";
class CGameSceneTitle
	:public CGameScene
{
public:
	CGameSceneTitle(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11,CCamera* m_Camera);
	~CGameSceneTitle()override;
	void Update()override;
	void Draw()override;
private:
	CSprite2D* m_pTSprite;
	CUIObject* m_pTImg;
};

