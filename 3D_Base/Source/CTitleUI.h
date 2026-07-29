#pragma once
#include "CUIObject.h"

class CTitleUI
	: public CUIObject
{
public:
	static const int Max = 3;
public:
	CTitleUI();
	~CTitleUI()override;

	void Update()override;
	void Draw()override;

	//背景.
	void BackUI();

	//お先にいただきます
	void Title();
	//開始
	void Start();
	//終了
	void Finish();
	//鍋
	void Nabe();

private:
	CSprite2D* m_Title;
	CSprite2D* m_TitleText[Max];
	CSprite2D* m_Nabe;
};
