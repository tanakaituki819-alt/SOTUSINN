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

//食材の種類からカテゴリを判定.
enIngredientCategory CResultDataManager::GetCategory(Ingredients IngredientsNo)
{
	switch (IngredientsNo)
	{
	//高級食材.
	case Ingredients::KANI:		//カニ.
	case Ingredients::KUMANOTE:	//クマの手.
	case Ingredients::ROBUSTER:	//ロブスター.
	case Ingredients::SAKANA:	//リュウグウノツカイ.
	case Ingredients::TAI:		//タイ.
		return enIngredientCategory::Special;

	//肉系.
	case Ingredients::NIKU:		//肉.
	case Ingredients::UINNER:	//ウインナー.
		return enIngredientCategory::Meat;

	//魚系.
	case Ingredients::TARA:		//タラ.
		return enIngredientCategory::Fish;

	//野菜系.
	case Ingredients::NINZIN:	//にんじん.
	case Ingredients::DAIKON:	//大根.
	case Ingredients::HAKUSAI1:	//白菜.
	case Ingredients::HAKUSEI2:	//白菜.
	case Ingredients::NEGI:		//ねぎ.
		return enIngredientCategory::Veggie;

	//きのこ系.
	case Ingredients::ENOKI:	//エノキ.
	case Ingredients::SITAKE:	//シイタケ.
		return enIngredientCategory::Mushroom;

	//豆腐.
	case Ingredients::TOUHU:	//豆腐.
		return enIngredientCategory::Tofu;

	//none,MAXなど食材ではないもの.
	default:
		return enIngredientCategory::None;
	}
}

//各カテゴリの獲得数からお気に入りカテゴリを判定する（同数の場合は高級食材 > 肉 > 魚 > 野菜 > きのこ > 豆腐 の順）.
enIngredientCategory CResultDataManager::CalculateFavoriteCategory(const SPlayerResultData& Data)
{
	const struct { enIngredientCategory Category; int Count; } Table[] =
	{
		{ enIngredientCategory::Special,	Data.m_nSpecialCount	},
		{ enIngredientCategory::Meat,		Data.m_nMeatCount		},
		{ enIngredientCategory::Fish,		Data.m_nFishCount		},
		{ enIngredientCategory::Veggie,		Data.m_nVeggieCount		},
		{ enIngredientCategory::Mushroom,	Data.m_nMushroomCount	},
		{ enIngredientCategory::Tofu,		Data.m_nTofuCount		},
	};

	enIngredientCategory Favorite = enIngredientCategory::None;
	int MaxCount = 0;
	for (const auto& Entry : Table)
	{
		//同数なら先に見つかった方(優先度が高い方)を残す.
		if (Entry.Count > MaxCount)
		{
			MaxCount = Entry.Count;
			Favorite = Entry.Category;
		}
	}
	return Favorite;
}

//CPlayerの配列からスナップショットを取る.
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
			case enIngredientCategory::Meat:		m_PlayerData[i].m_nMeatCount++;		break;
			case enIngredientCategory::Fish:		m_PlayerData[i].m_nFishCount++;		break;
			case enIngredientCategory::Veggie:		m_PlayerData[i].m_nVeggieCount++;	break;
			case enIngredientCategory::Mushroom:	m_PlayerData[i].m_nMushroomCount++;	break;
			case enIngredientCategory::Tofu:		m_PlayerData[i].m_nTofuCount++;		break;
			case enIngredientCategory::Special:		m_PlayerData[i].m_nSpecialCount++;	break;
			default: break;
			}
		}

		//お気に入りカテゴリを判定.
		m_PlayerData[i].m_FavoriteCategory = CalculateFavoriteCategory(m_PlayerData[i]);
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

	//順位を設定.
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
