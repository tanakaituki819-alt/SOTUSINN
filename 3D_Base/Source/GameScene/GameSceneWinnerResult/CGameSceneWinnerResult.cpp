#include "CGameSceneWinnerResult.h"
#include "Game/CGame.h"

CGameSceneWinnerResult::CGameSceneWinnerResult(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pWinnerResultUI(nullptr)
	, m_pController(nullptr)
{
	m_pController = new CXInput(0);
	m_pWinnerResultUI = new CWinnerResultUI();
	m_pWinnerResultUI->SetXInput(m_pController);	
}

CGameSceneWinnerResult::~CGameSceneWinnerResult()
{
	SAFE_DELETE(m_pWinnerResultUI);	
	SAFE_DELETE(m_pController);
}

void CGameSceneWinnerResult::Update()
{
	m_pController->Update();		

	m_pWinnerResultUI->Update();

	if (m_pWinnerResultUI->IsDecided())
	{
		//リザルトへ
		SenenChang(enScene::Result);
		m_pWinnerResultUI->Init();
	}
}

void CGameSceneWinnerResult::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pWinnerResultUI->Draw();
	m_pDx11->SetDepth(true);	//深度テスト無効.

}
