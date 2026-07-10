#include "CPlayerSetupUI.h"
#include "CSpriteManager.h"
#include <iostream>


CPlayerSetupUI::CPlayerSetupUI()
	: m_pPlayerSetUpUI()
	, m_pPlayerNameUI()
	, m_pPlayercontrollerUI()
	, m_pTextUI()
	, m_pScrollUI()
{
	//3つのスプライトを設定.
	for (int i = 0; i < Ui_Max; i++)
	{
		m_pPlayerSetUpUI[i]		 = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);
		m_pPlayerNameUI[i]		 = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Playerfont);
		m_pPlayercontrollerUI[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_controller);
	}
	//テキストスプライトを設定.
	for (int i = 0; i < Text_Max; i++)
	{
		m_pTextUI[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);
		m_pScrollUI[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);
	}
}

CPlayerSetupUI::~CPlayerSetupUI()
{

}

void CPlayerSetupUI::Update()
{

}

void CPlayerSetupUI::Draw()
{

	PlayerBackUI();
	PlayerNameUI();
	ControllerUI();
	ScrollUI();
	TextUI();
}

//プレイヤーの背景UI.
void CPlayerSetupUI::PlayerBackUI()
{
	//プレイヤー1の背景.
	m_pPlayerSetUpUI[0]->SetPosition(D3DXVECTOR3(8.f, 0.f, 0.f));
	m_pPlayerSetUpUI[0]->SetScale(D3DXVECTOR3(300.f, 630.f, 0.f));
	m_pPlayerSetUpUI[0]->SetPatternNo(0.f, 0.f);
	m_pPlayerSetUpUI[0]->Render();
	//プレイヤー2の背景.
	m_pPlayerSetUpUI[1]->SetPosition(D3DXVECTOR3(64.f * 5.2, 0.f, 0.f));
	m_pPlayerSetUpUI[1]->SetScale(D3DXVECTOR3(300.f, 630.f, 0.f));
	m_pPlayerSetUpUI[1]->SetPatternNo(1.f, 0.f);
	m_pPlayerSetUpUI[1]->Render();
	//プレイヤー3の背景.
	m_pPlayerSetUpUI[2]->SetPosition(D3DXVECTOR3(64.f * 10.2, 0.f, 0.f));
	m_pPlayerSetUpUI[2]->SetScale(D3DXVECTOR3(300.f, 630.f, 0.f));
	m_pPlayerSetUpUI[2]->SetPatternNo(2.f, 0.f);
	m_pPlayerSetUpUI[2]->Render();
	//プレイヤー4の背景.
	m_pPlayerSetUpUI[3]->SetPosition(D3DXVECTOR3(64.f * 15.2, 0.f, 0.f));
	m_pPlayerSetUpUI[3]->SetScale(D3DXVECTOR3(300.f, 630.f, 0.f));
	m_pPlayerSetUpUI[3]->SetPatternNo(3.f, 0.f);
	m_pPlayerSetUpUI[3]->Render();

}
//プレイヤーの名前UI.
void CPlayerSetupUI::PlayerNameUI()
{
	//プレイヤー1の名前.
	m_pPlayerNameUI[0]->SetPosition(D3DXVECTOR3(8.f, 0.f, 0.f));
	m_pPlayerNameUI[0]->SetScale(D3DXVECTOR3(300.f, 75.f, 0.f));
	m_pPlayerNameUI[0]->SetPatternNo(0.f, 0.f);
	m_pPlayerNameUI[0]->Render();
	//プレイヤー2の名前.
	m_pPlayerNameUI[1]->SetPosition(D3DXVECTOR3(64.f * 5.2, 0.f, 0.f));
	m_pPlayerNameUI[1]->SetScale(D3DXVECTOR3(300.f, 75.f, 0.f));
	m_pPlayerNameUI[1]->SetPatternNo(0.f, 1.f);
	m_pPlayerNameUI[1]->Render();
	//プレイヤー3の名前.
	m_pPlayerNameUI[2]->SetPosition(D3DXVECTOR3(64.f * 10.2, 0.f, 0.f));
	m_pPlayerNameUI[2]->SetScale(D3DXVECTOR3(300.f, 75.f, 0.f));
	m_pPlayerNameUI[2]->SetPatternNo(0.f, 2.f);
	m_pPlayerNameUI[2]->Render();
	//プレイヤー4の名前.
	m_pPlayerNameUI[3]->SetPosition(D3DXVECTOR3(64.f * 15.2, 0.f, 0.f));
	m_pPlayerNameUI[3]->SetScale(D3DXVECTOR3(300.f, 75.f, 0.f));
	m_pPlayerNameUI[3]->SetPatternNo(0.f, 3.f);
	m_pPlayerNameUI[3]->Render();
}
//プレイヤーのコントローラーUI.
void CPlayerSetupUI::ControllerUI()
{
	static int x = 0;
	if (GetAsyncKeyState('W') & 0x8000) {
		x += 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		x -= 1;
	}
	//プレイヤー1.
	m_pPlayercontrollerUI[0]->SetAlpha(0.2f);
	m_pPlayercontrollerUI[0]->SetPosition(D3DXVECTOR3(154.f, 100.f, 0.f));
	m_pPlayercontrollerUI[0]->SetScale(D3DXVECTOR3(150.f, 75.f, 0.f));

	if (m_pMyController[0]->IsConnect() == true)
	{
		m_pPlayercontrollerUI[0]->SetAlpha(1.0f);
	}

	m_pPlayercontrollerUI[0]->Render();
	//プレイヤー2.
	m_pPlayercontrollerUI[1]->SetAlpha(0.2f);
	m_pPlayercontrollerUI[1]->SetPosition(D3DXVECTOR3(64.f * 7.5f, 100.f, 0.f));
	m_pPlayercontrollerUI[1]->SetScale(D3DXVECTOR3(150.f, 75.f, 0.f));
	
	if (m_pMyController[1]->IsConnect() == true)
	{
		m_pPlayercontrollerUI[1]->SetAlpha(1.0f);
	}
	m_pPlayercontrollerUI[1]->Render();
	//プレイヤー3.
	m_pPlayercontrollerUI[2]->SetAlpha(0.2f);
	m_pPlayercontrollerUI[2]->SetPosition(D3DXVECTOR3(64.f *12.5f, 100.f, 0.f));
	m_pPlayercontrollerUI[2]->SetScale(D3DXVECTOR3(150.f, 75.f, 0.f));
	if (m_pMyController[2]->IsConnect() == true)
	{
		m_pPlayercontrollerUI[2]->SetAlpha(1.0f);
	}
	m_pPlayercontrollerUI[2]->Render();
	//プレイヤー4.
	m_pPlayercontrollerUI[3]->SetAlpha(0.2f);
	m_pPlayercontrollerUI[3]->SetPosition(D3DXVECTOR3(64.f * 17.5f, 100.f, 0.f));
	m_pPlayercontrollerUI[3]->SetScale(D3DXVECTOR3(150.f, 75.f, 0.f));
	if (m_pMyController[3]->IsConnect() == true)
	{
		m_pPlayercontrollerUI[3]->SetAlpha(1.0f);
	}
	m_pPlayercontrollerUI[3]->Render();
}
//テキストUI.
void CPlayerSetupUI::TextUI()
{	
	//タイトルに戻るUI.
	m_pTextUI[0]->SetPosition(D3DXVECTOR3(700.f, WND_H - 75.f, 0.f));
	m_pTextUI[0]->SetScale(D3DXVECTOR3(300.f, 100.f, 0.f));
	m_pTextUI[0]->SetPatternNo(0.f, 2.f);
	m_pTextUI[0]->Render();
	//準備完了キャンセルUI.
	m_pTextUI[1]->SetPosition(D3DXVECTOR3(64.f * 15.f - 32.f, WND_H - 75.f, 0.f));
	m_pTextUI[1]->SetScale(D3DXVECTOR3(300.f, 100.f, 0.f));
	m_pTextUI[1]->SetPatternNo(0.f, 1.f);
	m_pTextUI[1]->Render();
}
//巻物UI.
void CPlayerSetupUI::ScrollUI()
{
	static int x = 0;
	if (GetAsyncKeyState('W') & 0x8000) {
		x += 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		x -= 1;
	}
	//タイトルに戻るUI.
	m_pScrollUI[0]->SetPosition(D3DXVECTOR3(685, WND_H - 80.f, 0.f));
	m_pScrollUI[0]->SetScale(D3DXVECTOR3(210.f, 100.f, 0.f));
	m_pScrollUI[0]->Render();
	//準備完了キャンセルUI.
	m_pScrollUI[1]->SetPosition(D3DXVECTOR3(64.f * 13.9f, WND_H - 80.f, 0.f));
	m_pScrollUI[1]->SetScale(D3DXVECTOR3(380.f, 100.f, 0.f));
	m_pScrollUI[1]->Render();
}
