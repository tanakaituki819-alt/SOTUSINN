#pragma once
#include "CStaticMesh.h"

/**************************************************
*	バウンディングスフィアクラス.
**/
class CBoundingSphere
{
public:
	CBoundingSphere();
	~CBoundingSphere();

	//モデルに合わせたバウンディングスフィアを作成.
	HRESULT CreatSphereForMesh( const CStaticMesh& pMesh );
	HRESULT CreatSphereForMesh(const CStaticObjMesh& pMesh);
	
	//他の球体との当たり判定.
	bool IsHit(const CBoundingSphere& pBSphere);
	
	//中心座標を設定する.
	void SetPosition(const D3DXVECTOR3& Pos){ m_Position = Pos;		}
	//半径(長さ)を設定する.
	void SetRadius(float Radius)			{ m_Radius = Radius;	}
	float GetRadius() { return m_Radius; };
private:
	D3DXVECTOR3		m_Position;	//中心座標.
	float			m_Radius;	//半径(長さ).
};