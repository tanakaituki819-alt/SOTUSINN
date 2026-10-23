#include "CPlayerTitle.h"
#include "ResultDataManager/CResultDataManager.h"
#include <cstdlib>	//rand用.

namespace
{
	//食材の種類ごとのIngredients(enFoodKindと同じ順).
	//白菜はゲームで使っているHAKUSEI2だけを扱う(HAKUSAI1は読み込まれているだけ).
	const Ingredients FOOD_KIND_INGREDIENTS[static_cast<int>(enFoodKind::MAX)] =
	{
		Ingredients::NINZIN,	//人参.
		Ingredients::DAIKON,	//大根.
		Ingredients::HAKUSEI2,	//白菜.
		Ingredients::NEGI,		//ねぎ.
		Ingredients::TOUHU,		//豆腐.
		Ingredients::ENOKI,		//エノキ.
		Ingredients::SITAKE,	//シイタケ.
		Ingredients::NIKU,		//豚肉.
		Ingredients::UINNER,	//ウインナー.
		Ingredients::TARA,		//タラ.
		Ingredients::KANI,		//カニ.
		Ingredients::KUMANOTE,	//クマの手.
		Ingredients::ROBUSTER,	//ロブスター.
		Ingredients::SAKANA,	//リュウグウノツカイ.
		Ingredients::TAI,		//タイ.
	};

	//enFoodKindを配列の添字にする.
	int ToIndex(enFoodKind Kind) { return static_cast<int>(Kind); }
}

//全プレイヤーの称号を決めて、それぞれのm_Titleに入れる.
void CPlayerTitle::DecideTitles(SPlayerResultData Data[], int PlayerMax)
{
	std::vector<std::vector<STitle>> Candidates(PlayerMax);

	//候補が1つに決まるプレイヤーから先に称号を決める.
	for (int i = 0; i < PlayerMax; i++)
	{
		Data[i].m_Title = STitle();
		if (!Data[i].m_bActive) continue;

		Candidates[i] = FindCandidates(Data[i]);
		if (Candidates[i].size() == 1)
		{
			Data[i].m_Title = Candidates[i][0];
		}
	}

	//候補が複数あるプレイヤーは、ここまでに決まった称号と被らないものを選ぶ.
	for (int i = 0; i < PlayerMax; i++)
	{
		if (Candidates[i].size() < 2) continue;
		Data[i].m_Title = ChooseUnusedTitle(Candidates[i], Data, PlayerMax, i);
	}
}

//Ingredientsから食材の種類に変換する.
enFoodKind CPlayerTitle::ToFoodKind(Ingredients IngredientsNo)
{
	for (int Kind = 0; Kind < ToIndex(enFoodKind::MAX); Kind++)
	{
		if (FOOD_KIND_INGREDIENTS[Kind] == IngredientsNo) return static_cast<enFoodKind>(Kind);
	}
	return enFoodKind::None;
}

//称号の候補を探す.
std::vector<STitle> CPlayerTitle::FindCandidates(const SPlayerResultData& Data)
{
	STitle Title;

	//レア度の高い順に判定して、最初に当てはまったものにする.
	if (JudgeRainbow(Data, &Title))	return { Title };
	if (JudgeGold(Data, &Title))	return { Title };
	if (JudgeSilver(Data, &Title))	return { Title };
	return JudgeBronze(Data);
}

//虹の判定.
bool CPlayerTitle::JudgeRainbow(const SPlayerResultData& Data, STitle* pOut)
{
	//鍋将軍：高級食材も含めて全種類を同じ数だけ食べた.
	if (IsAllSameCount(Data, enFoodKind::Ninzin, enFoodKind::MAX))
	{
		*pOut = STitle(enRainbowTitle::NabeShougun);
		return true;
	}
	return false;
}

//金の判定.
bool CPlayerTitle::JudgeGold(const SPlayerResultData& Data, STitle* pOut)
{
	//鍋奉行：高級食材以外の全種類を同じ数だけ食べた.
	if (IsAllSameCount(Data, enFoodKind::Ninzin, enFoodKind::NORMAL_MAX))
	{
		*pOut = STitle(enGoldTitle::NabeBugyou);
		return true;
	}

	//仙人：何も食べなかった.
	if (GetEatCount(Data) == 0)
	{
		*pOut = STitle(enGoldTitle::Sennin);
		return true;
	}

	//〇〇系を食べない：野菜・きのこ・肉のうち、その系統だけ1個も食べなかった(高級食材は数えない).
	const bool IsEatVeggie		= Data.m_nVeggieCount > 0;
	const bool IsEatMushroom	= Data.m_nMushroomCount > 0;
	const bool IsEatMeat		= Data.m_nMeatCount > 0;
	if (!IsEatVeggie && IsEatMushroom && IsEatMeat)
	{
		*pOut = STitle(enGoldTitle::Okosama);
		return true;
	}
	if (IsEatVeggie && !IsEatMushroom && IsEatMeat)
	{
		*pOut = STitle(enGoldTitle::TakenokoHa);
		return true;
	}
	if (IsEatVeggie && IsEatMushroom && !IsEatMeat)
	{
		*pOut = STitle(enGoldTitle::Diet);
		return true;
	}

	//〇〇一筋：高級食材以外は1種類しか食べなかった.
	int EatKindNum = 0;
	enFoodKind EatKind = enFoodKind::None;
	for (int Kind = 0; Kind < ToIndex(enFoodKind::NORMAL_MAX); Kind++)
	{
		if (Data.m_nFoodKindCount[Kind] > 0)
		{
			EatKindNum++;
			EatKind = static_cast<enFoodKind>(Kind);
		}
	}
	if (EatKindNum == 1)
	{
		*pOut = OnlyTitle(EatKind);
		return true;
	}

	return false;
}

//銀の判定.
bool CPlayerTitle::JudgeSilver(const SPlayerResultData& Data, STitle* pOut)
{
	//箸さばき名人：連打対決で規定回数以上勝った.
	if (Data.m_nMashWinCount >= MASH_WIN_COUNT)
	{
		*pOut = STitle(enSilverTitle::HashiMeijin);
		return true;
	}

	//お人好し：連打対決で規定回数以上負けた.
	if (Data.m_nMashLoseCount >= MASH_LOSE_COUNT)
	{
		*pOut = STitle(enSilverTitle::Ohitoyoshi);
		return true;
	}

	//食後です：食べた数規が定以下.
	if (GetEatCount(Data) <= SHOKUGO_EAT_COUNT)
	{
		*pOut = STitle(enSilverTitle::Shokugo);
		return true;
	}

	return false;
}

//銅の判定.
std::vector<STitle> CPlayerTitle::JudgeBronze(const SPlayerResultData& Data)
{
	//〇〇系しか食べない：食べた系統が1つだけ.
	const struct { enIngredientCategory Category; int Count; } CategoryTable[] =
	{
		{ enIngredientCategory::Special,	Data.m_nSpecialCount	},
		{ enIngredientCategory::Veggie,		Data.m_nVeggieCount		},
		{ enIngredientCategory::Mushroom,	Data.m_nMushroomCount	},
		{ enIngredientCategory::Meat,		Data.m_nMeatCount		},
	};
	int EatCategoryNum = 0;
	enIngredientCategory EatCategory = enIngredientCategory::None;
	for (const auto& Entry : CategoryTable)
	{
		if (Entry.Count > 0)
		{
			EatCategoryNum++;
			EatCategory = Entry.Category;
		}
	}
	if (EatCategoryNum == 1)
	{
		return { CategoryTitle(EatCategory) };
	}

	//一番多く食べた食材を探す.
	int MaxCount = 0;
	std::vector<enFoodKind> MostKinds;
	for (int Kind = 0; Kind < ToIndex(enFoodKind::MAX); Kind++)
	{
		const int Count = Data.m_nFoodKindCount[Kind];
		if (Count == 0) continue;

		if (Count > MaxCount)
		{
			MaxCount = Count;
			MostKinds.clear();
		}
		if (Count == MaxCount)
		{
			MostKinds.push_back(static_cast<enFoodKind>(Kind));
		}
	}

	//何も食べていなければ候補なし.
	if (MostKinds.empty()) return {};

	//1種類だけなら〇〇好き.
	if (MostKinds.size() == 1)
	{
		return { LikeTitle(MostKinds[0]) };
	}

	//2種類以上が同じ系統なら、その系統の称号.
	const enIngredientCategory FirstCategory = GetCategory(MostKinds[0]);
	bool IsSameCategory = true;
	for (enFoodKind Kind : MostKinds)
	{
		if (GetCategory(Kind) != FirstCategory)
		{
			IsSameCategory = false;
			break;
		}
	}
	if (IsSameCategory)
	{
		return { CategoryTitle(FirstCategory) };
	}

	//別の系統が混ざっていれば、それぞれの〇〇好きを候補にする(他のプレイヤーと被らないものを後で選ぶ).
	std::vector<STitle> Candidates;
	for (enFoodKind Kind : MostKinds)
	{
		Candidates.push_back(LikeTitle(Kind));
	}
	return Candidates;
}

//候補の中から他のプレイヤーと被らない称号を選ぶ.
STitle CPlayerTitle::ChooseUnusedTitle(const std::vector<STitle>& Candidates,
	const SPlayerResultData Data[], int PlayerMax, int MyIndex)
{
	//他のプレイヤーがまだ持っていない候補を集める.
	std::vector<STitle> Unused;
	for (const STitle& Candidate : Candidates)
	{
		bool IsUsed = false;
		for (int i = 0; i < PlayerMax; i++)
		{
			if (i == MyIndex || !Data[i].m_bActive) continue;
			if (Data[i].m_Title == Candidate)
			{
				IsUsed = true;
				break;
			}
		}
		if (!IsUsed) Unused.push_back(Candidate);
	}

	//被らない候補があればその中から、全部被っていれば候補全体からランダムに選ぶ.
	const std::vector<STitle>& ChooseFrom = Unused.empty() ? Candidates : Unused;
	return ChooseFrom[rand() % ChooseFrom.size()];
}

//〇〇好き.
STitle CPlayerTitle::LikeTitle(enFoodKind Kind)
{
	STitle Title(enBronzeTitle::LikeTop);
	Title.PatternNo += ToIndex(Kind);
	return Title;
}

//〇〇一筋.
STitle CPlayerTitle::OnlyTitle(enFoodKind Kind)
{
	STitle Title(enGoldTitle::OnlyTop);
	Title.PatternNo += ToIndex(Kind);
	return Title;
}

//〇〇系しか食べない.
STitle CPlayerTitle::CategoryTitle(enIngredientCategory Category)
{
	switch (Category)
	{
	case enIngredientCategory::Special:		return STitle(enBronzeTitle::Kizoku);		//貴族.
	case enIngredientCategory::Veggie:		return STitle(enBronzeTitle::Vegetarian);	//ベジタリアン.
	case enIngredientCategory::Mushroom:	return STitle(enBronzeTitle::KinokoHa);		//キノコ派.
	case enIngredientCategory::Meat:		return STitle(enBronzeTitle::Nikushokukei);	//肉食系.
	default:								return STitle();
	}
}

//食べた食材の合計数.
int CPlayerTitle::GetEatCount(const SPlayerResultData& Data)
{
	int EatCount = 0;
	for (int Count : Data.m_nFoodKindCount)
	{
		EatCount += Count;
	}
	return EatCount;
}

//全部1個以上、同じ数だけ食べたかどうか.
bool CPlayerTitle::IsAllSameCount(const SPlayerResultData& Data, enFoodKind First, enFoodKind End)
{
	const int FirstCount = Data.m_nFoodKindCount[ToIndex(First)];
	if (FirstCount == 0) return false;

	for (int Kind = ToIndex(First) + 1; Kind < ToIndex(End); Kind++)
	{
		if (Data.m_nFoodKindCount[Kind] != FirstCount) return false;
	}
	return true;
}

//食材の種類の系統を取得する.
enIngredientCategory CPlayerTitle::GetCategory(enFoodKind Kind)
{
	if (Kind <= enFoodKind::None || Kind >= enFoodKind::MAX) return enIngredientCategory::None;
	return CResultDataManager::GetCategory(FOOD_KIND_INGREDIENTS[ToIndex(Kind)]);
}
