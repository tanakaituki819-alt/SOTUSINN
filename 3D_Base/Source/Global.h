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

#include "MyMacro.h"




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