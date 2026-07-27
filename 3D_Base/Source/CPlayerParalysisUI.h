#pragma once
#include "CSpriteObject.h"


class CPlayerParalysisUI
	: public CSpriteObject
{
public:
	CPlayerParalysisUI();
	~CPlayerParalysisUI()override;

	void Update();
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)override;

	void Paralysis(D3DXVECTOR3 Position);	//プレイヤーのポジションを受け取る.

private:
	bool	m_UIChange		= false;	//UIを変更していいか判定.
	float	m_UIChangeTimer = 0;		//UIを変更するタイミングを決めるタイマー.
};

