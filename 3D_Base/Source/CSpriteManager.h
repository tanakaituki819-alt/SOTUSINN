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
		Img_Fad,				//フェード
		IMG_Shutter,			//シャッター
		Digit0_9,				//0-9まで;
		Img_Playerfont,			//プレイヤーフォント.
		Img_Playericon,			//プレイヤーアイコン
		Img_PlayerBackground,	//プレイヤーバックグラウンド.
		Img_BackGround,			//和室背景.
		Img_Player1,			//プレイヤー1.
		Img_Player2,			//プレイヤー2.
		Img_Player3,			//プレイヤー3.
		Img_Player4,			//プレイヤー4.
		Img_controller,			//Xboxコントローラー.
		Img_Text,				//文字.
		Img_Hand,				//いただきます用の手.
		Img_Scroll,				//巻物UI.
		Img_Xbox,				//Xboxコントローラー.
		Img_Cusoru,				//カーソル
		Img_Pause,				//ポーズ.
		IMG_PauseOptions,		//ポーズ中の選択肢.
		Img_Water,				//なべの中
		Img_WhiteBack,			//白画像.
		Img_TextFrame,			//テキストのフレーム.
		Img_SelectionFrame,		//セレクトフレーム.
		IMG_WinnerText,			//あっぱれ.
		IMG_WinnerFont,			//背景の色付きプレイヤーフォント
		Img_RED,				//タイマーの赤色
		Img_ButtonPush,			//ボタン.
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
		NABE,//なべ
		S_NIKU,//肉
		S_NINZIN,//人参
		S_DAIKON,//大根
		S_ENOKI,//エノキ
		S_HAKUSAI1,//白菜
		S_HAKUSAI2,//白菜
		S_KANI,//かに
		S_KUMANOTE,//クマの手
		S_NEGI,//ねぎ
		S_ROBUSTER,//ロブスター
		S_SAKANA,//リュウグウノツカイ
		S_SITAKE,//シイタケ
		S_TAI,//タイ
		S_TARA,//タラ
		S_TOUHU,//豆腐
		S_UINNER,//ウインナー
		Chopsticks,	//箸.
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


	ImgList IMG_LIST[static_cast <int>(enImagList::MAX)];
	StaticMeshList MESH_LIST[static_cast <int>(enImagList::MAX)];
	StaticMeshList MESH_OBJ_List[static_cast <int>(enImagList::MAX)];

	CSprite2D*			Sprite2D[static_cast <int>(enImagList::MAX)];
	CSprite3D*			Sprite3D[static_cast <int>(enImagList::MAX)];
	CSpriteBlock*		SpriteBlock[static_cast <int>(enImagList::MAX)];
	CSpriteTriangle*	SpriteTriangle[static_cast <int>(enImagList::MAX)];
	CStaticMesh* StaticMesh[static_cast <int>(enMeshList::MAX)];
	CStaticObjMesh* StaticObjMesh[static_cast <int>(enMeshObjList::MAX)];
};
//コードの短縮用
namespace {
	using CSpM = CSpriteManager;
	using CSpMLstiImg = CSpriteManager::enImagList;
}