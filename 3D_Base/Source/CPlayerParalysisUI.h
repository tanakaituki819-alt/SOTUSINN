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

	void Paralysis(D3DXVECTOR3 Position);

private:
};

