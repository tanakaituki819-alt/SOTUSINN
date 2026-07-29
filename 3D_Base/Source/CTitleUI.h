#pragma once
#include "CUIObject.h"

class CTitleUI
	: public CUIObject
{
public:
	enum class enSelect
	{
		Start,		//ゲームに戻る
		Fin,	//タイトルに戻る
	};
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
	//割りばし()
	void STARTWaribashi();
	//割りばし
	void FINWaribashi();

private:
	enSelect m_Select;
	bool m_Decided;			//決定フラグ
	bool m_SticTitltOld;	//前フレームではスティックが倒れていたかどうか
	CXInput* m_pController;	//コントローラー
private:
	CSprite2D* m_Title;
	CSprite2D* m_TitleText[Max];
	CSprite2D* m_Nabe;
	CSprite2D* m_Waribashi;
};
