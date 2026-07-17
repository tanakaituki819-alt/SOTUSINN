#include "CGame.h"
#include "CGameSceneGameMain.h"


#include"Effect.h"
CGameSceneGameMain::CGameSceneGameMain(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pGround		( nullptr )
	, m_pPlayer		(  )
	, m_pPauseUI	( nullptr )
	, m_Pause		( false )
{

	//カメラ座標.
	m_pCamera->SetPosition(D3DXVECTOR3(0.f, 10.f, -5.f));
	m_pCamera->SetLookPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pCamera->SetUpVec(D3DXVECTOR3(0.f, 0.f, 1.f));//前が上
	//ライト情報.

	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);	// ライト方向.
	m_Light.fIntensity = 1;//	ライトパワー


	
	
	m_pGround = new CNabe();
	for (int i = 0;i < PlayerMax;i++) {
		CONTROLA[i] = new CXInput(i);
		m_pPlayer[i] = new CPlayer();
		m_pPlayer[i]->SetXInput(CONTROLA[i]);
		m_pPlayer[i]->SetPlayerNo(i);
	}


	//エフェクト
	Effect::GetInstance()->Create(m_pDx11->GetDevice(), m_pDx11->GetContext());
	Effect::GetInstance()->LoadData();


	m_pStaticMeshBSphere = CSpriteManager::GetMesh(CSpriteManager::enMeshList::Sphere);

	m_pCing = new CIngredients();

	//ポーズUIの生成とコントローラーをセット.
	m_pPauseUI = new CPauseUI();
	m_pPauseUI->SetXInput(CONTROLA[0]);

	m_pCingM = new CIngredientsmanager();
	m_pCingM->SetNabe(m_pGround);
}

CGameSceneGameMain::~CGameSceneGameMain()
{

	SAFE_DELETE(m_pPauseUI);

	SAFE_DELETE(m_pGround);
	for (int i = 0;i < PlayerMax;i++) {
		SAFE_DELETE(m_pPlayer[i]);
	}
}

void CGameSceneGameMain::Update()
{
	for (int i = 0;i < PlayerMax;i++) {
		CONTROLA[i]->Update();
	}

	//1Pがスタートボタンをしたらポーズ.
	if (CONTROLA[0]->IsDown(CXInput::START, true))
	{
		m_Pause = !m_Pause;
		if (m_Pause)
		{
			m_pPauseUI->OpenInit();
		}
	}
	
	//ポーズ中
	if (m_Pause)
	{
		m_pPauseUI->Update();
		
		if (m_pPauseUI->IsDecided())
		{
			if (m_pPauseUI->GetSelect() == CPauseUI::enSelect::ReturnToTitle)
			{
				//タイトルに戻る
				SenenChang(enScene::Title, CSceneChange::TransitionType::Fade, 60, 60);
				m_Pause = false;
			}
			else
			{
				//ゲームに戻る.
				m_Pause = false;
				m_pPauseUI->CloseInit();
			}
		}
		return;	//次に行かないように.
	}

	m_pGround->Update();
	for (int i = 0;i < PlayerMax;i++) {
		m_pPlayer[i]->Update();
	}
	m_pCingM->Update();
	
	////エフェクト制御
	//static ::EsHandle handle = -1;

	//static int i = 0;
	//i += 6;
	//if (GetAsyncKeyState('C') & 0x0001) {
	//	handle=Effect::Play(EFE::Test0,D3DXVECTOR3(0.f, 0.f, 0.f));
	//}
	//if (GetAsyncKeyState('V') & 0x0001 ) {
	//	Effect::StopAll();
	//	handle = -1;
	//}

	if (GetAsyncKeyState('C') & 0x8000) {
		m_pCingM->Create();
	}

	//Effect::SetRotation(handle, D3DXVECTOR3(0.f,D3DXToRadian(i), 0.f));
	UpdateBSpherePos();

	CheckCollision();

}

void CGameSceneGameMain::Draw()
{
	m_pCamera->Cmera();
	Projection();

	//背景
	m_pDx11->SetDepth(false);
	static float S=0;
	S += 0.01;


	m_pDx11->SetDepth(true);


	m_pGround->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());


	m_pCingM->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());

	m_pGround->DrawWater(m_pCamera->GetView(), m_mProj);
	for (int i = 0;i < PlayerMax;i++) {
		m_pPlayer[i]->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());
	}
	Effect::GetInstance()->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());

	m_pDx11->SetDepth(false);
	for (int i = 0;i < PlayerMax;i++) {
		m_pPlayer[i]->DrawUI();
	}

	//ポーズ
	if (m_Pause)
	{
		m_pPauseUI->Draw();
	}

	m_pDx11->SetDepth(true);



}

void CGameSceneGameMain::UpdateBSpherePos()
{
	//当たり判定の中心座標を更新する.
	//m_pPlayer->UpdateBSpherePos();
	m_pCingM->UpdateBSpherePos();
}

void CGameSceneGameMain::CheckCollision()
{

}
