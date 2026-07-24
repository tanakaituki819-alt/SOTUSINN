#pragma once
#include "CGameScene.h"
#include "CUIObject.h"
#include "CCharacterUI.h"
#include "CPlayerSetupUI.h"
#include "CGameRdyUI.h"

/***************************************
*	コントローラー接続ゲームシーンクラス.
**/
class CGameScenePlayerSetup
	: public CGameScene		//ゲームシーンクラス継承.
{
public:
	//コントローラー最大数.
	static const int Controller_Max = 4;
public:
	CGameScenePlayerSetup(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameScenePlayerSetup()override;

	void Update()override;
	void Draw()override;


private:
	CUIObject*		m_pBackImg						= nullptr;
	CCharacterUI*	m_pCharacterUI					= nullptr;
	CPlayerSetupUI* m_pPlayerSetupUI				= nullptr;
	CGameRdyUI*		m_pGameRdyUI					= nullptr;
	CXInput*		m_pController[Controller_Max]   = {};
};

