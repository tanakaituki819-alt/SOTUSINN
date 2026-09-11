#pragma once
#include "GameObject/UIObject/CUIObject.h"

class CButtonMahBattleUI
	:public CUIObject
{
public:
	static const int GAUGE_MAX = 4;
public:
	CButtonMahBattleUI();
	~CButtonMahBattleUI()override;

	void Update()override;
	void Draw()override;

	//連打対決の初期化.
	void InitMashBattle();

	//プレイヤー壱のゲージの描画.
	void RenderPlayer1Gauge();
	//プレイヤー弐のゲージの描画.
	void RenderPlayer2Gauge();
	//プレイヤー参のゲージの描画.
	void RenderPlayer3Gauge();
	//プレイヤー肆のゲージの描画.
	void RenderPlayer4Gauge();

private:
	CSprite2D* m_pGaugeImg[GAUGE_MAX];	//ゲージ画像
};