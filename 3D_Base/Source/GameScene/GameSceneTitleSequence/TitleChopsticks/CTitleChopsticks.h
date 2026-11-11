#pragma once
#include <array>
#include <memory>
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
	*	列挙型.
	**/
	//お箸の動きの段階.
	enum class Phase {
		FirstPair	= 0,	//右上と左下の2本.
		SecondPair	= 1,	//左上と右下の2本.
		Final		= 2,	//4本全部(最後にシーンチェンジ).
	};

public:
	CTitleChopsticks();
	~CTitleChopsticks() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
	void SetPhase(Phase next);	//段階を切り替えて、初期状態に戻す.

	bool GetSceneChangeflag() const { return m_SceneChangeflag; }

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }

private:
	void  UpdateHitStop();				//衝突後の静止時間を数える.
	void  UpdateMove();					//お箸を動かし、衝突と段階の切り替えを判定する.
	void  OnImpact();					//ぶつかった瞬間の処理.
	void  PlayHitEffect();				//雷のエフェクトを出す.
	void  ApplyPositions();				//移動量から、お箸の位置を決める.
	bool  IsActive(int index) const;	//今の段階で動かすお箸か.
	float GetImpactDist() const;		//ぶつかる位置までの移動量(X方向).

private:
	std::array<std::unique_ptr<CStaticMeshObjObject>, Chopsticks_Max> m_pChopsticks;	//お箸オブジェクト.
	CXInput* m_pController[Chopsticks_Max] = {};	//コントローラー参照.

	//座標回転パラメータ.
	D3DXVECTOR3 m_BasePos = D3DXVECTOR3(2.5f, 1.6f, 4.4f);	//出発位置(右上の角を基準にした X, Y, Z).
	D3DXVECTOR3 m_BaseRot = D3DXVECTOR3(D3DXToRadian(12.0f), D3DXToRadian(-68.0f), D3DXToRadian(26.0f));	//基準回転.

	//動きの状態.
	Phase m_Phase			= Phase::FirstPair;	//現在の段階.
	float m_MoveDist		= 0.f;				//出発位置から中心へ向かった距離(X方向).
	float m_MoveSpeed		= 0.f;				//1フレームの移動量(マイナスで戻る).
	bool  m_AttackMoveNow	= true;				//中心へ向かっている最中か(falseなら戻り中).
	int   m_StopTimer		= 0;				//衝突後に止まっている残りフレーム.
	bool  m_SceneChangeflag = false;			//シーン移動していいか.
};