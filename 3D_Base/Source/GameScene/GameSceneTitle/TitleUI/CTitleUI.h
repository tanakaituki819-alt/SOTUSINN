#pragma once
#include "GameObject/UIObject/CUIObject.h"

class CTitleUI
	: public CUIObject
{
public:
	enum class enSelect
	{
		Start,		//hazimeru
		Fin,	//owaru
	};
public:
	static const int Max = 3;
public:
	CTitleUI();
	~CTitleUI()override;

	void Update()override;
	void Draw()override;

	bool IsDecided() const { return m_Decided; }	//決定されたかどうか
	enSelect GetSelect() const { return m_Select; }	//何が選ばれたか

	//コントローラー
	void SetXInput(CXInput* Input) { m_pController = Input; }

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
	//割りばし(開始)
	void STARTWaribashi();
	//割りばし(終了)
	void FINWaribashi();

	//初期化
	void Init();

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
