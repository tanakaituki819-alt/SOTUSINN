#pragma once

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>
#include <crtdbg.h>

//DirectX9.
#include <d3dx9.h>
//DirectX11.
#include <D3DX11.h>
#include <D3D11.h>
//DirectX10.
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "System/MyMacro.h"




//ライブラリ読み込み.
#pragma comment( lib, "winmm.lib" )
//DirectX9.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
//DirectX11.
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
//DirectX10.
#pragma comment( lib, "d3dx10.lib" )	//「D3DX～」の定義使用時に必要.

//=================================================
//	定数.
//=================================================
const int WND_W = 1280;	//ウィンドウの幅.
const int WND_H = 720;	//ウィンドウの高さ.
const int FPS = 60;		//フレームレート.

//=================================================
//	構造体.
//=================================================
//カメラ情報構造体.
struct CAMERA
{
	D3DXVECTOR3	Position;	//視点.
	D3DXVECTOR3 Look;		//注視点.
};

//ライト情報構造体.
struct LIGHT
{
	D3DXVECTOR3	Position;	//位置.
	D3DXVECTOR3	vDirection;	//方向.
	D3DXMATRIX	mRotation;	//回転行列.
	float		fIntensity;	//強度(明るさ).
};
//======================================
//	構造体.
//======================================
//幅高さ構造体.
struct WHSIZE
{
	float w;
	float h;
};
//スプライト構造体.
struct SPRITE_STATE
{
	D3DXVECTOR3 Disp;	//表示幅高さ.
	WHSIZE Base;	//元画像幅高さ.
	WHSIZE Stride;	//１コマあたりの幅高さ.
};

bool D2CollizionXZ(D3DXVECTOR3 pos, float SC, D3DXVECTOR3 pos2, float SC2);

//レイ構造体.
struct RAY
{
	D3DXVECTOR3	Axis;		//軸.
	D3DXVECTOR3	Position;	//位置.
	float		Length;		//長さ.
	float		RotationY;	//Y軸回転.

	RAY() : Axis(), Position(), Length(), RotationY() {}
};

//レイ構造体（十字）.
struct CROSSRAY
{
	enum enDir { ZF, ZB, XL, XR, max };
	RAY Ray[enDir::max];

	CROSSRAY() : Ray()
	{
		Ray[enDir::ZF].Axis = D3DXVECTOR3(0.f, 0.f, 1.f);
		Ray[enDir::ZB].Axis = D3DXVECTOR3(0.f, 0.f, -1.f);
		Ray[enDir::XL].Axis = D3DXVECTOR3(-1.f, 0.f, 0.f);
		Ray[enDir::XR].Axis = D3DXVECTOR3(1.f, 0.f, 0.f);

		for (int i = 0; i < enDir::max; i++) {
			Ray[i].Length = 1.f;
		}
	}
};


//よく使うもの
//D3DXVECTOR3;//x,y,zの変数型

// D3DXVECTOR3を長さに変換
// D3DXVec3Length()

//D3DXToRadian(Degree);
//D3DXToRadian(Degree);

//ラジアンを度に変換します
//D3DXToDegree(Radian);

//D3DXMatrixRotationX();
//D3DXMatrixRotationY();
//D3DXMatrixRotationZ();
//D3DXMATRIX* D3DXMatrixRotationZ(
//	D3DXMATRIX* pOut, // 回転行列が格納されるポインタ（出力）
//	FLOAT       Angle // 回転角度（ラジアン単位）
//);

//D3DXMatrixRotationAxis();
//D3DXMATRIX* D3DXMatrixRotationAxis(
//	_Inout_ D3DXMATRIX* pOut,      // 回転行列を格納するポインタ
//	_In_   const D3DXVECTOR3* pV, // 回転軸となる方向ベクトル（正規化必須）
//	_In_   FLOAT Angle            // 回転角度（ラジアン単位）
//);

//長さに変換
//D3DXVec3Length

//正規化
//D3DXVec3Normalize();

//D3D3DXVECTOR3* D3DXVec3Project(
//	D3DXVECTOR3* pOut,       // [out] 変換後の2D座標（Z成分には深度値が入る）
//	const D3DXVECTOR3* pV,         // [in]  変換したい3D座標
//	const D3DVIEWPORT9* pViewport, // [in]  ビューポート（画面サイズや描画範囲の情報）
//	const D3DXMATRIX* pProjection, // [in]  射影（プロジェクション）行列
//	const D3DXMATRIX* pView,       // [in]  ビュー行列
//	const D3DXMATRIX* pWorld       // [in]  ワールド行列
//);

D3DXVECTOR3 GetPosition2D(D3DXMATRIX& mView, D3DXMATRIX& mProj,D3DXVECTOR3 Scale,D3DXVECTOR3 Position, D3DXVECTOR3 Rotation)
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
