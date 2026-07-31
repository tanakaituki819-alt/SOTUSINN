#pragma once
#include "GameObject/UIObject/CUIObject.h"

class CResultUI
	: public CUIObject
{
public:
	static const int Player_Max = 4;
public:
	CResultUI();
	~CResultUI()override;

	void Update()override;
	void Draw()override;
	
	//背景.
	void ResultBackUI();
	//プレイヤー壱の背景
	void Player1BackUI();
	//プレイヤー弐の背景
	void Player2BackUI();
	//プレイヤー参の背景
	void Player3BackUI();
	//プレイヤー肆の背景
	void Player4BackUI();

	//プレイヤー1の位置.
	void Player1Pos();
	//プレイヤー2の位置.
	void Player2Pos();
	//プレイヤー3の位置.
	void Player3Pos();
	//プレイヤー4の位置.
	void Player4Pos();
	//プレイヤーアイコンの拡縮.
	void PlayerScl();

	//プレイヤー壱が一位の時のアイコン.
	void Player1First();
	//プレイヤー壱が二位の時のアイコン.
	void Player1Second();
	//プレイヤー壱が三位の時のアイコン.
	void Player1Third();
	//プレイヤー壱が四位の時のアイコン.
	void Player1Fourth();

	//プレイヤー弐が一位の時のアイコン.
	void Player2First();
	//プレイヤー弐が二位の時のアイコン.
	void Player2Second();
	//プレイヤー弐が三位の時のアイコン.
	void Player2Third();
	//プレイヤー弐が四位の時のアイコン.
	void Player2Fourth();

	//プレイヤー参が一位の時のアイコン.
	void Player3First();
	//プレイヤー参が二位の時のアイコン.
	void Player3Second();
	//プレイヤー参が三位の時のアイコン.
	void Player3Third();
	//プレイヤー参が四位の時のアイコン.
	void Player3Fourth();

	//プレイヤー肆が一位の時のアイコン.
	void Player4First();
	//プレイヤー肆が二位の時のアイコン.
	void Player4Second();
	//プレイヤー肆が三位の時のアイコン.
	void Player4Third();
	//プレイヤー肆が四位の時のアイコン.
	void Player4Fourth();

private:
	CSprite2D* BackGround_Img;
	CSprite2D* PlayerBack_Img[Player_Max];
	CSprite2D* PlayerIcon_Img[Player_Max];

	int Rank;	//仮の条件のためのもの
	int Count;	//仮の条件のためのもの

};