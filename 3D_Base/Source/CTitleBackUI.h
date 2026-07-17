#pragma once
#include "CUIObject.h"

class CTitleBackUI
	: public CUIObject
{
public:
	static const int Text_Max = 3;
	static const int Controller_Max = 4;
public:
	CTitleBackUI();
	~CTitleBackUI()override;

	void Update()override;
	void Draw()override;

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pController[i] = Input; }

private:
	CSprite2D*	m_pWhiteBack;
	CSprite2D*	m_pText[Text_Max];
	CSprite2D*	m_pTextFrame[Text_Max];
	CXInput*	m_pController[Controller_Max];
	bool		m_TitleBackflag;
};
