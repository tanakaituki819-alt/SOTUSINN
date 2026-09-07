#pragma once

#include <GameObject/UIObject/CUIObject.h>

class CPlayerUI
{
private:
	static constexpr int UIH=95;
	static constexpr int UIW=255;
	static constexpr int NoSize=40;
public:
	CPlayerUI();
	~CPlayerUI();

	void SetPlayerNo(int No) { PlayerNo = No; }
	void Draw(int Score);
private:
	CUIObject* Playericon;
	CUIObject* PlayerBackground;
	CUIObject* Playerfont;
	CUIObject* Digit0_9;
	int PlayerNo;
	D3DXVECTOR2 m_Position;
};

