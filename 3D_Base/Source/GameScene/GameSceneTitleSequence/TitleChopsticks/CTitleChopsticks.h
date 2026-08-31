#pragma once

#include "GameObject/StaticMeshObjObject/CStaticMeshObjObject.h"

class CTitleChopsticks
	: public CStaticMeshObjObject
{
public:
	/***********************************************
	*	定数宣言.
	**/
	static const int Chopsticks_Max = 4;	// お箸の最大数.

	/***********************************************
	*	構造体.
	**/
	//箸1本の状態.
	struct ChopstickState {
		D3DXVECTOR3 pos;      //現在位置.
		D3DXVECTOR3 velocity; //移動速度.
		D3DXVECTOR3 rot;      //回転角度.
	};

public:
	CTitleChopsticks();
	~CTitleChopsticks() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
	void SetCaseId(int nextCaseId);

	bool GetSceneChangeflag() { return m_SceneChangeflag; }

private:
	//メッシュステート.
	CStaticMeshObjObject* m_pChopsticks[Chopsticks_Max] = {};		//お箸オブジェクト配列.
	ChopstickState        m_ChopsticksState[Chopsticks_Max] = {};	//お箸個別のステート.
	//座標回転パラメータ.
	D3DXVECTOR3 m_BasePos = D3DXVECTOR3(2.5f, 1.6f, 4.4f);          //初期基準位置(X, Y, Z).
	D3DXVECTOR3 m_BaseRot = D3DXVECTOR3(D3DXToRadian(12.0f), D3DXToRadian(-68.0f), D3DXToRadian(26.0f));	//初期基準回転.
	//移動速度.
	D3DXVECTOR2 m_MoveOffset = D3DXVECTOR2(0.3f, 0.3f);				//現在の移動オフセット量(X, Y).
	D3DXVECTOR2 m_MoveSpeed = D3DXVECTOR2(0.08f, 0.08f / 1.5f);		//移動スピード(X, Y).
	//制御フラグ状態.
	bool    m_AttackMoveNow = true;		//攻撃移動中フラグ.
	bool	m_SceneChangeflag = false;	//シーン移動していいか判定.
	int     m_CaseId = 1;				//現在のケースID.
	CAMERA* m_pCamera = nullptr;		//カメラ参照.
	float progress = {};				//お箸が中央に向かう進捗率.
	D3DXMATRIX m_CustomView = {};		//ケース3演出用のカメラ行列を複製.
	D3DXMATRIX m_MatZoom;				//ケース3演出用のズーム行列.
};