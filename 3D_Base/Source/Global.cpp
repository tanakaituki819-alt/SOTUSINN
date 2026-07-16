#include "stdafx.h"

bool D2CollizionXZ(D3DXVECTOR3 pos, float SC, D3DXVECTOR3 pos2, float SC2)
{
	D3DXVECTOR3 m_Position = { pos.x,0,pos.z };
	D3DXVECTOR3 m_Position2 = { pos2.x,0,pos2.z };
	//２つの球体の中心間の距離を求める.
	D3DXVECTOR3 vLenth = m_Position - m_Position2;
	//上記のベクトルから長さに変換.
	float length = D3DXVec3Length(&vLenth);

//SC2からSCを引くことでSC2のなかに完全に埋まっている奈良にする

	if (length <= SC2- SC )
	{
		return true;	//衝突している.
	}
	return false;		//衝突していない.
}
