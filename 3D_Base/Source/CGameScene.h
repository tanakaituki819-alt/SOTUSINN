#pragma once
#include "CStaticMesh.h"
#include"CCamera.h"
#include"CSceneChange.h"
class CGameScene
{
public:
	CGameScene(HWND	Hwnd, CDirectX9* Dx9, CDirectX11* Dx11,CCamera* m_Camera);
	virtual ~CGameScene();
	virtual void Update() = 0;
	virtual void Draw() = 0;	//カメラを参照しなくて良い
	
	enScene ChangeScene() const { return m_SceneChangeFlag; };


	enScene GetScene()const {return m_enScene;};//使っていない
	

	int GetSenenChangTimeFarst()const {return m_First;}
	int GetSenenChangTimeEnd()const {return m_End;}
	CSceneChange::TransitionType GetSenenChangTransitionType()const { return m_SceneChange; }
protected:
	void SenenChang(enScene enScene, CSceneChange::TransitionType TransitionType = CSceneChange::TransitionType::Fade, int Farst = 0, int End = 0);
	//プロジェクション関数.
	void Projection();
	enScene m_SceneChangeFlag;	//シーン変更フラグ
	enScene m_enScene;
	int m_First;
	int m_End;
	CSceneChange::TransitionType m_SceneChange;

	CDirectX9*  m_pDx9;
	CDirectX11* m_pDx11;
	//ウィンドウハンドル.
	HWND			m_hWnd;


	//ライト情報.
	LIGHT			m_Light;
	//行列.
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）行列.
	CCamera*			m_pCamera;
};

