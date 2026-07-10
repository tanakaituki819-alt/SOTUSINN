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

	void Draw()override;
	void RightHand();
	void LeftHand();

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }

private:
	CSprite2D*	m_pRightHand[hand_Max];
	CSprite2D*	m_pLeftHand[hand_Max];
	CXInput*	m_pController[hand_Max];
};
