#pragma once
#include "GameObject/UIObject/CUIObject.h"
#include "Player/CPlayer.h"
#include "GameScene/GameSceneGameMain/Ingredients/CIngredients.h"

/********************************************************************************
*	連打対決(ボタン連打対決)UIクラス.
*
*	同じ具材に複数のプレイヤーが同時に手を伸ばした時、
*	Aボタンを連打した回数で取り合いの決着をつけるためのミニゲームを管理する.
*
*	・対決が始まると「3・2・1」のカウントダウンを挟んでから連打を受け付け始める.
*	  (カウントダウン中は参加プレイヤーの行動をロックするが、連打はまだカウントしない).
*	・先に目標回数連打したプレイヤー、または制限時間切れの時点で
*	  最も多く連打していたプレイヤーが具材を獲得する.
*	・対決は最大BATTLE_SLOT_MAX組まで同時に進行できる
*	  (例:プレイヤー1と2が鍋の左側で、3と4が右側で同時に対決するようなケース).
*
*	食材の取り合い判定そのものはCCollisionManagerが行い、
*	このクラスはStartBattleで渡された参加者の入力を集計して勝敗を決めるだけに専念する.
**/
class CButtonMashBattleUI
	:public CUIObject
{
public:
	static const int GAUGE_MAX = 4;			//プレイヤーの最大数(=ゲージの最大本数).
	static const int BATTLE_SLOT_MAX = 2;		//同時に進行できる対決の組数.
	static const int MASH_GOAL = 10;			//この回数を先に連打したプレイヤーが即座に勝利する.
	static const int BATTLE_TIME_LIMIT = 180;	//連打対決本編の制限時間(60fps換算で3秒。カウントダウン終了後から数える).

	static const int COUNTDOWN_STEP_FRAMES = 30;					//カウントダウンの数字1つ分の表示フレーム数(60fps換算で0.5秒).
	static const int COUNTDOWN_TOTAL_FRAMES = COUNTDOWN_STEP_FRAMES * 3;	//「3」「2」「1」で3つ分.
	static const int GO_FLASH_FRAMES = 15;							//カウントダウン終了直後、ゲージを光らせる(GO演出)フレーム数.

public:
	CButtonMashBattleUI();
	~CButtonMashBattleUI()override;

	void Update()override;
	void Draw()override;

	//対決中の具材の画面上の位置を更新する(ゲージを具材の真上に表示するため).
	//3D座標→2D座標への変換にカメラ行列が必要なので、描画の直前に外部から呼んでもらう.
	void UpdateScreenPosition(D3DXMATRIX& View, D3DXMATRIX& Proj);

	//連打対決の初期化(進行中の対決を全て強制リセットする).
	void InitMashBattle();

	//新しく対決を開始できる空きスロットがあるかどうか.
	bool CanStartBattle() const;
	//いずれかの対決が進行中(カウントダウン含む)かどうか.
	bool IsBattle() const;

	//連打対決を開始する(空いているスロットを自動で選んで使用する).
	//pTarget	:取り合いになっている具材.
	//pContenders:対決に参加させるプレイヤーの配列(要素数はEntryCount).
	//EntryCount:参加人数(2人以上).
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
		bool IsActive = false;		//このスロットが使用中(カウントダウン中〜対決中)かどうか.
		bool IsCountdown = false;	//カウントダウン中(まだ連打を受け付けない)かどうか.
		int CountdownTimer = 0;	//カウントダウンの残りフレーム数.
		int BattleTimer = 0;		//連打対決本編の残り制限フレーム数.
		int GoFlashTimer = 0;		//カウントダウン終了直後のGO演出用の残りフレーム数(見た目のみ).

		CIngredients*	pTargetIngredients = nullptr;	//取り合いになっている具材.
		CPlayer*		pEntries[GAUGE_MAX] = {};		//対決参加者(添字=プレイヤー番号。不参加はnullptr).
		int				MashCount[GAUGE_MAX] = {};		//各プレイヤーの連打回数.
		D3DXVECTOR3		ScreenPos = {};					//対象具材を2D画面座標に変換した位置(ゲージをその上に表示するため).
	};

	BattleSlot m_Slots[BATTLE_SLOT_MAX];	//同時に進行できる対決のスロット.

private:
	//1つのスロットの更新(カウントダウン進行 or 連打集計・勝敗判定)を行う.
	void UpdateSlot(BattleSlot& Slot);
	//決着をつけて後片付けをする(勝者に具材を渡し、参加者のロックを解除してスロットを空ける).
	void EndSlot(BattleSlot& Slot, int WinnerNo);
	//スロットの内容を未使用状態に戻す.
	void ResetSlot(BattleSlot& Slot);

	//指定したプレイヤーが今どのスロットで対決中か調べる。対決中でなければ-1を返す.
	int FindSlotIndexForPlayer(int PlayerNo) const;

	//指定したプレイヤー番号のゲージ(またはカウントダウン数字)を描画する.
	void RenderGauge(int PlayerNo);
	//カウントダウン中の数字(3・2・1)を、スロットにつき1つだけ代表して描画する.
	void RenderCountdownNumber(const BattleSlot& Slot);
	//同じスロット内で、指定したプレイヤーが何番目(0始まり)の参加者かを求める.
	//複数人のゲージを具材の真上に縦に積み重ねて表示するために使う.
	int GetLocalRank(const BattleSlot& Slot, int PlayerNo) const;

private:
	CSprite2D* m_pGaugeImg[GAUGE_MAX];	//ゲージ画像(プレイヤー番号ごとに色違いのパターンを持つ1枚絵).
	CSprite2D* m_pCountdownDigit;		//カウントダウンの数字表示用(0〜9の数字パターンを持つ1枚絵を流用).
};
