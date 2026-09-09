#pragma once
#include"GameScene/CGameScene.h"
#include"GameObject/UIObject/CUIObject.h";
#include "GameScene/GameSceneTitle/TitleUI/CTitleUI.h"
#include "System/XInput/CXInput.h"

class CGameSceneTitle
	:public CGameScene
{
public:
	CGameSceneTitle(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11,CCamera* m_Camera);
	~CGameSceneTitle()override;
	void Update()override;
	void Draw()override;

	void SetXInput(CXInput* Controller) { m_pController = Controller; 	m_pTitleUI->SetXInput(m_pController);
	m_pTitleUI->Init();
	}
private:
	CSprite2D* m_pTSprite;
	CUIObject* m_pTImg;

private:
	CTitleUI* m_pTitleUI;
	CXInput* m_pController;	//コントローラー.
};

