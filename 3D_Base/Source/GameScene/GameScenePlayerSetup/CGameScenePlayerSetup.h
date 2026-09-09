#pragma once
#include "GameScene/CGameScene.h"
#include "GameObject/UIObject/CUIObject.h"

#include "CharacterUI/CCharacterUI.h"
#include "PlayerSetupUI/CPlayerSetupUi.h"
#include "GameRdyUI/CGameRdyUI.h"

/***************************************
*	コントローラー接続ゲームシーンクラス.
**/
class CGame;
class CPlayer;
class CGameScenePlayerSetup
	: public CGameScene		//ゲームシーンクラス継承.
{
public:
	//コントローラー最大数.
	static const int Controller_Max = 4;
public:
	CGameScenePlayerSetup(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameScenePlayerSetup()override;
	void StartFinalSetup()override;
	void Update()override;
	void Draw()override;

	void PlayerControllerSet(CXInput**Xinput, CPlayer**player);
	void PlayerControllerSet(CGame*m_pCGame);
private:
	CUIObject*		m_pBackImg						= nullptr;
	CCharacterUI*	m_pCharacterUI					= nullptr;
	CPlayerSetupUI* m_pPlayerSetupUI				= nullptr;
	CGameRdyUI*		m_pGameRdyUI					= nullptr;
	CXInput* m_pController[Controller_Max]			= {};
	CPlayer* m_pPlayer[Controller_Max]				= {};
	CGame* CG;
};

