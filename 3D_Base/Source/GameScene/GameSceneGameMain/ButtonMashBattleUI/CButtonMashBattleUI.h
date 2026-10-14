#pragma once
#include "GameObject/UIObject/CUIObject.h"
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"

/********************************************************************************
*	連打対決(ボタン連打対決)UIクラス.
**/
class CButtonMashBattleUI
	:public CUIObject
{
public:
	static const int GAUGE_MAX = 4;				//プレイヤーの最大数.
	static const int BATTLE_SLOT_MAX = 2;		//同時に進行できる対決の組数.
	static const int MASH_GOAL = 10;			//この回数を先に連打したプレイヤーが即座に勝利する.
	static const int BATTLE_TIME_LIMIT = 180;	//連打対決本編の制限時間.

	static const int COUNTDOWN_STEP_FRAMES = 30;					//カウントダウンの数字1つ分の表示フレーム数.
	static const int COUNTDOWN_TOTAL_FRAMES = COUNTDOWN_STEP_FRAMES * 3;
	static const int GO_FLASH_FRAMES = 15;							//カウントダウン終了直後、ゲージを光らせるフレーム数.

public:
	CButtonMashBattleUI();
	~CButtonMashBattleUI()override;

	void Update()override;
	void Draw()override;

	//対決中の具材の画面上の位置を更新.
	void UpdateScreenPosition(D3DXMATRIX& View, D3DXMATRIX& Proj);

	//連打対決の初期化(進行中の対決を全て強制リセットする).
	void InitMashBattle();

	//新しく対決を開始できる空きスロットがあるかどうか.
	bool CanStartBattle() const;
	//いずれかの対決が進行中かどうか.
	bool IsBattle() const;

	//連打対決を開始する.
	void StartBattle(CIngredients* pTarget, CPlayer* const pContenders[], int EntryCount);

	//プレイヤー壱のゲージの描画.
	void RenderPlayer1Gauge();
	//プレイヤー弐のゲージの描画.
	void RenderPlayer2Gauge();
	//プレイヤー参のゲージの描画.
	void RenderPlayer3Gauge();
	//プレイヤー肆のゲージの描画.
	void RenderPlayer4Gauge();

private:
	//1組の連打対決の情報(カウントダウン〜決着までをまとめて管理する).
	struct BattleSlot
	{
		bool IsActive = false;		//このスロットが使用中かどうか.
		bool IsCountdown = false;	//カウントダウン中かどうか.
		int CountdownTimer = 0;	//カウントダウンの残りフレーム数.
		int BattleTimer = 0;		//連打対決本編の残り制限フレーム数.
		int GoFlashTimer = 0;		//カウントダウン終了直後のGO演出用の残りフレーム数.

		CIngredients*	pTargetIngredients = nullptr;	//取り合いになっている具材.
		CPlayer*		pEntries[GAUGE_MAX] = {};		//対決参加者(添字=プレイヤー番号。不参加はnullptr).
		int				MashCount[GAUGE_MAX] = {};		//各プレイヤーの連打回数.
		D3DXVECTOR3		ScreenPos = {};					//対象具材を2D画面座標に変換した位置(ゲージをその上に表示するため).
	};

	BattleSlot m_Slots[BATTLE_SLOT_MAX];	//同時に進行できる対決のスロット.

private:
	//1つのスロットの更新を行う.
	void UpdateSlot(BattleSlot& Slot);
	//決着をつけて後片付けをする.
	void EndSlot(BattleSlot& Slot, int WinnerNo);
	//スロットの内容を未使用状態に戻す.
	void ResetSlot(BattleSlot& Slot);

	//指定したプレイヤーが今どのスロットで対決中か調べる。対決中でなければ-1を返す.
	int FindSlotIndexForPlayer(int PlayerNo) const;

	//指定したプレイヤー番号のゲージを描画する.
	void RenderGauge(int PlayerNo);
	//カウントダウン中の数字を、スロットにつき1つだけ描画する.
	void RenderCountdownNumber(const BattleSlot& Slot);
	//同じスロット内で、指定したプレイヤーが何番目の参加者かを求める.
	int GetLocalRank(const BattleSlot& Slot, int PlayerNo) const;

private:
	CSprite2D* m_pGaugeImg[GAUGE_MAX];	//ゲージ画像.
	CSprite2D* m_pCountdownDigit;		//カウントダウンの数字表示用.
};
