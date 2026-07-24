#pragma once
#include "CUIObject.h"

class CWinnerResultUI
	: public CUIObject
{
public:
	static const int Font_Max = 4;

public:
	CWinnerResultUI();
	~CWinnerResultUI()override;

	void Update()override;
	void Draw()override;

	//背景.
	void WinnerResultBackUI();
	//あっぱれ.
	void WinnerTextUI();
	//プレイヤー壱の立ち絵
	void Player1WinnerUI();
	//プレイヤー弐の立ち絵
	void Player2WinnerUI();
	//プレイヤー参の立ち絵
	void Player3WinnerUI();
	//プレイヤー肆の立ち絵
	void Player4WinnerUI();
	//プレイヤー壱とプレイヤー弐の立ち絵
	void Player1AndPlayer2WinnerUI();
	//プレイヤー壱とプレイヤー参の立ち絵
	void Player1AndPlayer3WinnerUI();
	//プレイヤー壱とプレイヤー肆の立ち絵
	void Player1AndPlayer4WinnerUI();
	//プレイヤー弐とプレイヤー参の立ち絵
	void Player2AndPlayer3WinnerUI();
	//プレイヤー弐とプレイヤー肆の立ち絵
	void Player2AndPlayer4WinnerUI();
	//プレイヤー参とプレイヤー肆の立ち絵
	void Player3AndPlayer4WinnerUI();
	// プレイヤー壱とプレイヤー弐とプレイヤー参の立ち絵
	void Player1AndPlayer2AndPlayer3WinnerUI();
	// プレイヤー壱とプレイヤー弐とプレイヤー肆の立ち絵
	void Player1AndPlayer2AndPlayer4WinnerUI();
	// プレイヤー壱とプレイヤー参とプレイヤー肆の立ち絵
	void Player1AndPlayer3AndPlayer4WinnerUI();
	// プレイヤー弐とプレイヤー参とプレイヤー肆の立ち絵
	void Player2AndPlayer3AndPlayer4WinnerUI();
	// プレイヤー壱とプレイヤー弐とプレイヤー参とプレイヤー肆の立ち絵
	void AllPlayerWinnerUI();
	//プレイヤー壱のプレイヤー文字
	void Player1FontUI();
	//プレイヤー弐のプレイヤー文字
	void Player2FontUI();
	//プレイヤー参のプレイヤー文字
	void Player3FontUI();
	//プレイヤー肆のプレイヤー文字
	void Player4FontUI();
	//プレイヤー壱とプレイヤー弐のプレイヤー文字
	void Player1AndPlayer2FontUI();
	//プレイヤー壱とプレイヤー参のプレイヤー文字
	void Player1AndPlayer3FontUI();
	//プレイヤー壱とプレイヤー肆のプレイヤー文字
	void Player1AndPlayer4FontUI();
	//プレイヤー弐とプレイヤー参のプレイヤー文字.
	void Player2AndPlayer3FontUI();
	//プレイヤー弐とプレイヤー肆のプレイヤー文字.
	void Player2AndPlayer4FontUI();
	//プレイヤー参とプレイヤー肆のプレイヤー文字.
	void Player3AndPlayer4FontUI();
	// プレイヤー壱とプレイヤー弐とプレイヤー参のプレイヤー文字.
	void Player1AndPlayer2AndPlayer3FontUI();
	// プレイヤー壱とプレイヤー弐とプレイヤー肆のプレイヤー文字.
	void Player1AndPlayer2AndPlayer4FontUI();
	// プレイヤー壱とプレイヤー参とプレイヤー肆のプレイヤー文字.
	void Player1AndPlayer3AndPlayer4FontUI();
	// プレイヤー弐とプレイヤー参とプレイヤー肆のプレイヤー文字.
	void Player2AndPlayer3AndPlayer4FontUI();
	// プレイヤー壱とプレイヤー弐とプレイヤー参とプレイヤー肆のプレイヤー文字.
	void AllPlayerFontUI();

private:
	CSprite2D* BackGround_Img;
	CSprite2D* Winner_Img;
	CSprite2D* Player1_Img;
	CSprite2D* Player2_Img;
	CSprite2D* Player3_Img;
	CSprite2D* Player4_Img;
	CSprite2D* PlayerFont_Img[Font_Max];

	int Win;
};