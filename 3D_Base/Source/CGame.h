#pragma once
#include "CDirectX9.h"
#include "CDirectX11.h"
#include "CSprite3D.h"
#include "CSprite2D.h"
#include "CSpriteObject.h"
#include "CCharacter.h"

#include "CShot.h"
#include "CExplosion.h"
#include "CUIObject.h"
#include "CStaticMesh.h"
#include "CStaticMeshObject.h"

//
#include"CGameScene.h"

//
#include"CFadeSceneTransition.h"
#include "CSceneChange.h"

#include"CCamera.h"
/********************************************************************************
*	ゲームクラス.
**/


class CGame
{
private:
public:

	CGame( CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd );
	~CGame();

	void Create();
	HRESULT LoadData();
	void Release();

	virtual void Update();
	virtual void Draw();

private:
	//プロジェクション関数.
	void Projection();

private:
	CDirectX9*		m_pDx9;
	CDirectX11*		m_pDx11;

	//ウィンドウハンドル.
	HWND			m_hWnd;

	//カメラ情報.
	CAMERA			m_Camera;
	//ライト情報.
	LIGHT			m_Light;

	//行列.
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）行列.


private:
	CCamera* m_pCamara;//カメラ

	CGameScene* m_pGeamScene;//ゲームシーン
	CSceneChange* m_SceneChanger;//シーンチェンジの仕方
private:
	void StartChangeScene();	//シーン変更開始関数（使うシーンチェンジの選択とシーンチェンジの時間固定）
	void ChangeScene();	//シーン変更関数
	//=delete「削除定義」と呼ばれる機能.
	//指定された場合、その関数は呼び出せなくなる.
	CGame() = delete;	//デフォルトコンストラクタ禁止.
	CGame( const CGame& ) = delete;
	CGame& operator = (const CGame& rhs ) = delete;
};