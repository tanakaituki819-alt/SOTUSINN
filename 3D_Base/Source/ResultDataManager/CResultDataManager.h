#pragma once
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"	//Ingredients列挙体を使うため.

//========================================================
//	リザルト画面用のプレイヤー結果データ構造体.
//========================================================
struct SPlayerResultData
{
	bool	m_bActive;			//このプレイヤーが参加しているか(コントローラー接続済みか).
	int		m_nScore;			//合計スコア.
	int		m_nMeatCount;		//獲得した肉系食材の数.
	int		m_nVeggieCount;		//獲得した野菜系食材の数.
	int		m_nSpecialCount;	//獲得した高級食材の数.
	int		m_nRank;			//順位(1位〜、非参加なら0).

	//初期化.
	SPlayerResultData()
		: m_bActive			(false)
		, m_nScore			(0)
		, m_nMeatCount		(0)
		, m_nVeggieCount	(0)
		, m_nSpecialCount	(0)
		, m_nRank			(0)
	{
	}
};

/************************************************************
*	リザルト画面へ渡すデータをまとめるクラス(シングルトン).
*
*	【使い方】
*	ゲームメイン終了〜勝利者リザルトへ遷移するタイミングで、
*	一度だけ CaptureFromPlayers() を呼んでスナップショットを取る.
*	(例: CGameSceneGameMain::Update()内、WinnnerResultへSenenChangする直前)
*
*		CResultDataManager::GetInstance()->CaptureFromPlayers(m_pPlayer);
*
*	※参加人数は、各CPlayerのGetConnected()(コントローラー接続判定)を見て
*	  このクラス自身が自動的に数える。外部で参加人数を管理する必要はない.
*
*	リザルト側(CResultUI/CWinnerResultUI)は、以降は
*	CResultDataManager::GetInstance()->GetPlayerData(i) を読むだけでよい.
*	(m_bActiveがfalseのプレイヤー番号は非参加として無視すること)
**/
class CResultDataManager
{
public:
	static const int PLAYER_MAX = 4;	//プレイヤー最大数.

public:
	//インスタンス取得(唯一のアクセス経路).
	static CResultDataManager* GetInstance()
	{
		//唯一のインスタンスを作成する.
		static CResultDataManager s_Instance;
		return &s_Instance;
	}

	//CPlayerの配列(要素数PLAYER_MAX)からデータを取得してスナップショット化する.
	//参加しているかどうか(m_bActive)は、各プレイヤーのGetConnected()から自動判定する.
	//	pPlayer : プレイヤーの配列(nullptrが混ざっていても良い).
	void CaptureFromPlayers(CPlayer* pPlayer[]);

	//参加人数を取得(接続されているプレイヤーの数).
	int GetPlayerCount() const { return m_nPlayerCount; }

	//指定したプレイヤーの結果データを取得.
	const SPlayerResultData& GetPlayerData(int PlayerIndex) const;

	//1位のプレイヤー番号を取得(同着の場合は最も若い番号を返す)．非参加のみの場合は0.
	int GetWinnerPlayerIndex() const;

	//1位で同着になっているプレイヤーをビットで表す(bit0=1P, bit1=2P, bit2=3P, bit3=4P).
	//※CWinnerResultUI側で「誰と誰が同着1位か」の表示パターン分岐に使う想定.
	int GetTiedFirstPlaceMask() const;

private:
	CResultDataManager();
	~CResultDataManager();
	//コピーコンストラクタによるコピーを禁止する.
	CResultDataManager(const CResultDataManager& rhs) = delete;
	//代入演算子によるコピーを禁止する.
	CResultDataManager& operator=(CResultDataManager& rhs) = delete;

	//食材の種類から「肉/野菜/高級食材」のどれに数えるかを判定する.
	//※分類の基準を変えたい場合はCResultDataManager.cpp側のこの関数を書き換えればよい.
	enum class enCategory { Meat, Veggie, Special };
	static enCategory GetCategory(Ingredients IngredientsNo);

	//参加プレイヤーのみを対象に、スコアが高い順に順位を計算する(同点は番号が若い方を上位とする).
	void CalculateRanks();

private:
	SPlayerResultData	m_PlayerData[PLAYER_MAX];	//プレイヤー毎の結果データ.
	int					m_nPlayerCount;				//参加人数(接続数から自動算出).
};
