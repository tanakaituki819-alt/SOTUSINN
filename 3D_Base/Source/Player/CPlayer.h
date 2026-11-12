#pragma once


#include <list>

#include "Player/PlayerUI/CPlayerUI.h"
#include "Player/PlayerParalysisUI/CPlayerParalysisUI.h"
#include <GameScene/GameSceneGameMain/Ingredients/CIngredients.h>
#include <GameObject/StaticMeshObjObject/CStaticMeshObjObject.h>
#include <GameScene/GameSceneGameMain/IngredientsGetEffect/IngredientsGetEffectManager.h>

class CPlayer
	:public  CStaticMeshObjObject	//スタティックメッシュオブジェのオブジェクトを継承.
{
private:
	static constexpr float Speed = 0.1;		//スピード
	static const int Chopsticks_Max = 2;	//お箸描画最大数.

public:
	static const int SCORE_MAX = 999;		//スコアの上限.

public:
	CPlayer();
	~CPlayer();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
	//コントローラー情報.
	void SetXInput(CXInput* Input) { MyController = Input; }
	//コントローラー情報を取得.
	CXInput* GetXInput() const { return MyController; }
	//UIの描画.
	void DrawUI();
	//マヒ状態にする.
	void OnTouchRawIngredient();
	//プレイヤーNoセット.
	void SetPlayerNo(int No) { PlayerNo = No; };
	//プレイヤーNo取得.
	int GetPlayerNo() const { return PlayerNo; }
	//今コントローラー接続されているかどうか.
	bool GetConnected() { return m_IsConnected; }
	//今回収中かどうか.
	bool GetIsCollecting() { return m_IsCollecting; }
	//マヒ状態かどうか.
	bool GetIsParalysis() const { return m_IsParalysis; }
	//連打対決中かどうか.
	bool GetIsMashBattle() const { return m_IsMashBattle; }
	//連打対決の参加/終了をセットする.
	void SetMashBattleLock(bool IsMashBattle) { m_IsMashBattle = IsMashBattle; }
	//連打対決で勝った回数を1増やす.
	void AddMashWin() { m_MashWinCount++; }
	//連打対決で負けた回数を1増やす.
	void AddMashLose() { m_MashLoseCount++; }
	//連打対決で勝った回数を取得.
	int GetMashWinCount() const { return m_MashWinCount; }
	//連打対決で負けた回数を取得.
	int GetMashLoseCount() const { return m_MashLoseCount; }
	//CBoundingSphere* GetTipBSphere();
	void IngredientsGetter(CIngredients* Ingredients);
	//具材回収中の処理.
	void IngredientsCollecting();

	void GetIngredients(CIngredients* YASAI);
	std::list<Ingredients>& GetAllIngredients() { return my_list; }

	//スコアを取得.
	int GetScore() const { return Score; }
	//指定した食材で獲得したスコアを取得.
	int GetIngredientScore(Ingredients No) const
	{
		if (No < Ingredients::none || No >= Ingredients::MAX) return 0;
		return m_IngredientScore[static_cast<int>(No)];
	}

	//スコアと獲得した具材のリスト、連打対決の勝敗数、カーソルの位置を初期化.
	void ResetGameData()
	{
		Score = 0;
		my_list.clear();
		for (auto& IngredientScore : m_IngredientScore) IngredientScore = 0;
		m_MashWinCount = 0;
		m_MashLoseCount = 0;
		m_Position = { 0.f, 0.f, 0.f };
		m_OldPosition = { 0.f, 0.f, 0.f };
		m_CousorPosition = { 0.f, 0.f, 0.f };
	}

	void SetIngredientsGetEffectManager(IngredientsGetEffectManager* IngredientsGetEffectManager) {
		m_IngredientsGetEffectManager = IngredientsGetEffectManager;
	}
private:
	//回収
	void UpdateLowerChopsticks();	//お箸を下ろす処理(具材をつかむまで、一定時間下げる).

	//マヒ
	void UpdateParalysis();	//マヒ中の処理(時間短縮・震え・ボタン案内UI).
	void EndParalysis();	//マヒを解除する.

private:
	D3DXVECTOR3	m_Pos;				//ポジション.
	CXInput* MyController;			//コントローラー.
	CSprite2D* m_Cousor;			//カーソル.
	CStaticMeshObjObject* m_pChopsticks[Chopsticks_Max];	//お箸(お箸を二つ描画するために配列化).
	std::list<Ingredients>	my_list;						//リスト.
	int						Score;							//スコア.
	int						m_IngredientScore[static_cast<int>(Ingredients::MAX)] = {};	//食材ごとの獲得スコア.
	CPlayerUI* P_UI;									//プレイヤーUI.
	int						PlayerNo;					//プレイヤーNo.
	bool					m_IsCollecting = false;		//今回収中かどうか.
	int						m_Collectingtime = 0;		//お箸を下ろし始めてからの経過フレーム.
	bool					m_IsConnected = false;		//コントローラー接続中か判定.
	bool					m_IsMashBattle = false;		//連打対決中かどうか.
	int						m_MashWinCount = 0;			//連打対決で勝った回数.
	int						m_MashLoseCount = 0;		//連打対決で負けた回数.
	D3DXVECTOR3				m_CousorPosition = {};		//カーソルのポジションを入れる.
	D3DXVECTOR3 m_OldPosition;
	CIngredients* m_pIngredients = nullptr;
	int						m_Status = 1;					//プレイヤーの状態を入れる変数(0:笑顔 1:デフォルト 3:マヒ中 ).
	float					m_SmileTimer = 0;				//笑顔の時間を計るタイマー.	
	IngredientsGetEffectManager* m_IngredientsGetEffectManager = nullptr;//エフェクトのマネージャー

	//マヒ
	CPlayerParalysisUI		m_ParalysisUI;					//マヒ中のボタン案内UI(プレイヤーと一緒に作られ、一緒に消える).
	bool					m_IsParalysis		= false;	//マヒ中かどうか.
	int						m_ParalysisTimer	= 0;		//マヒの経過フレーム.
	float					m_ParalysisShakeY	= 0.f;		//今、震えで上下にずらしている量.
};