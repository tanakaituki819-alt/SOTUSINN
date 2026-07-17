#include "CGame.h"
#include"CGameSceneTitle.h"
#include"CGameSceneGameMain.h"
#include "CGameScenePlayerSetup.h"

#include "enScene.h"
#include "CSoundManager.h"
#include"RawInput.h"
//コンストラクタ.
CGame::CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd )
	: m_pDx9				( &pDx9 )
	, m_pDx11				( &pDx11 )
	, m_hWnd				( hWnd )
	, m_mView				()
	, m_mProj				()

	, m_Camera				()
	, m_Light				()

	, m_pGeamScene(nullptr)
	, m_SceneChanger(nullptr)
{

}


//デストラクタ.
CGame::~CGame()
{

	Release();
	//外部で作成しているので、ここでは破棄しない.	m_hWnd = nullptr;
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

//構築.
void CGame::Create()
{
	RawInput::firstSetting(m_hWnd);
	CSpriteManager::LoadDeat(*m_pDx11,*m_pDx9);
	m_SceneChanger = new CSceneChange();
	m_pCamara = new CCamera();
	m_pGeamScene = new CGameScenePlayerSetup(m_hWnd,m_pDx9,m_pDx11,m_pCamara);

}

//ロードデータ関数.
HRESULT CGame::LoadData()
{

	if (CSoundManager::GetInstance()->Load(m_hWnd) == false) {
		E_FAIL;
	}

	

	return S_OK;
}

//解放関数.
void CGame::Release()
{
	SAFE_DELETE(m_pGeamScene);
	SAFE_DELETE(m_SceneChanger);
}


//更新処理.
void CGame::Update()
{
	RawInput::Update();
	m_SceneChanger->Update();
	//CSoundManager::PlayLoop(CSoundManager::enSingleSoundList::ShutterOpen);
	if (m_pGeamScene != nullptr) {
		if (m_pGeamScene->ChangeScene() != enScene::None) {
			//シーン変更フラグが立っているとき
			if (m_SceneChanger->GetSceneChangeFlag() == CSceneChange::SceneChange::none) {
				StartChangeScene();
			}

		}
		//画面が真っ暗になったら
		if (m_SceneChanger->GetSceneChangeFlag() == CSceneChange::SceneChange::Underchange) {

			ChangeScene();//シーン変更
		}
		if (m_SceneChanger->GetSceneChangeFlag() == CSceneChange::SceneChange::none) {
			m_pGeamScene->Update();
		}

	}

}

//描画処理.
void CGame::Draw()
{

	m_pGeamScene->Draw();
	m_pDx11->SetDepth(false);

	m_SceneChanger->Draw();
	m_pDx11->SetDepth(true);

}


//プロジェクション関数.
void CGame::Projection()
{
	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 100.0f;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);	//遠いビュー平面のz値.
}

void CGame::StartChangeScene()
{
	switch (m_pGeamScene->ChangeScene())
	{
	case enScene::None:

		break;

	default:
		m_SceneChanger->SetSceneChangeFlag(m_pGeamScene->GetSenenChangTimeFarst(), m_pGeamScene->GetSenenChangTransitionType());
		break;

	}
}

void CGame::ChangeScene()
{
		switch (m_pGeamScene->ChangeScene())
		{
			//タイトルに代わるとき
		case enScene::Title:
			m_SceneChanger->endSceneChange(m_pGeamScene->GetSenenChangTimeEnd());
			SAFE_DELETE(m_pGeamScene);
			m_pGeamScene = new CGameSceneTitle(m_hWnd, m_pDx9, m_pDx11, m_pCamara);
			break;
	
		case enScene::GameMain:
			m_SceneChanger->endSceneChange(m_pGeamScene->GetSenenChangTimeEnd());
			SAFE_DELETE(m_pGeamScene);
			m_pGeamScene = new CGameSceneGameMain(m_hWnd, m_pDx9, m_pDx11, m_pCamara);
			break;
		case enScene::PlayerSetUp:
			m_SceneChanger->endSceneChange(m_pGeamScene->GetSenenChangTimeEnd());
			SAFE_DELETE(m_pGeamScene);
			m_pGeamScene = new CGameScenePlayerSetup(m_hWnd, m_pDx9, m_pDx11, m_pCamara);
			break;

		}
	
}
