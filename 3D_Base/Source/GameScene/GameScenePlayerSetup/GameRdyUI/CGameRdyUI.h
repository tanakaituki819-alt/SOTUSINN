#pragma once
#include "GameObject/UIObject/CUIObject.h"

class CGameRdyUI
	: public CUIObject
{
public:
	static const int hand_Max  = 4;	//手の最大数.
	static const int Text_Max  = 3;	//テキストの最大数.
	static const int Frame_Max = 2;	//フレームの最大数.

public:
	CGameRdyUI();
	~CGameRdyUI()override;

	void Update()override;
	void Draw()override;
	//タイトルに戻る画面での処理.
	void TitleBackUpdate();
	//準備完了処理.
	void GameRdyHandUpdate();
	//右手.
	void DrawRightHand();
	//左手.
	void DrawLeftHand();
	//準備完了.
	void DrawGameRdy();
	//タイトルに戻るUI.
	void DrawTitleBack();
	//全員が準備完了になったときに表示されるUI.
	void AllGameRdy();

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }
	//ゲームメインに戻る際のフラグ.
	bool GetMainSceneChangeflag() { return m_GameMainSceneChangeflag; }
	//タイトルに戻る際のフラグ.
	bool GetTitleSceneChangeflag() { return m_TitleSceneChangeflag; }

private:
	CSprite2D*  m_pWhiteBack				= nullptr;	//白い背景(背景ぼかし用).
	CSprite2D*  m_pSelectionFrame			= nullptr;	//セレクトフレーム(今どこを選んでいるかわかりやすくするためのフレーム).
	CSprite2D*  m_pScroll					= nullptr;	//巻物.
	CSprite2D*  m_pTitleBackText[Text_Max]	= {};		//タイトルテキスト.
	CSprite2D*  m_pTextFrame	[Text_Max]	= {};		//テキストのフレーム.
	CSprite2D*  m_pRightHand	[hand_Max]	= {};		//右手.
	CSprite2D*  m_pLeftHand		[hand_Max]	= {};		//左手.
	CSprite2D*  m_pGamerdy		[hand_Max]	= {};		//準備完了UI.
	CSprite2D*	m_pAllGameRdy				= nullptr;	//全員が準備完了になったときに表示されるUI.
	CXInput*	m_pController	[hand_Max]	= {};		//コントローラー.
	float		m_RightHandclap	[hand_Max]	= {};		//右手の移動する数値.
	float		m_LeftHandclap	[hand_Max]	= {};		//左手の移動する数値.
	int			m_Connectedcnt				= 0;		//コントローラーが何台接続されているかカウントする.
	int			m_Readycnt					= 0;		//何人準備完了しているかカウントする.
	bool		m_Handclapflag	[hand_Max]	= {};		//手を合わせ終わったかのフラグ.
	bool		m_TitleBackflag				= false;	//タイトルに戻る描画を出すかのフラグ.
	bool		m_IsYesSelected				= false;	//(はい)、か、(いいえ)どちらを選んでいるか判定する.
	bool		m_SelectTimeOver			= false;	//セレクトフレームを動かしていいか.
	bool		m_IsAllReady				= false;	//接続されている全員が準備完了か判定をとる.
	int			m_SelectchangeTimer			= 0;		//セレクトフレームを変更で期までのタイマー.
	bool		m_GameMainSceneChangeflag	= false;	//ゲームメインシーンチェンジ判定フラグ.
	bool		m_TitleSceneChangeflag		= false;	//タイトルシーンチェンジ判定フラグ.
};
