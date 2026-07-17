#include "CGameRdyUI.h"
#include <iostream>


CGameRdyUI::CGameRdyUI()
	: m_pRightHand			()
	, m_pLeftHand			()
	, m_pGamerdy			()
	, m_pController			()
	, m_RightHandclap		()
	, m_LeftHandclap		()
	, m_Handclapflag		()
	, m_TitleBackflag		()
	, m_pTextFrame			()
	, m_IsYesSelected		()
	, m_SelectchangeTimer	()
	, m_SelectTimeOver		()
{
	//手の最大数分(4).
	for (int i = 0; i < hand_Max; i++)
	{
		m_pRightHand[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);		//手の画像.
		m_pLeftHand[i]  = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);		//手の画像.
		m_pGamerdy[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);		//テキスト画像.
	}
	//テキスト最大数(3).
	for (int i = 0; i < Text_Max; i++)
	{
		m_pTitleBackText[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);	//タイトルに戻るテキスト.
	}
	//フレームの最大数.
	for (int i = 0; i < Frame_Max; i++)
	{
		m_pTextFrame[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TextFrame);	//テキストのフレーム.
	}
	m_pWhiteBack		= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_WhiteBack);			//白の背景画像.
	m_pScroll			= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);				//巻物.
	m_pSelectionFrame	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_SelectionFrame);		//セレクトフレーム.
}

CGameRdyUI::~CGameRdyUI()
{
}

void CGameRdyUI::Update()
{
	//連続でセレクトを動かせないようにするため(バグ防止、後で数値かえるかも).
	{
		//セレクトフレームを動かせないなら.
		if (m_SelectTimeOver == false)
		{
			++m_SelectchangeTimer;	//タイマー増加.
			//タイマーが一定のフレーム数を超えたなら.
			if (m_SelectchangeTimer > 15)
			{
				m_SelectTimeOver = true;	//セレクトフレームを動かせるようにする.
				m_SelectchangeTimer = 0;	//タイマー初期化.
			}
		}
	}
	//タイトルに戻る画面が出ているなら.
	if (m_TitleBackflag == true)
	{
		//セレクトフレームを移動させていいか
		if (m_SelectTimeOver == true)
		{
			//左スティックを左に倒したなら.
			if (m_pController[0]->GetLThumbX() > 0.5f)
			{
				m_IsYesSelected = true;		//はいを選択中.
				m_SelectTimeOver = false;	//セレクトフレームを動かせないようにする.
			}
			//左スティックを右に倒したなら.
			else if (m_pController[0]->GetLThumbX() < -0.5f)
			{
				m_IsYesSelected = false;	//いいえを選択中.
				m_SelectTimeOver = false;	//セレクトフレームを動かせないようにする.
			}
		}
		//はいを選択中なら.
		if (m_IsYesSelected == true)
		{
			//Aボタンを押した.
			if (m_pController[0]->IsDown(CXInput::A, true))
			{
				//テキストメッセージ(仮).
				MessageBox(nullptr,
					_T("タイトルに戻りますか？"),
					_T("警告"), MB_YESNO == IDYES);
			}
		}
		//いいえを選択中なら.
		else
		{
			//Aボタンを押した.
			if (m_pController[0]->IsDown(CXInput::A, true))
			{
				m_TitleBackflag = false;	//タイトルに戻る描画をしない.
			}
		}
		return;	//リターンで返して以降通らないようにする.
	}

	//準備完了ではないなら.
	if (m_Handclapflag[0] == false)
	{
		//トリガー判定.
		if (m_pController[0]->IsDown(CXInput::B, true))
		{
			m_TitleBackflag = true;		//タイトルに戻る画面を表示する.
			m_IsYesSelected = false;	//いいえを選択している状態に戻す.
			return;		//リターンで返して以降通らないようにする
		}
	}
	//プレイヤーの最大数分.
	for (int i = 0; i < hand_Max; i++)
	{
		//Aボタン準備完了.
		if (m_pController[i]->IsDown(CXInput::A, true))
		{
			m_Handclapflag[i] = true;	//準備完了状態へ.
		}
		//Bボタンを押すと準備完了解除.
		else if (m_pController[i]->IsDown(CXInput::B, true))
		{
			//準備完了状態なら.
			if (m_Handclapflag[i])
			{
				m_Handclapflag[i] = false;	//準備完了解除.
			}
		}
		//準備完了状態なら.
		if (m_Handclapflag[i])
		{
			//手が一定の位置まで移動していなければ.
			if (m_LeftHandclap[i] < 60 * 0.65f)
			{
				//手を動かす.
				m_RightHandclap[i] -= 4.f;
				m_LeftHandclap[i] += 4.f;
			}
		}
		else
		{
			//位置リセット.
			m_RightHandclap[i] = 0.f;
			m_LeftHandclap[i] = 0.f;
		}
	}
}

void CGameRdyUI::Draw()
{
	//各関数呼び出し.
	RightHand();
	LeftHand();
	GameRdy();
	TitleBack();
}
//右手.
void CGameRdyUI::RightHand()
{
	//プレイヤー1.
	if (m_Handclapflag[0])	//ボタンが押されたなら.
	{
		m_pRightHand[0]->SetAlpha(0.f);
		m_pRightHand[0]->SetPosition(D3DXVECTOR3(80.f + m_RightHandclap[0], 80.f, 0.f));
		m_pRightHand[0]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pRightHand[0]->SetPatternNo(0.f, 1.f);
		//コントローラーが接続されているか.
		if (m_pController[0]->IsConnect() == true)
		{
			m_pRightHand[0]->SetAlpha(1.0f); //α値を戻す.
		}
		m_pRightHand[0]->Render();
	}
	//プレイヤー2.
	if (m_Handclapflag[1])	//ボタンが押されたなら.
	{
		m_pRightHand[1]->SetAlpha(0.f);
		m_pRightHand[1]->SetPosition(D3DXVECTOR3(64.f * 4.6f + 110.f + m_RightHandclap[1], 80.f, 0.f));
		m_pRightHand[1]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pRightHand[1]->SetPatternNo(0.f, 1.f);
		//コントローラーが接続されているか.
		if (m_pController[1]->IsConnect() == true)
		{
			m_pRightHand[1]->SetAlpha(1.0f); //α値を戻す.
		}
		m_pRightHand[1]->Render();
	}
	//プレイヤー3.
	if (m_Handclapflag[2])	//ボタンが押されたなら.
	{
		m_pRightHand[2]->SetAlpha(0.f);
		m_pRightHand[2]->SetPosition(D3DXVECTOR3(64.f * 9.6f + 110.f + m_RightHandclap[2], 80.f, 0.f));
		m_pRightHand[2]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pRightHand[2]->SetPatternNo(0.f, 1.f);
		//コントローラーが接続されているか.
		if (m_pController[2]->IsConnect() == true)
		{
			m_pRightHand[2]->SetAlpha(1.0f); //α値を戻す.
		}
		m_pRightHand[2]->Render();
	}
	//プレイヤー4.
	if (m_Handclapflag[3])	//ボタンが押されたなら.
	{
		m_pRightHand[3]->SetAlpha(0.f);
		m_pRightHand[3]->SetPosition(D3DXVECTOR3(64.f * 14.6 + 110.f + m_RightHandclap[3], 80.f, 0.f));
		m_pRightHand[3]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pRightHand[3]->SetPatternNo(0.f, 1.f);
		//コントローラーが接続されているか.
		if (m_pController[3]->IsConnect() == true)
		{
			m_pRightHand[3]->SetAlpha(1.0f); //α値を戻す.
		}
		m_pRightHand[3]->Render();
	}
}
//左手.
void CGameRdyUI::LeftHand()
{
	//プレイヤー1.
	if (m_Handclapflag[0])	//ボタンが押されたなら.
	{
		m_pLeftHand[0]->SetAlpha(0.f);
		m_pLeftHand[0]->SetPosition(D3DXVECTOR3(0.f + m_LeftHandclap[0], 80.f, 0.f));
		m_pLeftHand[0]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pLeftHand[0]->SetPatternNo(0.f, 0.f);
		//コントローラーが接続されているか.
		if (m_pController[0]->IsConnect() == true)
		{
			m_pLeftHand[0]->SetAlpha(1.0f);
		}
		m_pLeftHand[0]->Render();
	}
	//プレイヤー2.
	if (m_Handclapflag[1])	//ボタンが押されたなら.
	{
		m_pLeftHand[1]->SetAlpha(0.f);
		m_pLeftHand[1]->SetPosition(D3DXVECTOR3(64.f * 4.6f + 31.f + m_LeftHandclap[1], 80.f, 0.f));
		m_pLeftHand[1]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pLeftHand[1]->SetPatternNo(0.f, 0.f);
		//コントローラーが接続されているか.
		if (m_pController[1]->IsConnect() == true)
		{
			m_pLeftHand[1]->SetAlpha(1.0f);
		}
		m_pLeftHand[1]->Render();
	}
	//プレイヤー3.
	if (m_Handclapflag[2])	//ボタンが押されたなら.
	{
		m_pLeftHand[2]->SetAlpha(0.f);
		m_pLeftHand[2]->SetPosition(D3DXVECTOR3(64.f * 9.6f + 31.f + m_LeftHandclap[2], 80.f, 0.f));
		m_pLeftHand[2]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pLeftHand[2]->SetPatternNo(0.f, 0.f);
		//コントローラーが接続されているか.
		if (m_pController[2]->IsConnect() == true)
		{
			m_pLeftHand[2]->SetAlpha(1.0f);
		}
		m_pLeftHand[2]->Render();
	}
	//プレイヤー4.
	if (m_Handclapflag[3])	//ボタンが押されたなら.
	{
		m_pLeftHand[3]->SetAlpha(0.f);
		m_pLeftHand[3]->SetPosition(D3DXVECTOR3(64.f * 14.6 + 31.f + m_LeftHandclap[3], 80.f, 0.f));
		m_pLeftHand[3]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pLeftHand[3]->SetPatternNo(0.f, 0.f);
		//コントローラーが接続されているなら.
		if (m_pController[3]->IsConnect() == true)	
		{
			m_pLeftHand[3]->SetAlpha(1.0f);	
		}
		m_pLeftHand[3]->Render();
	}
}
//
void CGameRdyUI::GameRdy()
{

	if (m_LeftHandclap[0] > 60 * 0.65f)
	{
		m_pGamerdy[0]->SetAlpha(0.f);
		m_pGamerdy[0]->SetPosition(D3DXVECTOR3(36.f, 50.f, 0.f));
		m_pGamerdy[0]->SetScale(D3DXVECTOR3(350.f, 170.f, 0.f));
		m_pGamerdy[0]->SetRotation(D3DXVECTOR3(0.f,0.f,0.3f));
		m_pGamerdy[0]->SetPatternNo(0.f, 5.f);
		//コントローラーが接続されているか.
		if (m_pController[0]->IsConnect() == true)
		{
			m_pGamerdy[0]->SetAlpha(1.0f);
		}
		m_pGamerdy[0]->Render();
	}
	if (m_LeftHandclap[1] > 60 * 0.65f)
	{
		m_pGamerdy[1]->SetAlpha(0.f);
		m_pGamerdy[1]->SetPosition(D3DXVECTOR3(64.f * 4.6f + 31.f + m_LeftHandclap[1], 80.f, 0.f));
		m_pGamerdy[1]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pGamerdy[1]->SetPatternNo(0.f, 5.f);
		//コントローラーが接続されているか.
		if (m_pController[1]->IsConnect() == true)
		{
			m_pGamerdy[1]->SetAlpha(1.0f);
		}
		m_pGamerdy[1]->Render();
	}
		//プレイヤー3.
	if (m_LeftHandclap[2] > 60 * 0.65f)
	{
		m_pGamerdy[2]->SetAlpha(0.f);
		m_pGamerdy[2]->SetPosition(D3DXVECTOR3(64.f * 9.6f + 31.f + m_LeftHandclap[2], 80.f, 0.f));
		m_pGamerdy[2]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pGamerdy[2]->SetPatternNo(0.f, 5.f);
		//コントローラーが接続されているか.
		if (m_pController[2]->IsConnect() == true)
		{
			m_pGamerdy[2]->SetAlpha(1.0f);
		}
		m_pGamerdy[2]->Render();
	}
		//プレイヤー4.
	if (m_LeftHandclap[3] > 60 * 0.65f)
	{
		m_pGamerdy[3]->SetAlpha(0.f);
		m_pGamerdy[3]->SetPosition(D3DXVECTOR3(64.f * 14.6 + 31.f + m_LeftHandclap[3], 80.f, 0.f));
		m_pGamerdy[3]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pGamerdy[3]->SetPatternNo(0.f, 5.f);
		//コントローラーが接続されているか.
		if (m_pController[3]->IsConnect() == true)
		{
			m_pGamerdy[3]->SetAlpha(1.0f);
		}
		m_pGamerdy[3]->Render();
	}
}
//タイトルに戻るときのUI.
void CGameRdyUI::TitleBack()
{
	static int y = 0;
	if (GetAsyncKeyState('W') & 0x8000) {
		y += 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		y -= 1;
	}
	static int x = 0;
	if (GetAsyncKeyState('A') & 0x8000) {
		x += 1;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		x -= 1;
	}

	std::cout << x << std::endl;
	std::cout << y << std::endl;
	//タイトルに戻る描画を出していいなら.
	if (m_TitleBackflag == true)
	{
		//白背景.
		m_pWhiteBack->SetAlpha(0.5f);
		m_pWhiteBack->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_pWhiteBack->SetScale(D3DXVECTOR3(1280.f, 720.f, 0.f));
		m_pWhiteBack->SetPatternNo(0.f, 0.f);
		m_pWhiteBack->Render();
		//巻物.
		m_pScroll->SetAlpha(1.f);
		m_pScroll->SetPosition(D3DXVECTOR3(650.f, WND_H / 2.f, 0.f));
		m_pScroll->SetScale(D3DXVECTOR3(800.f, 1350.f, 0.f));
		m_pScroll->SetRotation(D3DXVECTOR3(0.f, 0.f, 1.57f));
		m_pScroll->SetPatternNo(0.f, 0.f);
		m_pScroll->Render();
		//テキストのフレーム.
		m_pTextFrame[0]->SetAlpha(1.f);
		m_pTextFrame[0]->SetPosition(D3DXVECTOR3(400.f, 484.f, 0.f));
		m_pTextFrame[0]->SetScale(D3DXVECTOR3(350.f, 150.f, 0.f));
		m_pTextFrame[0]->SetPatternNo(0.f, 0.f);
		m_pTextFrame[0]->Render();

		m_pTextFrame[1]->SetAlpha(1.f);
		m_pTextFrame[1]->SetPosition(D3DXVECTOR3(860.f, 484.f, 0.f));
		m_pTextFrame[1]->SetScale(D3DXVECTOR3(350.f, 150.f, 0.f));
		m_pTextFrame[1]->SetPatternNo(0.f, 0.f);
		m_pTextFrame[1]->Render();

		//はいを選択中なら.
		if (m_IsYesSelected == true)
		{
			//セレクトフレーム.
			m_pSelectionFrame->SetAlpha(1.f);
			m_pSelectionFrame->SetPosition(D3DXVECTOR3(860.f, 484.f, 0.f));
			m_pSelectionFrame->SetScale(D3DXVECTOR3(358.f, 158.f, 0.f));
			m_pSelectionFrame->Render();
		}
		//いいえを選択中なら.
		else
		{
			//セレクトフレーム.
			m_pSelectionFrame->SetAlpha(1.f);
			m_pSelectionFrame->SetPosition(D3DXVECTOR3(400.f, 484.f, 0.f));
			m_pSelectionFrame->SetScale(D3DXVECTOR3(358.f, 158.f, 0.f));
			m_pSelectionFrame->Render();
		}

		//テキスト.(タイトルに戻りますか?).
		m_pTitleBackText[0]->SetAlpha(1.f);
		m_pTitleBackText[0]->SetPosition(D3DXVECTOR3(340.f, 140.f, 0.f));
		m_pTitleBackText[0]->SetScale(D3DXVECTOR3(600.f, 260.f, 0.f));
		m_pTitleBackText[0]->SetPatternNo(0.f, 0.f);
		m_pTitleBackText[0]->Render();
		//(はい)
		m_pTitleBackText[1]->SetAlpha(1.f);
		m_pTitleBackText[1]->SetPosition(D3DXVECTOR3(748.f, 432.f, 0.f));
		m_pTitleBackText[1]->SetScale(D3DXVECTOR3(600.f, 150.f, 0.f));
		m_pTitleBackText[1]->SetPatternNo(0.f, 3.f);
		m_pTitleBackText[1]->Render();
		//(いいえ)
		m_pTitleBackText[2]->SetAlpha(1.f);
		m_pTitleBackText[2]->SetPosition(D3DXVECTOR3(262.f, 406.f, 0.f));
		m_pTitleBackText[2]->SetScale(D3DXVECTOR3(600.f, 150.f, 0.f));
		m_pTitleBackText[2]->SetPatternNo(0.f, 4.f);
		m_pTitleBackText[2]->Render();

	}
}
