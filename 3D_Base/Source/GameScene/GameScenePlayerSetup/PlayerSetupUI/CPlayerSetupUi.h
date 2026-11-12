#pragma once
#include "GameObject/UIObject/CUIObject.h"

//プレイヤーセットアップ画面のUI.
//・1P〜4Pの背景・名前・コントローラーアイコン
//・画面下のボタン案内(巻物とテキスト)
class CPlayerSetupUI
	: public CUIObject
{
public:
	//---------------------------
	//	定数宣言.
	//---------------------------
	static const int Ui_Max = 4;	//プレイヤーの最大数.
	static const int Text_Max = 2;	//画面下のボタン案内の数.

public:
	CPlayerSetupUI();
	~CPlayerSetupUI() override;

	void Update() override;
	void Draw() override;

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pMyController[i] = Input; }

private:
	void DrawBackgrounds();					//各プレイヤーの背景.
	void DrawNames();						//各プレイヤーの名前.
	void DrawControllerIcons();				//各プレイヤーのコントローラーアイコン.
	void DrawGuides();						//画面下のボタン案内.
	bool IsConnected(int player) const;		//コントローラーが接続されているか.

private:
	//描画のたびに位置などを設定し直すので、同じ画像は1つの変数で使い回す.
	CSprite2D* m_pBackground		= nullptr;	//プレイヤーの背景.
	CSprite2D* m_pName				= nullptr;	//プレイヤーの名前.
	CSprite2D* m_pControllerIcon	= nullptr;	//コントローラーアイコン.
	CSprite2D* m_pText				= nullptr;	//テキスト(文字).
	CSprite2D* m_pScroll			= nullptr;	//巻物.

	CXInput* m_pMyController[Ui_Max] = {};	//コントローラー(借りているだけ).
};