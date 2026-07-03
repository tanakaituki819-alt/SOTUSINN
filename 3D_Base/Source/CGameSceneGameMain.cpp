#include "CGame.h"
#include "CGameSceneGameMain.h"


#include"Effect.h"
CGameSceneGameMain::CGameSceneGameMain(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pGround(nullptr)
	, m_pPlayer(nullptr)

{

	//カメラ座標.
	m_pCamera->SetPosition(D3DXVECTOR3(0.f, 0.f, -5.f));
	m_pCamera->SetLookPosition(D3DXVECTOR3(0.f, 0.f, 0.f));

	//ライト情報.

	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);	// ライト方向.
	m_Light.fIntensity = 1;



	
	m_pGround = new CGround();
	m_pPlayer = new CPlayer();

	//エフェクト
	Effect::GetInstance()->Create(m_pDx11->GetDevice(), m_pDx11->GetContext());
	Effect::GetInstance()->LoadData();


	m_pStaticMeshBSphere = CSpriteManager::GetMesh(CSpriteManager::enMeshList::Sphere);


}

CGameSceneGameMain::~CGameSceneGameMain()
{



	SAFE_DELETE(m_pGround);
	SAFE_DELETE(m_pPlayer);
}

void CGameSceneGameMain::Update()
{




	m_pGround->Update();
	m_pPlayer->Update();

	
	//エフェクト制御
	static ::EsHandle handle = -1;

	static int i = 0;
	i += 6;
	if (GetAsyncKeyState('C') & 0x0001) {
		handle=Effect::Play(D3DXVECTOR3(0.f, 0.f, 0.f));
	}
	if (GetAsyncKeyState('V') & 0x0001 ) {
		Effect::StopAll();
		handle = -1;
	}
	Effect::SetRotation(handle, D3DXVECTOR3(0.f,D3DXToRadian(i), 0.f));
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
	m_pPlayer->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());

	Effect::GetInstance()->Draw(m_pCamera->GetView(), m_mProj, m_Light, m_pCamera->GetCamera());

	m_pDx11->SetDepth(false);


	m_pDx11->SetDepth(true);

}

void CGameSceneGameMain::UpdateBSpherePos()
{
	//当たり判定の中心座標を更新する.
	m_pPlayer->UpdateBSpherePos();

}

void CGameSceneGameMain::CheckCollision()
{

}
