#pragma once
#include "GameObject/UIObject/CUIObject.h"

//準備画面のUI.
class CGameRdyUI
	: public CUIObject
{
public:
	static const int hand_Max = 4;	//プレイヤーの最大数(1人につき左右の手が1組).
	static const int Text_Max = 3;	//タイトルに戻る画面のテキスト数.
	static const int Frame_Max = 2;	//タイトルに戻る画面の選択肢の枠の数.

public:
	CGameRdyUI();
	~CGameRdyUI() override;

	void Update() override;
	void Draw() override;

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }
	//ゲームメインへ進む際のフラグ.
	bool GetMainSceneChangeflag() const { return m_GameMainSceneChangeflag; }
	//タイトルに戻る際のフラグ.
	bool GetTitleSceneChangeflag() const { return m_TitleSceneChangeflag; }

private:
	//更新
	void UpdateTitleBack();					//タイトルに戻る画面での選択と決定.
	bool TryOpenTitleBack();				//タイトルに戻る画面を開く(開いたらtrue).
	void UpdateReadyInput();				//各プレイヤーの準備完了・解除と、手の動き.
	void UpdateAllReady();					//全員準備完了のときの演出とゲーム開始.
	void UpdateConnection();				//接続数を数え、全員準備完了かを判定する.

	//描画
	void DrawHands();						//準備完了したプレイヤーの手.
	void DrawReadyTexts();					//手を合わせ終わったプレイヤーの「準備完了」.
	void DrawTitleBack();					//タイトルに戻る確認画面.
	void DrawAllReady();					//全員準備完了の表示.

	//補助
	bool IsConnected(int player) const;		//コントローラーが接続されているか.
	bool IsClapFinished(int player) const;	//手を合わせ終わったか.

private:
	CSprite2D* m_pHand		= nullptr;		//手(右手と左手で共通).
	CSprite2D* m_pText		= nullptr;		//テキスト(準備完了・タイトルに戻る画面で共通).
	CSprite2D* m_pTextFrame = nullptr;		//選択肢の枠.
	CSprite2D* m_pSelectionFrame = nullptr;	//今選んでいる選択肢を示す枠.
	CSprite2D* m_pWhiteBack = nullptr;		//白い背景(背景ぼかし用).
	CSprite2D* m_pScroll	= nullptr;		//巻物.
	CSprite2D* m_pAllReady	= nullptr;		//全員準備完了のときの赤い巻物.

	CXInput* m_pController[hand_Max] = {};	//コントローラー(借りているだけ).

	//準備完了
	bool	m_IsReady[hand_Max]		 = {};		//準備完了しているか.
	float	m_ClapOffset[hand_Max]	 = {};		//手を合わせる移動量(右手は左へ、左手は右へ同じだけ動く).
	bool	m_WasConnected[hand_Max] = {};		//接続済みか(接続SEを1回だけ鳴らすため).
	bool	m_IsAllReady			 = false;	//2人以上が接続し、その全員が準備完了か.
	bool	m_AllReadySEPlayed		 = false;	//全員準備完了のSEを鳴らし済みか.

	//タイトルに戻る画面
	bool	m_TitleBackflag = false;	//タイトルに戻る画面を出しているか.
	bool	m_IsYesSelected = false;	//「はい」を選んでいるか(falseなら「いいえ」).
	int		m_SelectCooldown = 0;		//選択を切り替えられるようになるまでの残りフレーム.

	//シーンチェンジ
	bool	m_GameMainSceneChangeflag	= false;	//ゲームメインへ進むか.
	bool	m_TitleSceneChangeflag		= false;	//タイトルに戻るか.
};