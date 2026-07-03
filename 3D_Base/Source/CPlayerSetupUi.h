#pragma once
#include "CUIObject.h"

class CPlayerSetupUi
	: public CUIObject
{
public:
	//---------------------------
	//	定数宣言.
	//---------------------------
	static const int Ui_Max = 3;

public:
	CPlayerSetupUi();
	~CPlayerSetupUi()override;

	void Update()override;
	void Draw()override;

private:
	CSprite2D* m_pPlayerSetUpUI[Ui_Max];
};

