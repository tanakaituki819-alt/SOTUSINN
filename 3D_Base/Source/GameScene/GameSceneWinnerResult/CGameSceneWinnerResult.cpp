#include "CGameSceneWinnerResult.h"
#include "Game/CGame.h"

//お気に入り食材の表示位置と大きさ.
static const D3DXVECTOR2 FAVORITE_FOOD_POS = { 1120.f, 485.f };
static constexpr float FAVORITE_FOOD_RADIUS = 40.f;

CGameSceneWinnerResult::CGameSceneWinnerResult(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:CGameScene::CGameScene(Hwnd, Dx9, Dx11, m_Camera)
	, m_pWinnerResultUI(nullptr)
	, m_pFavoriteFood(nullptr)
	, m_pController(nullptr)
{
	//BGM再生.
	XAudio2::GetInstance()->BGMPlayHandle(XAudio2::BGMList::Result);

	m_pController = new CXInput(0);
	m_pWinnerResultUI = new CWinnerResultUI();
	m_pWinnerResultUI->SetXInput(m_pController);

	//お気に入り食材は単独1位のときだけ、そのプレイヤーの分を表示する.
	m_pFavoriteFood = new CWinnerResultFavoriteFood();
	for (int i = 0; i < CWinnerResultFavoriteFood::PLAYER_MAX; i++)
	{
		m_pFavoriteFood->SetVisible(i, false);
	}
	if (m_pWinnerResultUI->GetWinnerCount() == 1)
	{
		int No = m_pWinnerResultUI->GetWinnerNo(0);
		m_pFavoriteFood->SetLayout(No, FAVORITE_FOOD_POS, FAVORITE_FOOD_RADIUS);
		m_pFavoriteFood->SetVisible(No, true);
	}
}

CGameSceneWinnerResult::~CGameSceneWinnerResult()
{
	SAFE_DELETE(m_pFavoriteFood);
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
		SenenChang(enScene::Result, CSceneChange::TransitionType::FUSUMA, 10, 30);
		m_pWinnerResultUI->Init();
	}
}

void CGameSceneWinnerResult::Draw()
{
	m_pDx11->SetDepth(false);	//深度テスト無効.
	m_pWinnerResultUI->Draw();
	m_pDx11->SetDepth(true);	//深度テスト有効.
	m_pFavoriteFood->Draw();	//お気に入り食材.
}
