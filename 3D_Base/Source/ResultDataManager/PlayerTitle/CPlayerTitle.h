#pragma once
#include <vector>
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"

//前方宣言.
struct SPlayerResultData;
enum class enIngredientCategory;


//	食材の種類.
enum class enFoodKind
{
	None = -1,	//食材ではない.

	//普通の食材.
	Ninzin,		//人参.
	Daikon,		//大根.
	Hakusai,	//白菜.
	Negi,		//ねぎ.
	Touhu,		//豆腐.
	Enoki,		//エノキ.
	Sitake,		//シイタケ.
	Niku,		//豚肉.
	Uinner,		//ウインナー.
	Tara,		//タラ.

	//高級食材.
	Kani,		//カニ.
	Kumanote,	//クマの手.
	Robuster,	//ロブスター.
	Sakana,		//リュウグウノツカイ.
	Tai,		//タイ.

	MAX,
	NORMAL_MAX = Kani,	//普通の食材の数.
};


//	称号のレア度.
enum class enTitleRank
{
	None = -1,	//称号なし.
	Bronze,		//銅
	Silver,		//銀  
	Gold,		//金 
	Rainbow,	//虹
	MAX,
};

//	称号ごとの画像の行番号.
//銅.
enum class enBronzeTitle
{
	LikeTop		= 0,	//〇〇好き.
	Kizoku		= 15,	//貴族.
	Vegetarian,			//ベジタリアン.
	KinokoHa,			//キノコ派.
	Nikushokukei,		//肉食系.
};
//銀.
enum class enSilverTitle
{
	HashiMeijin	= 0,	//箸さばき名人.
	Ohitoyoshi,			//お人好し.
	Shokugo,			//食後です.
};
//金.
enum class enGoldTitle
{
	NabeBugyou	= 0,	//鍋奉行.
	Sennin,				//仙人.
	Okosama,			//お子様.
	TakenokoHa,			//タケノコ派.
	Diet,				//ダイエット中.
	OnlyTop,			//〇〇一筋.
};
//虹.
enum class enRainbowTitle
{
	NabeShougun = 0,	//鍋将軍.
};

//	称号.
struct STitle
{
	enTitleRank	Rank;		//レア度.
	int			PatternNo;	//画像の行番号.

	STitle()								: Rank(enTitleRank::None),		PatternNo(0) {}
	explicit STitle(enBronzeTitle Title)	: Rank(enTitleRank::Bronze),	PatternNo(static_cast<int>(Title)) {}
	explicit STitle(enSilverTitle Title)	: Rank(enTitleRank::Silver),	PatternNo(static_cast<int>(Title)) {}
	explicit STitle(enGoldTitle Title)		: Rank(enTitleRank::Gold),		PatternNo(static_cast<int>(Title)) {}
	explicit STitle(enRainbowTitle Title)	: Rank(enTitleRank::Rainbow),	PatternNo(static_cast<int>(Title)) {}

	bool operator==(const STitle& rhs) const { return Rank == rhs.Rank && PatternNo == rhs.PatternNo; }
	bool operator!=(const STitle& rhs) const { return !(*this == rhs); }
};

/************************************************************
*	ゲームの結果からプレイヤーの称号を決めるクラス.
**/
class CPlayerTitle
{
public:
	static const int MASH_WIN_COUNT		= 5;	//箸さばき名人になる連打対決の勝利数.
	static const int MASH_LOSE_COUNT	= 5;	//お人好しになる連打対決の敗北数.
	static const int SHOKUGO_EAT_COUNT	= 5;	//食後ですになる食べた数.

public:
	//全プレイヤーの称号を決めて、それぞれのm_Titleへ
	static void DecideTitles(SPlayerResultData Data[], int PlayerMax);

	//Ingredientsから食材の種類に変換する.
	static enFoodKind ToFoodKind(Ingredients IngredientsNo);

private:
	//称号の候補を探す.
	static std::vector<STitle> FindCandidates(const SPlayerResultData& Data);

	//レア度ごとの判定.
	static bool JudgeRainbow(const SPlayerResultData& Data, STitle* pOut);
	static bool JudgeGold(const SPlayerResultData& Data, STitle* pOut);
	static bool JudgeSilver(const SPlayerResultData& Data, STitle* pOut);
	//銅はそのまま返す.
	static std::vector<STitle> JudgeBronze(const SPlayerResultData& Data);

	//候補の中から他のプレイヤーと被らない称号を選ぶ(全部被っていれば候補全体からランダム).
	static STitle ChooseUnusedTitle(const std::vector<STitle>& Candidates,
		const SPlayerResultData Data[], int PlayerMax, int MyIndex);

	//食材ごとの称号を作る.
	static STitle LikeTitle(enFoodKind Kind);	//〇〇好き.
	static STitle OnlyTitle(enFoodKind Kind);	//〇〇一筋.

	//〇〇系しか食べない.
	static STitle CategoryTitle(enIngredientCategory Category);

	//食べた食材の合計数.
	static int GetEatCount(const SPlayerResultData& Data);

	//全種類の食材を全部1個以上、同じ数だけ食べたかどうか.
	static bool IsAllSameCount(const SPlayerResultData& Data, enFoodKind First, enFoodKind End);

	//食材の種類の系統を取得する.
	static enIngredientCategory GetCategory(enFoodKind Kind);
};
