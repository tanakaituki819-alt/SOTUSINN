#pragma once
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"	//Ingredients列挙体を使うため.
#include "ResultDataManager/PlayerTitle/CPlayerTitle.h"				//称号を使うため.

//	食材のカテゴリ.
enum class enIngredientCategory
{
	None,		//未分類(何も取っていない).
	Meat,		//肉系(肉、ウインナー、タラ).
	Veggie,		//野菜系(にんじん、大根、白菜、ねぎ、豆腐).
	Mushroom,	//きのこ系(エノキ、シイタケ).
	Special,	//高級食材(カニ、クマの手、ロブスター、リュウグウノツカイ、タイ).
};

//	リザルト画面用のプレイヤー結果データ構造体.
struct SPlayerResultData
{
	bool	m_bActive;			//このプレイヤーが参加しているか.
	int		m_nScore;			//合計スコア.
	int		m_nIngredientScore;	//具材ポイント.
	int		m_nSpecialScore;	//高級具材ポイント.
	int		m_nFavoriteScore;	//お気に入り食材で獲得したスコア.
	int		m_nMeatCount;		//獲得した肉系食材の数.
	int		m_nVeggieCount;		//獲得した野菜系食材の数.
	int		m_nMushroomCount;	//獲得したきのこ系食材の数.
	int		m_nSpecialCount;	//獲得した高級食材の数.
	int		m_nFoodKindCount[static_cast<int>(enFoodKind::MAX)];	//食材の種類ごとの獲得数.
	int		m_nMashWinCount;	//連打対決で勝った回数.
	int		m_nMashLoseCount;	//連打対決で負けた回数.
	int		m_nRank;			//順位.
	enIngredientCategory	m_FavoriteCategory;	//お気に入りカテゴリ.
	Ingredients				m_FavoriteIngredient;	//お気に入りカテゴリの中で一番多く取った食材.
	STitle					m_Title;			//称号.

	//初期化.
	SPlayerResultData()
		: m_bActive			(false)
		, m_nScore			()
		, m_nIngredientScore()
		, m_nSpecialScore	()
		, m_nFavoriteScore	()
		, m_nMeatCount		()
		, m_nVeggieCount	()
		, m_nMushroomCount	()
		, m_nSpecialCount	()
		, m_nFoodKindCount	()
		, m_nMashWinCount	()
		, m_nMashLoseCount	()
		, m_nRank			()
		, m_FavoriteCategory(enIngredientCategory::None)
		, m_FavoriteIngredient(Ingredients::none)
		, m_Title			()
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
	static const int SCORE_MAX = CPlayer::SCORE_MAX;	//スコアの上限.

public:
	//インスタンス取得.
	static CResultDataManager* GetInstance()
	{
		//インスタンスを作成.
		static CResultDataManager s_Instance;
		return &s_Instance;
	}

	//CPlayerの配列からデータを取得してスナップショット化する.
	void CaptureFromPlayers(CPlayer* pPlayer[]);

	//参加人数を取得.
	int GetPlayerCount() const { return m_nPlayerCount; }

	//指定したプレイヤーの結果データを取得.
	const SPlayerResultData& GetPlayerData(int PlayerIndex) const;

	//1位のプレイヤー番号を取得
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

	//スコアを上限で止める.
	static int ClampScore(int Score);

	//各カテゴリの獲得数からお気に入りカテゴリを判定する.
	static enIngredientCategory CalculateFavoriteCategory(const SPlayerResultData& Data);

	//参加プレイヤーのみを対象に、スコアが高い順に順位を計算する.
	void CalculateRanks();

private:
	SPlayerResultData	m_PlayerData[PLAYER_MAX];	//プレイヤー毎の結果データ.
	int					m_nPlayerCount;				//参加人数.
};
