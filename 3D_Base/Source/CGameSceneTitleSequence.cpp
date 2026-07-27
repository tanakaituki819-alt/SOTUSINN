#include "CGameSceneTitleSequence.h"

CGameSceneTitleSequence::CGameSceneTitleSequence(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
{
	//カメラ座標.
	m_pTitleChopsticks = new CTitleChopsticks();
}

CGameSceneTitleSequence::~CGameSceneTitleSequence()
{
}

void CGameSceneTitleSequence::Update()
{
	m_pTitleChopsticks->Update();
}

void CGameSceneTitleSequence::Draw()
{
	m_pCamera->Cmera();
	Projection();
	m_pTitleChopsticks->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());
}
