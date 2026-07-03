#pragma once
#include "CGameScene.h"
#include "CUIObject.h"
#include "CCharacterUI.h"
/***************************************
*	コントローラー接続ゲームシーンクラス.
**/
class CGameScenePlayerSetup
	: public CGameScene		//ゲームシーンクラス継承.
{
public:
	CGameScenePlayerSetup(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameScenePlayerSetup()override;

	void Update()override;
	void Draw()override;

private:
	CUIObject*		m_pBackImg;
	CCharacterUI*	m_pCharacterUI;
};

