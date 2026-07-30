#include "CGameSceneTitle.h"
#include "CGame.h"


CGameSceneTitle::CGameSceneTitle(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd,Dx9,Dx11, m_Camera)
	, m_pTitleUI(nullptr)
	, m_pController(nullptr)
{
	m_pTSprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TITLE);

	m_pTImg = new CUIObject();
	m_pTImg->SetScale(WND_W, WND_H, 0);
	m_pTImg->AttachSprite(*m_pTSprite);

	m_pController = new CXInput(0);

	m_pTitleUI = new CTitleUI();
	m_pTitleUI->SetXInput(m_pController);
	m_pTitleUI->Init();
}

CGameSceneTitle::~CGameSceneTitle()
{
	SAFE_DELETE(m_pController);
	SAFE_DELETE(m_pTitleUI);
	SAFE_DELETE(m_pTImg);
}

void CGameSceneTitle::Update()
{
	m_pController->Update();
	m_pTitleUI->Update();

	if (m_pTitleUI->IsDecided())
	{
		if (m_pTitleUI->GetSelect() == CTitleUI::enSelect::Start)
		{
			//準備画面へ.
			SenenChang(enScene::PlayerSetUp, CSceneChange::TransitionType::Fade, 60, 60);
		}
		else
		{
			//タイトル前の動画へorゲーム完全終了の処理.(未定)//2026.07.30.

		}
	}

}

void CGameSceneTitle::Draw()
{
	m_pDx11->SetDepth(false);
	Projection();

	m_pTImg->m_pSprite->SetisCOLOR(true);
	static float r, g, b = 0;
	r += rand() % 100 / 1000.f;
	g += rand() % 100 / 1000.f;
	b += rand() % 100 / 1000.f;
	if (r > 1) {
		r--;
	}
	if (g > 1) {
		g--;
	}
	if (b > 1) {
		b--;
	}

	m_pTImg->m_pSprite->SetCOLOR({ r,g,b });
	m_pTImg->Draw();

	m_pTitleUI->Draw();

	m_pDx11->SetDepth(true);



}
