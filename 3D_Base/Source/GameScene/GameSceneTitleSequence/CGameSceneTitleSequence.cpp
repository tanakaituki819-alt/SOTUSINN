#include "CGameSceneTitleSequence.h"
#include "Game/CGame.h"

CGameSceneTitleSequence::CGameSceneTitleSequence(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
{
	m_pTitleChopsticks = new CTitleChopsticks();	//お箸クラスインスタンス作成.
	m_pBackImg = new CUIObject();					//背景インスタンス作成.
	m_pBackImg->SetScale(WND_W, WND_H, 100.f);		//スケール.
	m_pBackImg->AttachSprite(*CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround));	//背景設定.
	//ライト情報.
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);	// ライト方向.
	m_Light.fIntensity = 2;//	ライトパワー

}

CGameSceneTitleSequence::~CGameSceneTitleSequence()
{
}

void CGameSceneTitleSequence::Update()
{
	if (m_pTitleChopsticks->GetSceneChangeflag()) {
		SenenChang(enScene::Title, CSceneChange::TransitionType::Fade, 10, 0);
	}
	m_pTitleChopsticks->Update();
}

void CGameSceneTitleSequence::Draw()
{
	m_pDx11->SetDepth(false);
	m_pCamera->Cmera();
	Projection();
	m_pBackImg->Draw();	//背景描画.
	m_pTitleChopsticks->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());	//お箸描画.
	Effect::GetInstance()->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());
	m_pDx11->SetDepth(true);	//深度テスト無効.
}
