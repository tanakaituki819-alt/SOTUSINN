#include "CGameSceneTitle.h"
#include "CGame.h"

CGameSceneTitle::CGameSceneTitle(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd,Dx9,Dx11, m_Camera)
{
	m_pTSprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TITLE);
	m_pTImg = new CUIObject();
	m_pTImg->SetScale( WND_W,WND_H,0);
	m_pTImg->AttachSprite(*m_pTSprite);
}

CGameSceneTitle::~CGameSceneTitle()
{
	SAFE_DELETE(m_pTImg);
}

void CGameSceneTitle::Update()
{
	if (GetAsyncKeyState('Z') & 0x8000) {
		
		SenenChang(enScene::PlayerSetUp, CSceneChange::TransitionType::Fade, 60, 60);
		
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
	m_pDx11->SetDepth(true);
}
