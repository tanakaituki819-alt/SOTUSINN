#pragma once
#include "CUIObject.h"

class CPlayerSetupUI
	: public CUIObject
{
public:
	//---------------------------
	//	定数宣言.
	//---------------------------
	static const int Ui_Max		= 4;
	static const int Text_Max	= 2;
public:
	CPlayerSetupUI();
	~CPlayerSetupUI()override;

	void Update()override;
	void Draw()override;

	//プレイヤー背景UI.
	void PlayerBackUI();
	//プレイヤーの名前.
	void PlayerNameUI();
	//プレイヤーのコントローラーUI.
	void ControllerUI();
	//テキストUI.
	void TextUI();
	//巻物UI.
	void ScrollUI();

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pMyController[i] = Input; }

private:
	CSprite2D*	m_pPlayerSetUpUI[Ui_Max];
	CSprite2D*	m_pPlayerNameUI	[Ui_Max];
	CSprite2D*	m_pPlayercontrollerUI[Ui_Max];
	CSprite2D*	m_pTextUI		[Text_Max];
	CSprite2D*	m_pScrollUI		[Text_Max];
	CXInput*	m_pMyController[Ui_Max];
};

