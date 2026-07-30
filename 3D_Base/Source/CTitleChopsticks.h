#pragma once
#include "CStaticMeshObjObject.h"

class CTitleChopsticks
	: public CStaticMeshObjObject
{
public:
	/***********************************************
	*	定数宣言.
	**/
	static const int Chopsticks_Max = 4;	//お箸の最大数.
	
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
	~CTitleChopsticks()override;

	void Update()override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera)override;
	void SetCaseId(int nextCaseId);

private:
	CStaticMeshObjObject* m_pChopsticks[Chopsticks_Max]	= {};	//お箸(お箸を二つ描画するために配列化).
	ChopstickState	m_ChopsticksState[Chopsticks_Max]	= {};	//お箸個別のステート.
	float	m_PosX	  = 2.5f;					//共通で使うXポジション.
	float	m_PosY	  = 1.6f;					//共通で使うYポジション.
	float	m_PosZ	  = 4.4f;					//共通で使うZポジション.
	float	m_Pitch	  = D3DXToRadian(12.0f);	//X角度.
	float	m_Yaw	  = D3DXToRadian(-68.0f);	//Y角度.
	float	m_Roll	  = D3DXToRadian(26.0f);	//Z角度.
	float	m_MoveX	  = 0.3f;					//移動スピード.
	float	m_MoveY	  = 0.3f;					//移動スピード.
	float	m_SpeedX  = 0.08f;					//X方向スピード.
	float	m_SpeedY  = 0.08f / 1.5f;			//Y方向スピード.
	bool	m_AttackMoveNow = true;				//今攻撃していいか.
	int		m_CaseId  = 1;						//ケースID.
	CAMERA* m_pCamera = nullptr;				//カメラ.
};
