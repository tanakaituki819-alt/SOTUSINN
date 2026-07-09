#pragma once
#include"CSprite3D.h"
#include"CSprite2D.h"
#include"CSpriteBlock.h"
#include"CSpriteTriangle.h"
#include <vector>
#include "CStaticMesh.h"
#include"CStaticObjMesh.h"

//一番下にコード短縮用のネームスペースがあります
class CSpriteManager
{
public:

	~CSpriteManager();

	enum class enImagList
	{
		Img_TITLE=0,
		Img_GameClear,
		Img_GameOver,
		Img_Fad,//フェード
		Img_BackFire,//爆発
		IMG_Shutter,//シャッター
		IMG_Base,//拠点内
		Img_BattleArea,
		Img_STRBack,//星背景
		Img_EnergyGaugeBase,//エネルギーの後ろ
		Img_EnergyGauge,//エネルギーの前
		Img_HPGaugeBase,//HPの後ろ
		Img_HPGauge,//HP緑
		Img_HPGauge1,//HP赤
		DefenseBaseHP,//防衛拠点HP
		DefenseBaseHPback,//防衛拠点HPの後ろ
		Digit0_9,//0-9まで;
		Img_Playerfont,			//プレイヤーフォント.
		Img_Playericon,			//プレイヤーアイコン
		Img_PlayerBackground,	//プレイヤーバックグラウンド.
		Img_BackGround,			//和室背景.
		Img_Player1,			//プレイヤー1.
		Img_Player2,			//プレイヤー2.
		Img_Player3,			//プレイヤー3.
		Img_Player4,			//プレイヤー4.
		Img_Xbox,				//Xboxコントローラー.
		Img_Cusoru,				//カーソル
		MAX
	};
	//ｘファイル
	enum class enMeshList
	{
		Sphere,
		MAX
	};

	enum class enMeshObjList
	{
		ROBO,//ロボット
		FighterJet,//戦闘機（プレイヤー）
		Monster,//モンスター
		GOZIRA,//
		Apartment,//アパート
		Grassland,//草原
		Missile,//ミサイル
		HATO,//鳩
		
		ROBOShot,//射撃ポーズロボット
		ROBO＿Hi,//高ポリゴンロボット
		DefenseBase,//防衛拠点
		Beam,//ビーム

		MAX
	};

	static CSpriteManager* GetInstance()
	{
		static CSpriteManager s_Instance;
		return &s_Instance;
	}


	static bool LoadDeat(CDirectX11& pDx11, CDirectX9& pDx9) {
		return GetInstance()->Load(pDx11,pDx9);
	}

	static CSprite2D* GetSprite2D( enImagList No);//２D板ポリ
	static CSprite3D* GetSprite3D( enImagList No);//３D板ポリ
	static CSpriteBlock* GetSpriteBlock(enImagList No);//四角
	static CSpriteTriangle* GetSpriteTriangle( enImagList No);//四角推
	static CStaticMesh* GetMesh( enMeshList No);//xファイル
	static CStaticObjMesh* GetObjMesh(enMeshObjList No);//obj

	static void DeleteSprite2D(enImagList No);//２D板ポリ
	static void DeleteSprite3D(enImagList No);//３D板ポリ
	static void DeleteSpriteBlock(enImagList No);//四角
	static void DeleteSpriteTriangle(enImagList No);//四角推
	static void DeleteMesh(enMeshList No);//xファイル
	static void DeleteObjMesh(enMeshObjList No);//obj
	void AllDelete() { GetInstance()->allDelete(); };
private:
	void Load2D();
	void Load3D();

	 void allDelete();
	struct ImgList
	{
		int listNo;		//enList列挙型を設定
		LPCTSTR path;	//ファイルの名前(パス付)
		SPRITE_STATE SPRITE;
		bool centralstandards=false;//２Dスプライトの中央規準
	};
	struct StaticMeshList
	{
		int listNo;		//enList列挙型を設定
		LPCTSTR path;	//ファイルの名前(パス付)
		
	};

	bool Load(CDirectX11& pDx11, CDirectX9& pDx9);

	CDirectX11* Dx11;
	CDirectX9* Dx9;
	CSpriteManager();
	//コピーコンストラクタによるコピーを禁止する
	CSpriteManager(const CSpriteManager& rhs) = delete;
	//代入演算子によるコピーを禁止する
	CSpriteManager& operator=(CSpriteManager& rhs) = delete;
	//↑この3つの演算子のセットでシングルトン化が成立する
	std::vector <ImgList> Img_List;
	std::vector <StaticMeshList> MeshList;
	std::vector<StaticMeshList>MeshObjList;

	CSprite2D*			Sprite2D[static_cast <int>(enImagList::MAX)];
	CSprite3D*			Sprite3D[static_cast <int>(enImagList::MAX)];
	CSpriteBlock*		SpriteBlock[static_cast <int>(enImagList::MAX)];
	CSpriteTriangle*	SpriteTriangle[static_cast <int>(enImagList::MAX)];
	CStaticMesh* StaticMesh[static_cast <int>(enMeshList::MAX)];
	CStaticObjMesh* StaticObjMesh[static_cast <int>(enMeshObjList::MAX)];
};

namespace {
	using CSpM = CSpriteManager;
	using CSpMLstiImg = CSpriteManager::enImagList;
}