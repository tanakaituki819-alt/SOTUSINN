#pragma once
#include "GameObject/UIObject/CUIObject.h"

class CWinnerResultUI
	: public CUIObject
{
public:
	static const int PLAYER_MAX = 4;	//プレイヤーの最大数.
	static const int FONT_MAM = 4;	//フォントの最大値
	static const int PARA_MAM = 4;	//平行四辺形最大値

public:
	CWinnerResultUI();
	~CWinnerResultUI()override;

	void Update()override;
	void Draw()override;

	bool IsDecided() const { return m_Decided; }	//決定されたかどうか

	//コントローラー
	void SetXInput(CXInput* Input) { m_pController = Input; }

	//初期化
	void Init();

	//1位の人数を取得.
	int GetWinnerCount() const { return m_WinnerCount; }
	//1位のプレイヤー番号を取得.
	int GetWinnerNo(int No) const { return m_WinnerNo[No]; }

	//背景.
	void WinnerResultBackUI();

	//-単独1位の画面-.
	void SoloWinnerUI();
	//あっぱれ.
	void SoloWinnerTextUI();
	//1位のプレイヤーの立ち絵.
	void SoloPlayerUI();
	//1位のプレイヤーの文字.
	void SoloPlayerFontUI();
	//ポイントの枠.
	void SoloShape();
	//ポイントの文字と数字.
	void SoloPointUI();
	//お気に入り食材の枠と文字と数字.
	void SoloFavoriteUI();

	//-同率1位の画面-.
	void TieWinnerUI();
	//あっぱれ.
	void TieWinnerTextUI();
	//1位のプレイヤーたちの立ち絵.
	void TiePlayerUI();
	//1位のプレイヤーたちの文字.
	void TiePlayerFontUI();
	//かくとくポイントと合計ポイント.
	void TiePointUI();
	//i人目の立ち絵の中心のX座標.
	float GetTieCenterX(int i) const;

	//数字と点の表示.
	void DrawNumber(int Score, float PosX, float PosY, float W, float H);
	//数字と点を並べたときの幅.
	float GetNumberWidth(int Score, float W);

private:
	bool m_Decided;			//決定フラグ
	CXInput* m_pController;	//コントローラー

	int m_WinnerNo[PLAYER_MAX];	//1位のプレイヤー番号.
	int m_WinnerCount;			//1位の人数.

private:
	CSprite2D* BackGround_Img;
	CSprite2D* Winner_Img;
	CSprite2D* Player_Img[PLAYER_MAX];
	CSprite2D* PlayerFont_Img[FONT_MAM];
	CSprite2D* Para_Img[PARA_MAM];
	CSprite2D* RoundRect_Img;
	CSprite2D* ResultFont_Img;
	CSprite2D* Number_Img;
	CSprite2D* FavoriteFont_Img;
};
