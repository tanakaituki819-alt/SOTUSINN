#include "CGameSceneResult.h"


CGameSceneResult::CGameSceneResult(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	: CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pResultUI(nullptr)
	, m_pController(nullptr)
{
	m_pController = new CXInput(0);
	m_pResultUI = new CResultUI();
	m_pResultUI->SetXInput(m_pController);
}

CGameSceneResult::~CGameSceneResult()
{
	SAFE_DELETE(m_pResultUI);
	SAFE_DELETE(m_pController);
}

void CGameSceneResult::Update()
{
	m_pController->Update();
	m_pResultUI->Update();

	if (m_pResultUI->IsADecided())
	{
		SenenChang(enScene::GameMain);
		m_pResultUI->BoolInit();
	}
	else if (m_pResultUI->IsBDecided())
	{
		SenenChang(enScene::PlayerSetUp);
		m_pResultUI->BoolInit();
	}

}

void CGameSceneResult::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pResultUI->Draw();
	m_pDx11->SetDepth(true);	//深度テスト無効.
}
