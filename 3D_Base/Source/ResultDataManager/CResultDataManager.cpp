#include "CResultDataManager.h"
#include <algorithm>	//std::sort用.

//コンストラクタ.
CResultDataManager::CResultDataManager()
	: m_nPlayerCount(0)
{
}

//デストラクタ.
CResultDataManager::~CResultDataManager()
{
}

//食材の種類からカテゴリを判定する.
//※CIngredientsmanager.cppのレア食材の括り(KANI/KUMANOTE/SAKANA/ROBUSTER)を
//  「高級食材」として流用している.
CResultDataManager::enCategory CResultDataManager::GetCategory(Ingredients IngredientsNo)
{
	switch (IngredientsNo)
	{
	//高級食材.
	case Ingredients::KANI:
	case Ingredients::KUMANOTE:
	case Ingredients::SAKANA:
	case Ingredients::ROBUSTER:
		return enCategory::Special;

	//肉系.
	case Ingredients::NIKU:
	case Ingredients::UINNER:
		return enCategory::Meat;

	//それ以外(野菜、豆腐、タイ、タラ等)は野菜系として扱う.
	//※分類を変えたい場合はここを調整すること.
	default:
		return enCategory::Veggie;
	}
}

//CPlayerの配列からスナップショットを取る.
//参加しているかどうかは、各プレイヤーのGetConnected()から自動判定する.
void CResultDataManager::CaptureFromPlayers(CPlayer* pPlayer[])
{
	m_nPlayerCount = 0;

	//プレイヤー最大数分.
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//データを初期化.
		m_PlayerData[i] = SPlayerResultData();

		//プレイヤーが存在しない、もしくはコントローラーが接続されていないなら非参加として次へ.
		if (pPlayer[i] == nullptr || !pPlayer[i]->GetConnected()) continue;

		//参加者として扱う.
		m_PlayerData[i].m_bActive = true;
		m_nPlayerCount++;

		//合計スコア.
		m_PlayerData[i].m_nScore = pPlayer[i]->GetScore();

		//獲得した食材のリストから内訳をカウントする.
		for (const auto& Item : pPlayer[i]->GetAllIngredients())
		{
			switch (GetCategory(Item))
			{
			case enCategory::Meat:		m_PlayerData[i].m_nMeatCount++;	break;
			case enCategory::Veggie:	m_PlayerData[i].m_nVeggieCount++;	break;
			case enCategory::Special:	m_PlayerData[i].m_nSpecialCount++;	break;
			}
		}
	}

	//順位を計算する.
	CalculateRanks();
}

//参加プレイヤーのみを対象に、スコアが高い順に順位を計算する.
void CResultDataManager::CalculateRanks()
{
	//参加しているプレイヤーのインデックスだけを集める.
	int Indices[PLAYER_MAX];
	int Num = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_PlayerData[i].m_bActive)
		{
			Indices[Num++] = i;
		}
	}

	//スコアが高い順(同点の場合はプレイヤー番号が若い順)にソート.
	std::sort(Indices, Indices + Num, [this](int a, int b)
		{
			if (m_PlayerData[a].m_nScore == m_PlayerData[b].m_nScore)
			{
				return a < b;
			}
			return m_PlayerData[a].m_nScore > m_PlayerData[b].m_nScore;
		});

	//順位を設定(1位〜).
	for (int Rank = 0; Rank < Num; Rank++)
	{
		int PlayerIndex = Indices[Rank];
		m_PlayerData[PlayerIndex].m_nRank = Rank + 1;
	}
}

//指定したプレイヤーの結果データを取得.
const SPlayerResultData& CResultDataManager::GetPlayerData(int PlayerIndex) const
{
	//範囲外アクセス防止の安全策.
	if (PlayerIndex < 0 || PlayerIndex >= PLAYER_MAX)
	{
		static SPlayerResultData Dummy;
		return Dummy;
	}
	return m_PlayerData[PlayerIndex];
}

//1位のプレイヤー番号を取得する.
int CResultDataManager::GetWinnerPlayerIndex() const
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_PlayerData[i].m_bActive && m_PlayerData[i].m_nRank == 1) return i;
	}
	return 0;
}

//1位で同着になっているプレイヤーをビットで返す.
int CResultDataManager::GetTiedFirstPlaceMask() const
{
	int Mask = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (m_PlayerData[i].m_bActive && m_PlayerData[i].m_nRank == 1)
		{
			Mask |= (1 << i);
		}
	}
	return Mask;
}
