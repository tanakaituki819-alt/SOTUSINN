#include "CGameRdyUI.h"


CGameRdyUI::CGameRdyUI()
	: m_pRightHand		()
	, m_pLeftHand		()
	, m_pGamerdy		()
	, m_pController		()
	, m_RightHandclap	()
	, m_LeftHandclap	()
	, m_Handclapflag	()
{
	for (int i = 0; i < hand_Max; i++)
	{
		m_pRightHand[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);	//手の画像.
		m_pLeftHand[i]  = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);	//手の画像.
		m_pGamerdy[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);	//テキスト画像.
	}
	for (int i = 0; i < Text_Max; i++)
	{
		m_pTitleBackText[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text); //タイトルに戻るテキスト.
	}
	m_pWhiteBack = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_WhiteBack);		 //白の背景画像.
	m_pScroll	 = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);			 //巻物.
}

CGameRdyUI::~CGameRdyUI()
{
}

void CGameRdyUI::Update()
{
	//準備完了ではないなら.
	if (m_Handclapflag[0] == false)
	{
		//トリガー判定.
		if (m_pController[0]->IsDown(CXInput::B, true)) 
		{
			m_TitleBackflag = true;		//タイトルに戻る画面を表示する.
		}
	}
	//プレイヤーの最大数分.
	for (int i = 0; i < hand_Max; i++)
	{
		//Aボタン準備完了.
		if (m_pController[i]->IsDown(CXInput::A, false))
		{
			m_Handclapflag[i] = true;	//準備完了状態へ.
			//1Pなら.
			if (i == 0) {
				m_TitleBackflag = false; //1Pが準備完了したら確認を消す.
			}
		}
		//Bボタンを押すと準備完了解除.
		else if (m_pController[i]->IsDown(CXInput::B, false))
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

		if (m_pController[0]->IsConnect() == true)
		{
			m_pRightHand[0]->SetAlpha(1.0f);
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
		if (m_pController[1]->IsConnect() == true)
		{
			m_pRightHand[1]->SetAlpha(1.0f);
		}
		m_pRightHand[1]->Render();
	}
	if (m_Handclapflag[2])	//ボタンが押されたなら.
	{
		//プレイヤー3.
		m_pRightHand[2]->SetAlpha(0.f);
		m_pRightHand[2]->SetPosition(D3DXVECTOR3(64.f * 9.6f + 110.f + m_RightHandclap[2], 80.f, 0.f));
		m_pRightHand[2]->SetScale(D3DXVECTOR3(110.f, 130.f, 0.f));
		m_pRightHand[2]->SetPatternNo(0.f, 1.f);
		if (m_pController[2]->IsConnect() == true)
		{
			m_pRightHand[2]->SetAlpha(1.0f);
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
		if (m_pController[3]->IsConnect() == true)
		{
			m_pRightHand[3]->SetAlpha(1.0f);
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
		//コントローラーが接続されているなら.
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
	if (m_TitleBackflag == true)
	{
		m_pWhiteBack->SetAlpha(0.5f);
		m_pWhiteBack->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_pWhiteBack->SetScale(D3DXVECTOR3(1280.f, 720.f, 0.f));
		m_pWhiteBack->SetPatternNo(0.f, 0.f);
		m_pWhiteBack->Render();
		
		m_pScroll->SetAlpha(1.f);
		m_pScroll->SetPosition(D3DXVECTOR3(1100, WND_H / 4.f, 0.f));
		m_pScroll->SetScale(D3DXVECTOR3(500.f, 850.f, 0.f));
		m_pScroll->SetRotation(D3DXVECTOR3(0.f, 0.f, 1.57f));
		m_pScroll->SetPatternNo(0.f, 0.f);
		m_pScroll->Render();


		m_pTitleBackText[0]->SetAlpha(1.f);
		m_pTitleBackText[0]->SetPosition(D3DXVECTOR3(200.f, 0.f, 0.f));
		m_pTitleBackText[0]->SetScale(D3DXVECTOR3(200.f, 120.f, 0.f));
		m_pTitleBackText[0]->SetPatternNo(0.f, 0.f);
		m_pTitleBackText[0]->Render();

		m_pTitleBackText[1]->SetAlpha(1.f);
		m_pTitleBackText[1]->SetPosition(D3DXVECTOR3(400.f, 0.f, 0.f));
		m_pTitleBackText[1]->SetScale(D3DXVECTOR3(200.f, 120.f, 0.f));
		m_pTitleBackText[1]->SetPatternNo(0.f, 3.f);
		m_pTitleBackText[1]->Render();

		m_pTitleBackText[2]->SetAlpha(1.f);
		m_pTitleBackText[2]->SetPosition(D3DXVECTOR3(600.f, 0.f, 0.f));
		m_pTitleBackText[2]->SetScale(D3DXVECTOR3(200.f, 120.f, 0.f));
		m_pTitleBackText[2]->SetPatternNo(0.f, 4.f);
		m_pTitleBackText[2]->Render();
	}
}
