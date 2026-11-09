#include "CGameSceneTitle.h"
#include "Game/CGame.h"


CGameSceneTitle::CGameSceneTitle(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd,Dx9,Dx11, m_Camera)
	, m_pTitleUI(nullptr)
	, m_pController(nullptr)
{
	//BGM再生.
	SoundHandleX BGMHandle = XAudio2::GetInstance()->BGMPlayHandle(XAudio2::BGMList::Title);
	XAudio2::GetInstance()->SetSoundVolume(BGMHandle, 1.0f);
	m_pTSprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TITLE);

	m_pTitleUI = new CTitleUI();
}

CGameSceneTitle::~CGameSceneTitle()
{

	SAFE_DELETE(m_pTitleUI);

}
void CGameSceneTitle::StartFinalSetup()
{




	m_pTitleUI->SetXInput(m_pController);
	m_pTitleUI->Init();

	m_pTitleUI->SetXInput(m_pController);
	m_pTitleUI->Init();
}
void CGameSceneTitle::Update()
{



	m_pController->Update();
	m_pTitleUI->Update();

	//遷移
	if (m_pTitleUI->IsDecided())
	{
		if (m_pTitleUI->GetSelect() == CTitleUI::enSelect::Start)
		{
			SenenChang(enScene::PlayerSetUp, CSceneChange::TransitionType::Fade, 60, 60);
			XAudio2::GetInstance()->AllStopBGM();
			m_pTitleUI->Init();
		}
		else if (m_pTitleUI->GetSelect() == CTitleUI::enSelect::Fin)
		{
			DestroyWindow(m_hWnd);

		}
	}
}

void CGameSceneTitle::Draw()
{
	m_pDx11->SetDepth(false);
	Projection();



	m_pTitleUI->Draw();

	Effect2D::GetInstance()->Draw();

	m_pDx11->SetDepth(true);



}


