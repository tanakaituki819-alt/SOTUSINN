#include "CGameSceneWinnerResult.h"
#include "CGame.h"

CGameSceneWinnerResult::CGameSceneWinnerResult(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pWinnerResultUI(nullptr)
{
	m_pWinnerResultUI = new CWinnerResultUI();
}

CGameSceneWinnerResult::~CGameSceneWinnerResult()
{
}

void CGameSceneWinnerResult::Update()
{
	m_pWinnerResultUI->Update();
}

void CGameSceneWinnerResult::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pWinnerResultUI->Draw();
	m_pDx11->SetDepth(true);	//深度テスト無効.

}
