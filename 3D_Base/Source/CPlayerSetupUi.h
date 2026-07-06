#pragma once
#include "CUIObject.h"

class CPlayerSetupUI
	: public CUIObject
{
public:
	//---------------------------
	//	定数宣言.
	//---------------------------
	static const int Ui_Max = 4;

public:
	CPlayerSetupUI();
	~CPlayerSetupUI()override;

	void Update()override;
	void Draw()override;

private:
	CSprite2D* m_pPlayerSetUpUI[Ui_Max];
};

