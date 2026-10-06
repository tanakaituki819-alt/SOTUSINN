#include "CGameDataManager.h"

CGameDataManager::CGameDataManager()
	: m_PlayerData()
{
	ResetGameData();
}

CGameDataManager::~CGameDataManager()
{
}

//全プレイヤーのデータを初期化する.
void CGameDataManager::ResetGameData()
{
	for (int i = 0; i < Player_Max; i++) {
		m_PlayerData[i] = SPlayerData();
		m_PlayerData[i].m_nPlayerIndex = i;
	}
}

//プレイヤーの結果を保存する.
void CGameDataManager::SetPlayerResult(int No, bool IsJoined, int Score, const std::list<Ingredients>& IngredientsList)
{
	if (!IsValidNo(No)) return;

	SPlayerData& Data = m_PlayerData[No];
	Data = SPlayerData();	//一度初期化してから詰め直す(何度呼ばれても同じ結果になるように).
	Data.m_nPlayerIndex = No;
	Data.m_IsJoined		= IsJoined;
	Data.m_nScore		= Score;

	//食材の種類ごとに数える.
	for (Ingredients Kind : IngredientsList) {
		int Index = static_cast<int>(Kind);
		if (Index < 0 || IngredientsKind_Max <= Index) continue;
		Data.m_IngredientsCount[Index]++;
		Data.m_nTotalCount++;
	}
}

//参加プレイヤーの得点から順位を計算する(同点は同順位).
void CGameDataManager::CalcRank()
{
	for (int i = 0; i < Player_Max; i++) {
		if (!m_PlayerData[i].m_IsJoined) continue;

		int Rank = 1;
		//自分より得点の高い参加者の数+1が順位.
		for (int j = 0; j < Player_Max; j++) {
			if (!m_PlayerData[j].m_IsJoined) continue;
			if (m_PlayerData[j].m_nScore > m_PlayerData[i].m_nScore) {
				Rank++;
			}
		}
		m_PlayerData[i].m_nRank = Rank;
	}
}

//プレイヤーのデータを取得.
const SPlayerData& CGameDataManager::GetPlayerData(int No) const
{
	if (!IsValidNo(No)) {
		static const SPlayerData s_Empty;	//範囲外は空データを返す.
		return s_Empty;
	}
	return m_PlayerData[No];
}

//参加していたプレイヤー数を取得.
int CGameDataManager::GetPlayerCount() const
{
	int Count = 0;
	for (const SPlayerData& Data : m_PlayerData) {
		if (Data.m_IsJoined) Count++;
	}
	return Count;
}

//1位のプレイヤー番号を取得.
int CGameDataManager::GetWinnerPlayerIndex() const
{
	int Winner = -1;
	for (int i = 0; i < Player_Max; i++) {
		if (!m_PlayerData[i].m_IsJoined) continue;
		if (Winner < 0 || m_PlayerData[i].m_nScore > m_PlayerData[Winner].m_nScore) {
			Winner = i;
		}
	}
	return Winner;
}
