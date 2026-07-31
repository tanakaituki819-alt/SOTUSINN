#include "CPlayerUI.h"

CPlayerUI::CPlayerUI()
{
	Playericon = new CUIObject();
	Playericon->AttachSprite(*CSpM::GetSprite2D(CSpMLstiImg::Img_Playericon));
	PlayerBackground = new CUIObject();
	PlayerBackground->AttachSprite(*CSpM::GetSprite2D(CSpMLstiImg::Img_PlayerBackground));
	Playerfont = new CUIObject();
	Playerfont->AttachSprite(*CSpM::GetSprite2D(CSpMLstiImg::Img_Playerfont));
	Digit0_9 = new CUIObject();
	Digit0_9->AttachSprite(*CSpM::GetSprite2D(CSpMLstiImg::Digit0_9));

	Playericon->SetScale(UIH, UIH,0);
	PlayerBackground->SetScale(UIW, UIH, 0);
	Playerfont->SetScale(UIW/2, UIH/3, 0);
	Digit0_9->SetScale( NoSize ,NoSize *1.5,0);
	


}

CPlayerUI::~CPlayerUI()
{
	SAFE_DELETE(Playericon);
	SAFE_DELETE(PlayerBackground);
	SAFE_DELETE(Playerfont);

	SAFE_DELETE(Digit0_9);

	
	
}

void CPlayerUI::Draw(int Score)
{
	int x=0, y=0;
	//各プレイヤーのUIpos
	switch (PlayerNo)
	{
	case 0:
		m_Position = { 0,0 };
		break;
	case 1:
		m_Position = { WND_W - UIW,0 };
		break;
	case 2:
		m_Position = { 0,WND_H-UIH };
		break;
	case 3:
		m_Position = { WND_W - UIW,WND_H - UIH };
		break;
	}

	PlayerBackground->SetPosition(m_Position.x, m_Position.y, 0);
	PlayerBackground->SetPatternNo(PlayerNo, 0);
	PlayerBackground->Draw();

	Playericon->SetPosition(m_Position.x+UIW-UIH, m_Position.y,0);
	Playericon->SetPatternNo(0,PlayerNo);
	Playericon->Draw();

	Playerfont->SetPosition(m_Position.x, m_Position.y,0);
	Playerfont->SetPatternNo(0, PlayerNo);
	Playerfont->Draw();

	int S = Score;
	
	if (S > 999) {
		S = 999;
	}

	int M = 100;
	for (int i = 0;i < 3;i++) {
	
		Digit0_9->SetPatternNo(static_cast<int>(S / M), 0);
		Digit0_9->SetPosition(m_Position.x+ NoSize*i, m_Position.y+UIH/3, 0);
		Digit0_9->Draw();
		S -= static_cast<int>(S / M) * M;
		M /= 10;
	}
	Digit0_9->SetPatternNo(10, 0);
	Digit0_9->SetPosition(m_Position.x + NoSize * 3, m_Position.y + UIH / 3, 0);
	Digit0_9->Draw();



}
