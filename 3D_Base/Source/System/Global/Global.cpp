#include "Global.h"

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

D3DXVECTOR3 GetPosition2D(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3 Position, D3DXVECTOR3 Scale, D3DXVECTOR3 Rotation)
{
	D3DXVECTOR3 Output;
	D3DXVECTOR3 Pos = { 0,0,0 };//オブジェクトのどこを見るか
	_D3DVIEWPORT9 Viewport{ 0 };//ビューポート
	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.Width = static_cast<DWORD>(WND_W);
	Viewport.Height = static_cast<DWORD>(WND_H);
	Viewport.MinZ = 0.0f;
	Viewport.MaxZ = 1.0f;

	//ワールド行列、スケール行列、回転行列、平行移動行列.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	//拡大縮小行列作成.
	D3DXMatrixScaling(
		&mScale,	//(out)計算結果.
		Scale.x, Scale.y, Scale.z);	//x,y,zそれぞれの拡縮値.
	//Y軸回転行列作成.
	D3DXMatrixRotationY(&mYaw, Rotation.y);
	//X軸回転行列作成.
	D3DXMatrixRotationX(&mPitch, Rotation.x);
	//Z軸回転行列作成.
	D3DXMatrixRotationZ(&mRoll, Rotation.z);
	//平行移動行列作成.
	D3DXMatrixTranslation(
		&mTran,	//(out)計算結果.
		Position.x, Position.y, Position.z);	//x,y,z座標.

	//回転行列を作成.
	mRot = mYaw * mPitch * mRoll;

	//ワールド行列作成.
	//拡縮×回転×移動 ※順番がとても大切！！.
	//mWorld = mScale * mRot ;
	mWorld = mScale * mRot * mTran;
	// 1. カメラ空間（ビュー空間）での位置を確認
	D3DXMATRIX mWorldView = mWorld * mView;
	D3DXVECTOR3 cameraSpacePos;
	D3DXVec3TransformCoord(&cameraSpacePos, &Pos, &mWorldView);
	//x,y,z(深度)
	D3DXVec3Project(&Output, &Pos, &Viewport, &mProj, &mView, &mWorld);

	return Output;
}
