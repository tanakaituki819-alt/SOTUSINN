#include "CGameSceneResult.h"

CGameSceneResult::CGameSceneResult(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	: CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pResultUI(nullptr)
{
	m_pResultUI = new CResultUI();
}

CGameSceneResult::~CGameSceneResult()
{
}

void CGameSceneResult::Update()
{
	m_pResultUI->Update();
}

void CGameSceneResult::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pResultUI->Draw();
	m_pDx11->SetDepth(true);	//深度テスト無効.
}
