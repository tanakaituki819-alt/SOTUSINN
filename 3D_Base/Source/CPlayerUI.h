#pragma once
#include"CUIObject.h"

class CPlayerUI
{
private:
	static constexpr int UIH=80;
	static constexpr int UIW=240;
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

