#pragma once
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"	//Ingredients列挙体を使うため.

//========================================================
//	食材のカテゴリ.
//========================================================
enum class enIngredientCategory
{
	None,		//未分類(何も取っていない).
	Meat,		//肉系(肉、ウインナー).
	Fish,		//魚系(タラ).
	Veggie,		//野菜系(にんじん、大根、白菜、ねぎ).
	Mushroom,	//きのこ系(エノキ、シイタケ).
	Tofu,		//豆腐.
	Special,	//高級食材(カニ、クマの手、ロブスター、リュウグウノツカイ、タイ).
};

//========================================================
//	リザルト画面用のプレイヤー結果データ構造体.
//========================================================
struct SPlayerResultData
{
	bool	m_bActive;			//このプレイヤーが参加しているか.
	int		m_nScore;			//合計スコア.
	int		m_nMeatCount;		//獲得した肉系食材の数.
	int		m_nFishCount;		//獲得した魚系食材の数.
	int		m_nVeggieCount;		//獲得した野菜系食材の数.
	int		m_nMushroomCount;	//獲得したきのこ系食材の数.
	int		m_nTofuCount;		//獲得した豆腐の数.
	int		m_nSpecialCount;	//獲得した高級食材の数.
	int		m_nRank;			//順位.
	enIngredientCategory	m_FavoriteCategory;	//お気に入り(一番多く取った)カテゴリ.

	//初期化.
	SPlayerResultData()
		: m_bActive			(false)
		, m_nScore			(0)
		, m_nMeatCount		(0)
		, m_nFishCount		(0)
		, m_nVeggieCount	(0)
		, m_nMushroomCount	(0)
		, m_nTofuCount		(0)
		, m_nSpecialCount	(0)
		, m_nRank			(0)
		, m_FavoriteCategory(enIngredientCategory::None)
	{
	}
};

/************************************************************
*	リザルト画面へ渡すデータをまとめるシングルトンのクラス.
**/
class CResultDataManager
{
public:
	static const int PLAYER_MAX = 4;	//プレイヤー最大数.

public:
	//インスタンス取得.
	static CResultDataManager* GetInstance()
	{
		//唯一のインスタンスを作成する.
		static CResultDataManager s_Instance;
		return &s_Instance;
	}

	//CPlayerの配列からデータを取得してスナップショット化する.
	void CaptureFromPlayers(CPlayer* pPlayer[]);

	//参加人数を取得.
	int GetPlayerCount() const { return m_nPlayerCount; }

	//指定したプレイヤーの結果データを取得.
	const SPlayerResultData& GetPlayerData(int PlayerIndex) const;

	//1位のプレイヤー番号を取得．非参加のみの場合は0.
	int GetWinnerPlayerIndex() const;

	//1位で同着になっているプレイヤーをビットで表す.
	int GetTiedFirstPlaceMask() const;

	//食材の種類からカテゴリを判定する.
	static enIngredientCategory GetCategory(Ingredients IngredientsNo);

private:
	CResultDataManager();
	~CResultDataManager();
	//コピーコンストラクタによるコピーを禁止する.
	CResultDataManager(const CResultDataManager& rhs) = delete;
	//代入演算子によるコピーを禁止する.
	CResultDataManager& operator=(CResultDataManager& rhs) = delete;

	//各カテゴリの獲得数からお気に入りカテゴリを判定する.
	static enIngredientCategory CalculateFavoriteCategory(const SPlayerResultData& Data);

	//参加プレイヤーのみを対象に、スコアが高い順に順位を計算する.
	void CalculateRanks();

private:
	SPlayerResultData	m_PlayerData[PLAYER_MAX];	//プレイヤー毎の結果データ.
	int					m_nPlayerCount;				//参加人数.
};
