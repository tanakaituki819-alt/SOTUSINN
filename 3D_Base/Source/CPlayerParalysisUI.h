#pragma once
#include "CUIObject.h"


class CPlayerParalysisUI
	: public CUIObject
{
public:
	CPlayerParalysisUI();
	~CPlayerParalysisUI()override;

	void Update();
	void Draw()override;

	void Paralysis(D3DXVECTOR3 Position);	//プレイヤーのポジションを受け取る.

private:
	bool	m_UIChange		= false;	//UIを変更していいか判定.
	float	m_UIChangeTimer = 0;		//UIを変更するタイミングを決めるタイマー.
};

