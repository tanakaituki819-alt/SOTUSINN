#pragma once
#include"CUIObject.h"

class CTimer
	:public CUIObject
{
public:
	CTimer();
	~CTimer();
	void SetTime(float Time) {
		m_TimeMAX = Time;
		m_TimeCount= Time;
	}
	virtual void Update() override;
	virtual void Draw()override;
private:
	CSprite2D* m_pbefore;
	int m_TimeMAX;
	int m_TimeCount;

};