#pragma once
#include "CUIObject.h"

class CWinnerResultUI
	: public CUIObject
{
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
	//プレイヤー壱とプレイヤー弐
	
	//プレイヤー壱とプレイヤー参
	
	//プレイヤー壱とプレイヤー肆

	//プレイヤー弐とプレイヤー参

	//プレイヤー弐とプレイヤー肆

	//プレイヤー参とプレイヤー肆

	
private:
	CSprite2D* BackGround_Img;
	CSprite2D* Winner_Img;
	CSprite2D* Player1_Img;
	CSprite2D* Player2_Img;
	CSprite2D* Player3_Img;
	CSprite2D* Player4_Img;

	int Win;
};