#pragma once
#include "GameObject/UIObject/CUIObject.h"

//しびれ(麻痺)状態のプレイヤーのそばに出すボタン操作の案内UI.
class CPlayerParalysisUI
	: public CUIObject
{
public:
	CPlayerParalysisUI();
	~CPlayerParalysisUI() override;

	void Update() override;
	void Draw() override;

	//表示の基準になるプレイヤーの位置を設定する.
	void SetPlayerPosition(const D3DXVECTOR3& playerPos);

private:
	int m_AnimFrame = 0;	//絵の切り替えに使うフレームカウンタ.
};