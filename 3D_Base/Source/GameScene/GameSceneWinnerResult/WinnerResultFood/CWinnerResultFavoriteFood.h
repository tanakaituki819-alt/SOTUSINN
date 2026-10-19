#pragma once
#include "SpriteManager/CSpriteManager.h"

/************************************************************
*	リザルト画面でお気に入り食材を表示するクラス.
**/
class CWinnerResultFavoriteFood
{
public:
	static const int PLAYER_MAX = 4;

public:
	CWinnerResultFavoriteFood();
	~CWinnerResultFavoriteFood();

	//UIを描画した後に呼ぶ.
	void Draw();

	//表示位置と大きさを設定.
	void SetLayout(int PlayerIndex, const D3DXVECTOR2& ScreenPos, float Radius);
	//表示するかどうかを設定.
	void SetVisible(int PlayerIndex, bool IsVisible);

private:
	//プレイヤー1人分の表示情報.
	struct SFoodView
	{
		CStaticObjMesh*	pMesh;		//表示するメッシュ.
		D3DXVECTOR3		Scale;		//スケール.
		D3DXVECTOR3		Center;		//モデルの中心.
		D3DXVECTOR3		Rotation;	//向き.
		bool			IsShell;	//カニかロブスターか.
		D3DXVECTOR2		ScreenPos;	//表示位置.
		float			Radius;		//表示サイズ.
		bool			IsVisible;	//表示するか.
	};

	//お気に入り食材のメッシュとサイズを設定する.
	void SetupFood(int PlayerIndex);

private:
	SFoodView	m_Food[PLAYER_MAX];

	D3DXMATRIX	m_mView;		//ビュー行列.
	D3DXMATRIX	m_mProj;		//射影行列.
	LIGHT		m_Light;		//ライト情報.
};
