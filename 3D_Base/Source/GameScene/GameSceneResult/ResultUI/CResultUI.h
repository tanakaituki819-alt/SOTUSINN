#pragma once
#include "GameObject/UIObject/CUIObject.h"

class CResultUI
	: public CUIObject
{
public:
	static const int PLAYER_MAX = 4;
	static const int MEDAL_MAM = 3;
	static const int FONT_MAM = 4;
public:
	CResultUI();
	~CResultUI()override;

	void Update()override;
	void Draw()override;
	
	//背景.
	void ResultBackUI();
	//プレイヤーの背景
	void PlayerBackUI();

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

	//プレイヤー壱のメダルUI
	void Medal_P1_1st_UI();
	void Medal_P1_2nd_UI();
	void Medal_P1_3rd_UI();
	//プレイヤー弐のメダルUI
	void Medal_P2_1st_UI();
	void Medal_P2_2nd_UI();
	void Medal_P2_3rd_UI();
	//プレイヤー参のメダルUI
	void Medal_P3_1st_UI();
	void Medal_P3_2nd_UI();
	void Medal_P3_3rd_UI();
	//プレイヤー肆のメダルUI
	void Medal_P4_1st_UI();
	void Medal_P4_2nd_UI();
	void Medal_P4_3rd_UI();
	//メダルのスケール、パターンナンバー、レンダ関数をまとめた関数.
	void Medal_Gold_Render();
	void Medal_Silver_Render();
	void Medal_Bronze_Render();

	//プレイヤー壱のフォントUI.
	void Font_P1_UI();
	//プレイヤー弐のフォントUI.
	void Font_P2_UI();
	//プレイヤー参のフォントUI.
	void Font_P3_UI();
	//プレイヤー肆のフォントUI.
	void Font_P4_UI();

	//プレイヤー壱の各行.
	void Font_P1_1st_UI();	//獲得ポイント.
	void Font_P1_2nd_UI();	//具材ポイント.
	void Font_P1_3rd_UI();	//高級具材ポイント.
	void Font_P1_4th_UI();	//合計ポイント.
	//プレイヤー弐の各行.
	void Font_P2_1st_UI();
	void Font_P2_2nd_UI();
	void Font_P2_3rd_UI();
	void Font_P2_4th_UI();
	//プレイヤー参の各行.
	void Font_P3_1st_UI();
	void Font_P3_2nd_UI();
	void Font_P3_3rd_UI();
	void Font_P3_4th_UI();
	//プレイヤー肆の各行.
	void Font_P4_1st_UI();
	void Font_P4_2nd_UI();
	void Font_P4_3rd_UI();
	void Font_P4_4th_UI();

	//フォントのスケール、パターンナンバー、レンダ関数をまとめた関数.
	void Font_1st_Render();	//獲得ポイント.
	void Font_2nd_Render();	//具材ポイント.
	void Font_3rd_Render();	//高級具材ポイント.
	void Font_4th_Render();	//合計ポイント.

	//数字と点の表示.
	void DrawNumber(int Score, const D3DXVECTOR3& Pos);

	//プレイヤーごとのスコア表示.
	void Number_P1_UI();
	void Number_P2_UI();
	void Number_P3_UI();
	void Number_P4_UI();

private:
	CSprite2D* BackGround_Img;
	CSprite2D* PlayerBack_Img[PLAYER_MAX];
	CSprite2D* PlayerIcon_Img[PLAYER_MAX];
	CSprite2D* Medal_Img[MEDAL_MAM];
	CSprite2D* Font_Img[FONT_MAM];
	CSprite2D* Number_Img;

	int Rank;	//仮の条件のためのもの
	int Count;	//仮の条件のためのもの

};