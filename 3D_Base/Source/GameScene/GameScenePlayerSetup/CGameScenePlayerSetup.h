#pragma once
#include <memory>
#include "GameScene/CGameScene.h"

class CGame;
class CXInput;
class CUIObject;
class CCharacterUI;
class CPlayerSetupUI;
class CGameRdyUI;

/***************************************
*	プレイヤーセットアップシーン.
*	コントローラーの接続と、各プレイヤーの準備完了を確認する.
**/
class CGameScenePlayerSetup
	: public CGameScene		//ゲームシーンクラス継承.
{
public:
	static const int Controller_Max = 4;	//コントローラー最大数.

public:
	CGameScenePlayerSetup(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera);
	~CGameScenePlayerSetup() override;

	void StartFinalSetup() override;
	void Update() override;
	void Draw() override;

	//ゲーム本体から、コントローラーとプレイヤーを受け取る.
	void PlayerControllerSet(CGame* pGame);

private:
	void ChangeToGameMain();			//ゲームメインへ切り替える.
	void ChangeToTitle();				//タイトルへ切り替える.
	void RemoveDisconnectedPlayers();	//未接続のプレイヤーを削除する(1Pは残す).

private:
	//このシーンが持ち主のUI.
	std::unique_ptr<CUIObject>		m_pBackImg;			//和室の背景.
	std::unique_ptr<CCharacterUI>	m_pCharacterUI;		//キャラクターの画像.
	std::unique_ptr<CPlayerSetupUI>	m_pPlayerSetupUI;	//背景・名前・コントローラーアイコン.
	std::unique_ptr<CGameRdyUI>		m_pGameRdyUI;		//準備完了とタイトルに戻る画面.

	//借りているだけ.
	CGame* m_pGame = nullptr;	//ゲーム本体.
	CXInput* m_pController[Controller_Max] = {};		//コントローラー.

	bool		m_IsSceneChanging = false;	//シーン切り替えを始めたか.
};