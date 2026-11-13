#pragma once

#include "GameObject/UIObject/CUIObject.h"

//プレイヤー選択画面などで、1P〜4Pのキャラクター画像と影を描画するUI.
class CCharacterUI
	: public CUIObject
{
public:
	static const int PlayerMax = 4;	//プレイヤー最大数.

public:
	CCharacterUI();
	~CCharacterUI() override;

	void Update() override;
	void Draw() override;

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pMyController[i] = Input; }

private:
	void DrawShadow(int index);			//index番目のキャラの影を描画する.
	void DrawBody(int index);			//index番目のキャラ本体を描画する.
	bool IsConnected(int index) const;	//index番目のコントローラーが接続されているか.

private:
	CSprite2D* m_pPlayerUI[PlayerMax]	= {};	//キャラ画像(CSpriteManagerから借りているだけ。解放しない).
	CXInput* m_pMyController[PlayerMax] = {};	//コントローラー(借りているだけ).
};