#include "CGameSceneTitle.h"
#include "Game/CGame.h"


CGameSceneTitle::CGameSceneTitle(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd,Dx9,Dx11, m_Camera)
	, m_pTitleUI(nullptr)
	, m_pController(nullptr)
{
	m_pTSprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TITLE);

	m_pTImg = new CUIObject();




	m_pTitleUI = new CTitleUI();
	
}

CGameSceneTitle::~CGameSceneTitle()
{

	SAFE_DELETE(m_pTitleUI);
	SAFE_DELETE(m_pTImg);
}
void CGameSceneTitle::StartFinalSetup()
{
	m_pTImg->SetScale(WND_W, WND_H, 0);
	m_pTImg->AttachSprite(*m_pTSprite);


	m_pTitleUI->SetXInput(m_pController);
	m_pTitleUI->Init();

	m_pTitleUI->SetXInput(m_pController);
	m_pTitleUI->Init();
}
void CGameSceneTitle::Update()
{
	static int i = 0;
	static int c = 0;
	i++;
	c++;
	static EsHandle H=-1;
	if (i>20) {
		i = 0;
		H= Effect2D::Play(Effect2D::KANSEI, { 0, 0, 0 });
		Effect2D::SetScale(H,{20,20,20});
	}
	if (H!=-1) {
		Effect2D::SetLocation(H, { cos(D3DXToRadian(c)) * WND_W / 4 + WND_W/2,sin(D3DXToRadian(c)) * WND_H / 4 + WND_H/2,0 });
		Effect2D::SetRotation(H,{0,0,D3DXToRadian(c) });
	}


	m_pController->Update();
	m_pTitleUI->Update();

	//遷移
	if (m_pTitleUI->IsDecided())
	{
		if (m_pTitleUI->GetSelect() == CTitleUI::enSelect::Start)
		{
			SenenChang(enScene::PlayerSetUp, CSceneChange::TransitionType::Fade, 60, 60);
			m_pTitleUI->Init();
		}
		else if (m_pTitleUI->GetSelect() == CTitleUI::enSelect::Fin)
		{
			SenenChang(enScene::WinnnerResult);

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

	Effect2D::GetInstance()->Draw();

	m_pDx11->SetDepth(true);



}


