#include "CGameScenePlayerSetup.h"
#include "CGame.h"

CGameScenePlayerSetup::CGameScenePlayerSetup(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pCharacterUI( nullptr )
{
	m_pCharacterUI = new CCharacterUI();
	m_pPfontImg = new CUIObject();
	m_pPfontImg->AttachSprite(*CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Playerfont));	//プレイヤーフォント設定.
}

CGameScenePlayerSetup::~CGameScenePlayerSetup()
{
	SAFE_DELETE(m_pPfontImg);
	SAFE_DELETE(m_pCharacterUI)
}

void CGameScenePlayerSetup::Update()
{

}

void CGameScenePlayerSetup::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pCharacterUI->Draw();
	m_pPfontImg->Draw();		//プレイヤーフォント描画.
	m_pDx11->SetDepth(true);	//深度テスト有効.
}
