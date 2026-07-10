#pragma once

#include "CUIObject.h"

class CCharacterUI
	: public CUIObject
{
public:
	static const int PlayerMax = 4;	//プレイヤー最大数.
public:
	CCharacterUI();
	~CCharacterUI()override;

	void Update()override;
	void Draw()override;
	void CharacterDraw();
	void CharacterShadowDraw();

	//コントローラー(XInput)のセット.
	void SetXInput(CXInput* Input, int i) { m_pMyController[i] = Input; }



private:
	CSprite2D* m_pPlayerUI[PlayerMax];
	CXInput* m_pMyController[PlayerMax];
};