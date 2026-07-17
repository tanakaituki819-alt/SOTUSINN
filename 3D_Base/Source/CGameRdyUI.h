#pragma once
#include "CUIObject.h"

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
	//右手.
	void RightHand();
	//左手.
	void LeftHand();
	//準備完了.
	void GameRdy();
	//タイトルに戻るUI.
	void TitleBack();

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }

private:
	CSprite2D*	m_pRightHand	[hand_Max];	//右手.
	CSprite2D*	m_pLeftHand		[hand_Max];	//左手.
	CSprite2D*	m_pGamerdy		[hand_Max];	//準備完了UI.
	CSprite2D*	m_pWhiteBack;				//白い背景(背景ぼかし用).
	CSprite2D*	m_pTitleBackText[Text_Max];	//タイトルテキスト.
	CSprite2D*	m_pScroll;					//巻物.
	CSprite2D*  m_pTextFrame	[Text_Max];	//テキストのフレーム.
	CSprite2D*  m_pSelectionFrame;			//セレクトフレーム(今どこを選んでいるかわかりやすくするためのフレーム).
	CXInput*	m_pController	[hand_Max];	//コントローラー.
	float		m_RightHandclap	[hand_Max];	//右手の移動する数値.
	float		m_LeftHandclap	[hand_Max];	//左手の移動する数値.
	int			m_SelectchangeTimer;		//セレクトフレームを変更で期までのタイマー.
	bool		m_Handclapflag	[hand_Max];	//手を合わせ終わったかのフラグ.
	bool		m_TitleBackflag;			//タイトルに戻る描画を出すかのフラグ.
	bool		m_IsYesSelected;			//(はい)、か、(いいえ)どちらを選んでいるか判定する.
	bool		m_SelectTimeOver;			//セレクトフレームを動かしていいか.
};
