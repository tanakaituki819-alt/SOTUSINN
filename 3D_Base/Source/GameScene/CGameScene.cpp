#include "CGameScene.h"

CGameScene::CGameScene(HWND Hwnd, CDirectX9* Dx9, CDirectX11* Dx11, CCamera* m_Camera)
	:m_hWnd(Hwnd)//モデルの作成に必要
	, m_pDx9(Dx9)//
	, m_pDx11(Dx11)//

	, m_pCamera(m_Camera)//描画に必要(継承先クラスで設定する)
	, m_Light()
	, m_mView()
	, m_mProj()

	, m_enScene()
	, m_SceneChangeFlag()
{
}

CGameScene::~CGameScene()
{
	m_pDx9 = nullptr;
	m_pDx11 = nullptr;
	m_hWnd = nullptr;
}




void CGameScene::SenenChang(enScene enScene, CSceneChange::TransitionType TransitionType, int Farst, int End)
{
	m_SceneChangeFlag = enScene;
	m_First = Farst;
	m_End = End;
	m_SceneChange = TransitionType;
}

void CGameScene::Projection()
{
	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 1000.0f;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);	//遠いビュー平面のz値.
}
