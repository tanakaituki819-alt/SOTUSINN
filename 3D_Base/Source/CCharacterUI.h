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

private:
	CSprite2D* m_pPlayerUI[PlayerMax];
};