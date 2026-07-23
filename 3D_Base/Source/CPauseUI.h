#pragma once
#include "CUIObject.h"

class CPauseUI
	:public CUIObject
{
public:

	static const int Option_MAX = 2;

public:
	enum class enSelect
	{
		ResumeGame,		//ゲームに戻る
		ReturnToTitle,	//タイトルに戻る
	};

public:
	CPauseUI();
	~CPauseUI()override;

	void Update()override;
	void Draw()override;

	bool IsDecided() const { return m_Decided; }	//決定されたかどうか
	enSelect GetSelect() const { return m_Select; }	//何が選ばれたか

	//コントローラー
	void SetXInput(CXInput* Input) { m_pController = Input; }

	void OpenInit();	//ポーズ画面を開くときの初期化
	void CloseInit();	//ポーズ画面を閉じるときの初期化

private:
	CSprite2D* m_pPauseImg;							//箸休め画像
	CSprite2D* m_pPauseOptionImg	[Option_MAX];	//箸休め中の選択肢.
	CSprite2D* m_pPauseSelectionFrameImg;			//選択中の枠.
	CSprite2D* m_pDimImg;							//暗幕.
	enSelect m_Select;
	bool m_Decided;			//決定フラグ
	bool m_SticTitltOld;	//前フレームではスティックが倒れていたかどうか
	CXInput* m_pController;	//コントローラー
};
