// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します.
//
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
//#include "targetver.h"

// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#define NOMINMAX
#include <stdio.h>
#include <tchar.h>


#include <Windows.h>
#include"System/Global/Global.h"
#include "System/MyMacro.h"

//DirectX9.
#include <d3dx9.h>
//DirectX11.
#include <D3DX11.h>
#include <D3D11.h>
//DirectX10.
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include"SpriteManager/CSpriteManager.h"

#include "Game/enScene.h"

#include"System/XInput/CXInput.h"

#include <iostream>
