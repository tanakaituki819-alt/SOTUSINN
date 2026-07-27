#pragma once
#include "CStaticMeshObjObject.h"

class CTitleChopsticks
	: public CStaticMeshObjObject
{
public:
	static const int Chopsticks_Max = 4;	//お箸の最大数.
public:
	CTitleChopsticks();
	~CTitleChopsticks()override;

	void Update()override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera)override;
private:
	CStaticMeshObjObject* m_pChopsticks[Chopsticks_Max] = {};	//お箸(お箸を二つ描画するために配列化).
	float	m_PosX	= 2.5f;					//共通で使うXポジション.
	float	m_PosY	= 1.6f;					//共通で使うYポジション.
	float	m_PosZ	= 4.4f;					//共通で使うZポジション.
	float	m_Pitch	= D3DXToRadian(12.0f);	//X角度.
	float	m_Yaw	= D3DXToRadian(-68.0f);	//Y角度.
	float	m_Roll	= D3DXToRadian(26.0f);	//Z角度.
	float	m_Move	= 0.3f;					//移動スピード.
	bool	m_AttackMoveNow = true;			//今攻撃していいか.
};
