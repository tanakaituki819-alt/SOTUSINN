#pragma once
#include <array>
#include <list>
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"	//食材の種類(enum class Ingredients).

//食材の種類の数.
static constexpr int IngredientsKind_Max = static_cast<int>(Ingredients::MAX);

//プレイヤーごとのリザルトデータ構造体.
struct SPlayerData
{
	int		m_nPlayerIndex;	//プレイヤー番号(0～3).
	bool	m_IsJoined;		//ゲームに参加していたか(コントローラー接続中だったか).
	int		m_nScore;		//獲得した合計得点.
	int		m_nRank;		//順位(1～4). 同点は同順位.
	int		m_nTotalCount;	//獲得した食材の合計数.
	std::array<int, IngredientsKind_Max> m_IngredientsCount;	//食材の種類ごとの獲得数(添字はIngredients).

	//初期化.
	SPlayerData()
		: m_nPlayerIndex	( 0 )
		, m_IsJoined		( false )
		, m_nScore			( 0 )
		, m_nRank			( 1 )
		, m_nTotalCount		( 0 )
		, m_IngredientsCount()
	{
		m_IngredientsCount.fill(0);
	}

	//指定した食材の獲得数を取得.
	int GetIngredientsCount(Ingredients Kind) const
	{
		return m_IngredientsCount[static_cast<int>(Kind)];
	}
};

// =================================================================
// クラス: ゲームデータ管理クラス (シングルトン)
// -----------------------------------------------------------------
// 【概要】
//  シーンを跨いで「各プレイヤーの得点・取得した食材の種類と数」を
//  保持・共有するためのマネージャークラスです。
//
// 【使い方ガイド】
//  1. ゲームメイン画面 (CGameSceneGameMain) 側:
//     // ゲーム開始時にデータをリセット
//     CGameDataManager::GetInstance()->ResetGameData();
//
//     // ゲーム終了時(シーン切り替え時)にプレイヤーの結果を保存
//     CGameDataManager::GetInstance()->SetPlayerResult(No, IsJoined, Score, IngredientsList);
//     // 全員分保存したら順位を計算
//     CGameDataManager::GetInstance()->CalcRank();
//
//  2. 優勝者・リザルト画面 (CGameSceneWinnerResult / CGameSceneResult) 側:
//     // 各プレイヤーのデータの取得
//     const SPlayerData& data = CGameDataManager::GetInstance()->GetPlayerData(playerIndex);
//     int score = data.m_nScore;
//     int kani  = data.GetIngredientsCount(Ingredients::KANI);	//カニの獲得数.
//
//     // 1位（勝者）のプレイヤーインデックスを取得
//     int winnerIdx = CGameDataManager::GetInstance()->GetWinnerPlayerIndex();
// =================================================================
class CGameDataManager
{
public:
	static constexpr int Player_Max = 4;	//最大プレイヤー数.

public:
	//インスタンス取得(唯一のアクセス経路).
	static CGameDataManager* GetInstance()
	{
		static CGameDataManager s_Instance;
		return &s_Instance;
	}
	~CGameDataManager();

	//全プレイヤーのデータを初期化する.
	void ResetGameData();

	//プレイヤーの結果を保存する.
	//No:プレイヤー番号, IsJoined:参加していたか, Score:得点, IngredientsList:取得した食材のリスト.
	void SetPlayerResult(int No, bool IsJoined, int Score, const std::list<Ingredients>& IngredientsList);

	//参加プレイヤーの得点から順位を計算する(同点は同順位).
	void CalcRank();

	//プレイヤーのデータを取得.
	const SPlayerData& GetPlayerData(int No) const;

	//参加していたプレイヤー数を取得.
	int GetPlayerCount() const;

	//1位のプレイヤー番号を取得(同点の場合は番号の小さい方, 参加者がいなければ-1).
	int GetWinnerPlayerIndex() const;

private:
	//外部からコンストラクタへのアクセスを禁止する.
	CGameDataManager();
	//コピー・代入を禁止する.
	CGameDataManager(const CGameDataManager& rhs) = delete;
	CGameDataManager& operator=(const CGameDataManager& rhs) = delete;

	//範囲内のプレイヤー番号か.
	static bool IsValidNo(int No) { return 0 <= No && No < Player_Max; }

private:
	std::array<SPlayerData, Player_Max> m_PlayerData;	//プレイヤーごとのデータ.
};
