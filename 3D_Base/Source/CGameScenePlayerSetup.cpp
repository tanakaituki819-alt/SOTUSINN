#include "CGameScenePlayerSetup.h"
#include "CGame.h"

CGameScenePlayerSetup::CGameScenePlayerSetup(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pCharacterUI	( nullptr )
	, m_pPlayerSetupUI	( nullptr )
{
	m_pBackImg			= new CUIObject();
	m_pCharacterUI		= new CCharacterUI();
	m_pPlayerSetupUI	= new CPlayerSetupUI();
	m_pBackImg->AttachSprite(*CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround));	//和室背景設定.
	m_pBackImg->SetPosition	(0, 0, 0);
	m_pBackImg->SetScale	(WND_W, WND_H,0);
}

CGameScenePlayerSetup::~CGameScenePlayerSetup()
{
	SAFE_DELETE(m_pBackImg);
	SAFE_DELETE(m_pCharacterUI);
	SAFE_DELETE(m_pPlayerSetupUI);
}

void CGameScenePlayerSetup::Update()
{
}

void CGameScenePlayerSetup::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pBackImg->Draw();			//プレイヤーフォント描画.
	m_pPlayerSetupUI->Draw();	//プレイヤーセットアップシーン関係の描画.
	m_pCharacterUI->Draw();		//キャラクターUI描画.
	m_pDx11->SetDepth(true);	//深度テスト有効.
}
