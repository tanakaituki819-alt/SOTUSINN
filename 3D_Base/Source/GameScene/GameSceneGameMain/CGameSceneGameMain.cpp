#include "Game/CGame.h"
#include "CGameSceneGameMain.h"



CGameSceneGameMain::CGameSceneGameMain(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pGround				( nullptr )
	, m_pPlayer				(  )
	, m_pCollisionManager	()
	, m_pPauseUI			( nullptr )
	, m_Pause				( false )

{

	//カメラ座標.
	m_pCamera->SetPosition(D3DXVECTOR3(0.f, 10.f, -5.f));
	m_pCamera->SetLookPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pCamera->SetUpVec(D3DXVECTOR3(0.f, 0.f,1.0f ));//前が上

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


	m_pStaticMeshBSphere = CSpriteManager::GetMesh(CSpriteManager::enMeshList::Sphere);
	m_pBackGround = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_GameMainGround);
	m_pBackGround->SetScale(D3DXVECTOR3{ 1920, 1080, 200 });

	m_pCing = new CIngredients();

	//当たり判定クラス.
	m_pCollisionManager = new CCollisionManager();
	for (int i = 0; i < PlayerMax; i++) {
		m_pCollisionManager->SetPlyaer(*m_pPlayer[i],i);	//プレイヤー数分セット.
	
	}
	
	//ポーズUIの生成とコントローラーをセット.
	m_pPauseUI = new CPauseUI();
	m_pPauseUI->SetXInput(CONTROLA[0]);

	m_pCIngredientsM = new CIngredientsmanager();
	m_pCIngredientsM->SetNabe(m_pGround);

	m_pTimer = new CTimer();
	m_pTimer->SetTime(90* 60);
	m_pCollisionManager->SetIngredients(*m_pCIngredientsM);		//具材マネージャーセット
}

CGameSceneGameMain::~CGameSceneGameMain()
{
	Effect::StopAll();
	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pPauseUI);
	SAFE_DELETE(m_pCollisionManager);

	SAFE_DELETE(m_pGround);
	for (int i = 0; i < PlayerMax; i++ ) {
		SAFE_DELETE(m_pPlayer[i]);
	}
}

void CGameSceneGameMain::Update()
{
	for (int i = 0;i < PlayerMax;i++) {
		CONTROLA[i]->Update();
	}
	m_pCollisionManager->Update();
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
	m_pCIngredientsM->SetTimu(m_pTimer->GetTimu());
	m_pCIngredientsM->Update();
	if (m_pTimer->GetTimu()<=0&& m_pCIngredientsM->GetIngredientsliveing()<=0) {
		SenenChang(enScene::WinnnerResult, CSceneChange::TransitionType::RSRIDE, 60, 60);
	}

	////エフェクト制御
	static ::EsHandle handle = -1;


	if (GetAsyncKeyState('C') & 0x0001) {
		static ::EsHandle handle = -1;
		handle=Effect::Play(EFE::KANSEI,D3DXVECTOR3(0.f, 0.f, 0.f));
	}
	if (GetAsyncKeyState('Z') & 0x0001) {
		SenenChang(enScene::WinnnerResult, CSceneChange::TransitionType::FUSUMA, 60, 60);
	}
	m_pTimer->Update();
	
	UpdateBSpherePos();

	CheckCollision();

}

void CGameSceneGameMain::Draw()
{
	m_pCamera->Cmera();
	Projection();

	//背景
	m_pDx11->SetDepth(false);


	m_pBackGround->Render();

	m_pDx11->SetDepth(true);


	m_pGround->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());


	m_pCIngredientsM->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());

	m_pGround->DrawWater(m_pCamera->GetView(), m_mProj);
	for (int i = 0;i < PlayerMax;i++) {
		m_pPlayer[i]->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());
	}
	Effect::GetInstance()->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());

	m_pDx11->SetDepth(false);
	for (int i = 0;i < PlayerMax;i++) {
		m_pPlayer[i]->DrawUI();
	}
	m_pTimer->Draw();

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
	m_pCIngredientsM->UpdateBSpherePos();
}

void CGameSceneGameMain::CheckCollision()
{

}
