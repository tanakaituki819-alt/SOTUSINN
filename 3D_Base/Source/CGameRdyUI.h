#pragma once
#include "CUIObject.h"

class CGameRdyUI
	: public CUIObject
{
public:
	static const int hand_Max = 4;

public:
	CGameRdyUI();
	~CGameRdyUI()override;

	void Update()override;
	void Draw()override;
	void RightHand();
	void LeftHand();
	void GameRdy();

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }
	//準備完了キャンセル状態か？.
	bool GetHandclapflag(int i) { return m_Handclapflag[i]; }

private:
	CSprite2D*	m_pRightHand[hand_Max];
	CSprite2D*	m_pLeftHand[hand_Max];
	CSprite2D*	m_pGamerdy[hand_Max];
	CXInput*	m_pController[hand_Max];
	float		m_RightHandclap[hand_Max];
	float		m_LeftHandclap[hand_Max];
	bool		m_Handclapflag[hand_Max];
};
