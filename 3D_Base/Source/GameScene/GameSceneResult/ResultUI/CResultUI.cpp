#include "CResultUI.h"
#include "SpriteManager/CSpriteManager.h"
#include "System/XInput/CXInput.h"
#include "ResultDataManager/CResultDataManager.h"
#include <iostream>

struct Transform {
	float posX, posY;
	float sclX, sclY;

	//D3DXVECTOR3に変換する関数.
	D3DXVECTOR3 GetPos() const { return D3DXVECTOR3(posX, posY, 0.0f); }
	D3DXVECTOR3 GetScl() const { return D3DXVECTOR3(sclX, sclY, 0.0f); }
};
//90度
static constexpr float IMAGE_ROTATION_ANGLE = 1.5707963;

//プレイヤー背景
static constexpr Transform PLAYER_1_BACK = { 155.f,340.f,630.f,350.f };
static constexpr Transform PLAYER_2_BACK = { 290.f,340.f,630.f,350.f };
static constexpr Transform PLAYER_3_BACK = { 765.f,340.f,630.f,350.f };
static constexpr Transform PLAYER_4_BACK = { 1070.f,340.f,630.f,350.f };

//プレイヤーアイコン(人数に関係なくプレイヤーごとに位置は固定).
static constexpr Transform PLAYER_1_ICON = { 45.f,100.f,120.f,120.f };
static constexpr Transform PLAYER_2_ICON = { 350.f,100.f,120.f,120.f };
static constexpr Transform PLAYER_3_ICON = { 655.f,100.f,120.f,120.f };
static constexpr Transform PLAYER_4_ICON = { 960.f,100.f,120.f,120.f };


//メダル関連.
static constexpr Transform P1_MEDAL = { 170.f,90.f,85.f,125.f };
static constexpr Transform P2_MEDAL = { 475.f,90.f,85.f,125.f };
static constexpr Transform P3_MEDAL = { 780.f,90.f,85.f,125.f };
static constexpr Transform P4_MEDAL = { 1085.f,90.f,85.f,125.f };

//フォント関連.
static constexpr Transform FONT_SCALE = { 0.f,0.f,210.f,45.f };

//四人の時
//プレイヤー壱
static constexpr Transform P1_FONT_1 = { 45.f, 215.f };     //かくとくポイント
static constexpr Transform P1_FONT_2 = { 45.f, 270.f };     //具材ポイント
static constexpr Transform P1_FONT_3 = { 45.f, 360.f };     //高級具材ポイント
static constexpr Transform P1_FONT_4 = { 45.f, 450.f };     //合計ポイント
//プレイヤー弐
static constexpr Transform P2_FONT_1 = { 350.f, 215.f };    //かくとくポイント
static constexpr Transform P2_FONT_2 = { 350.f, 270.f };    //具材ポイント
static constexpr Transform P2_FONT_3 = { 350.f, 360.f };    //高級具材ポイント
static constexpr Transform P2_FONT_4 = { 350.f, 450.f };    //合計ポイント
//プレイヤー参
static constexpr Transform P3_FONT_1 = { 655.f, 215.f };    //かくとくポイント
static constexpr Transform P3_FONT_2 = { 655.f, 270.f };    //具材ポイント
static constexpr Transform P3_FONT_3 = { 655.f, 360.f };    //高級具材ポイント
static constexpr Transform P3_FONT_4 = { 655.f, 450.f };    //合計ポイント
//プレイヤー肆
static constexpr Transform P4_FONT_1 = { 960.f, 215.f };    //かくとくポイント
static constexpr Transform P4_FONT_2 = { 960.f, 270.f };    //具材ポイント
static constexpr Transform P4_FONT_3 = { 960.f, 360.f };    //高級具材ポイント
static constexpr Transform P4_FONT_4 = { 960.f, 450.f };    //合計ポイント



//数字の1文字あたりの幅高さ.
static constexpr float NUMBER_SIZE_W = 30.f;
static constexpr float NUMBER_SIZE_H = 45.f;

//各プレイヤーの合計スコア表示位置.
static constexpr Transform P1_ALL_SCORE_POS = { 90.f,  500.f };
static constexpr Transform P2_ALL_SCORE_POS = { 395.f, 500.f };
static constexpr Transform P3_ALL_SCORE_POS = { 700.f, 500.f };
static constexpr Transform P4_ALL_SCORE_POS = { 1005.f, 500.f };


//ReStart関連.
static constexpr Transform RESTART = { 490.f,603.f,380.f,120.f };
static constexpr Transform FIN = { 880.f,610.f,380.f,120.f };

CResultUI::CResultUI()
    : m_ADecided(false)
    , m_BDecided(false)
    , m_pController(nullptr)
{
	BackGround_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround);	//背景.
	for (int i = 0; i < PLAYER_MAX; i++)
	{
        PlayerBack_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll2);	//プレイヤー背景.
		PlayerIcon_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Playericon);	//プレイヤーアイコン.
	}
    for (int i = 0; i < MEDAL_MAM; i++)
    {
        Medal_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_Medal);
}
    for (int i = 0; i < FONT_MAM; i++)
    {
        Font_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_ResultFont);      //フォント.
    }
    Number_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Digit0_9);	//数字.
    for (int i = 0; i < RESTART_MAM; i++)
    {
        ReStart_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_ReStart);      //フォント.
    }
    PushA = true;
}

CResultUI::~CResultUI()
{
    for (int i = 0; i < RESTART_MAM; i++)
    {
        ReStart_Img[i] = nullptr;
    }
    Number_Img = nullptr;
    for (int i = 0; i < FONT_MAM; i++)
	{
        Font_Img[i] = nullptr;
    }
    for (int i = 0; i < MEDAL_MAM; i++)
    {
        Medal_Img[i] = nullptr;
    }
	for (int i = 0; i < PLAYER_MAX; i++)
	{
        PlayerBack_Img[i] = nullptr;
		PlayerIcon_Img[i] = nullptr;
	}
    BackGround_Img = nullptr;

}

void CResultUI::Update()
{
    //Aボタン.
    if (!PushA) {
        if (m_pController->IsDown(CXInput::A, true))
        {
            m_ADecided = true;
        }
    }


    //Bボタン.
    if (m_pController->IsDown(CXInput::B, true))
    {
        m_BDecided = true;
    }
    PushA= m_pController->IsDown(CXInput::A, true);
}

void CResultUI::Draw()
{
	ResultBackUI();

    ReStart();

    FourPlayerBackUI();

    //ゲームの結果(CResultDataManager)をもとに描画.
    DrawPlayers();
}

//参加しているプレイヤーのフォント、スコア、順位アイコンを描画する.
void CResultUI::DrawPlayers()
{
    const CResultDataManager* pData = CResultDataManager::GetInstance();

    for (int i = 0; i < PLAYER_MAX; i++)
    {
        const SPlayerResultData& Data = pData->GetPlayerData(i);

        //参加していないプレイヤーは表示しない.
        if (!Data.m_bActive) continue;

        //フォント.
        switch (i)
        {
        case 0: Font_P1_UI(); break;
        case 1: Font_P2_UI(); break;
        case 2: Font_P3_UI(); break;
        case 3: Font_P4_UI(); break;
        }

        //スコア.
        switch (i)
        {
        case 0: Number_P1_UI(); break;
        case 1: Number_P2_UI(); break;
        case 2: Number_P3_UI(); break;
        case 3: Number_P4_UI(); break;
        }

        //順位アイコンとメダル.
        DrawPlayerRank(i, Data.m_nRank);
    }
}

//プレイヤー番号と順位から対応するアイコンを描画する.
void CResultUI::DrawPlayerRank(int PlayerIndex, int Rank)
{
    switch (PlayerIndex)
    {
    case 0:
        if      (Rank == 1) Player1First();
        else if (Rank == 2) Player1Second();
        else if (Rank == 3) Player1Third();
        else                Player1Fourth();
        break;
    case 1:
        if      (Rank == 1) Player2First();
        else if (Rank == 2) Player2Second();
        else if (Rank == 3) Player2Third();
        else                Player2Fourth();
        break;
    case 2:
        if      (Rank == 1) Player3First();
        else if (Rank == 2) Player3Second();
        else if (Rank == 3) Player3Third();
        else                Player3Fourth();
        break;
    case 3:
        if      (Rank == 1) Player4First();
        else if (Rank == 2) Player4Second();
        else if (Rank == 3) Player4Third();
        else                Player4Fourth();
        break;
    }
}
void CResultUI::BoolInit()
{
    m_ADecided = false;
    m_BDecided = false;
}

void CResultUI::ResultBackUI()
{
	BackGround_Img->SetPosition(D3DXVECTOR3(0, 0, 0));
	BackGround_Img->SetScale(D3DXVECTOR3(WND_W, WND_H, 0));
	BackGround_Img->Render();
}

void CResultUI::FourPlayerBackUI()
{
    PlayerBack_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_1_BACK.GetPos()));
    PlayerBack_Img[0]->SetScale(D3DXVECTOR3(PLAYER_1_BACK.GetScl()));
    PlayerBack_Img[0]->SetRotationZ(IMAGE_ROTATION_ANGLE);
    PlayerBack_Img[0]->Render();

    PlayerBack_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_2_BACK.GetPos()));
    PlayerBack_Img[1]->SetPosition(D3DXVECTOR3(460,340,0));
    PlayerBack_Img[1]->SetScale(D3DXVECTOR3(PLAYER_2_BACK.GetScl()));
    PlayerBack_Img[1]->SetRotationZ(IMAGE_ROTATION_ANGLE);
    PlayerBack_Img[1]->Render();

    PlayerBack_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_3_BACK.GetPos()));
    PlayerBack_Img[2]->SetScale(D3DXVECTOR3(PLAYER_3_BACK.GetScl()));
    PlayerBack_Img[2]->SetRotationZ(IMAGE_ROTATION_ANGLE);
    PlayerBack_Img[2]->Render();

    PlayerBack_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_4_BACK.GetPos()));
    PlayerBack_Img[3]->SetScale(D3DXVECTOR3(PLAYER_4_BACK.GetScl()));
    PlayerBack_Img[3]->SetRotationZ(IMAGE_ROTATION_ANGLE);
    PlayerBack_Img[3]->Render();

}

void CResultUI::Player1Pos()
{
    PlayerIcon_Img[0]->SetPosition(D3DXVECTOR3(PLAYER_1_ICON.GetPos()));
}
void CResultUI::Player2Pos()
{
    PlayerIcon_Img[1]->SetPosition(D3DXVECTOR3(PLAYER_2_ICON.GetPos()));
}
void CResultUI::Player3Pos()
{
    PlayerIcon_Img[2]->SetPosition(D3DXVECTOR3(PLAYER_3_ICON.GetPos()));
}
void CResultUI::Player4Pos()
{
    PlayerIcon_Img[3]->SetPosition(D3DXVECTOR3(PLAYER_4_ICON.GetPos()));
}

void CResultUI::PlayerScl()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		PlayerIcon_Img[i]->SetScale(D3DXVECTOR3(PLAYER_1_ICON.GetScl()));
	}
}


void CResultUI::Player1First()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(0,0);
	PlayerIcon_Img[0]->Render();
    Medal_P1_1st_UI();
}
void CResultUI::Player1Second()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(1, 0);
	PlayerIcon_Img[0]->Render();
    Medal_P1_2nd_UI();
}
void CResultUI::Player1Third()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(2, 0);
	PlayerIcon_Img[0]->Render();
    Medal_P1_3rd_UI();

}

void CResultUI::Player1Fourth()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(3, 0);
	PlayerIcon_Img[0]->Render();
}

void CResultUI::Player2First()
{
	Player2Pos();
	PlayerScl();
	PlayerIcon_Img[1]->SetPatternNo(0, 1);
	PlayerIcon_Img[1]->Render();
    Medal_P2_1st_UI();

}
void CResultUI::Player2Second()
{
	Player2Pos();
	PlayerScl();
	PlayerIcon_Img[1]->SetPatternNo(1, 1);
	PlayerIcon_Img[1]->Render();
    Medal_P2_2nd_UI();

}
void CResultUI::Player2Third()
{
	Player2Pos();
	PlayerScl();
	PlayerIcon_Img[1]->SetPatternNo(2, 1);
	PlayerIcon_Img[1]->Render();
    Medal_P2_3rd_UI();

}
void CResultUI::Player2Fourth()
{
	Player2Pos();
	PlayerScl();
	PlayerIcon_Img[1]->SetPatternNo(3, 1);
	PlayerIcon_Img[1]->Render();

}

void CResultUI::Player3First()
{
	Player3Pos();
	PlayerScl();
	PlayerIcon_Img[2]->SetPatternNo(0, 2);
	PlayerIcon_Img[2]->Render();
    Medal_P3_1st_UI();
}
void CResultUI::Player3Second()
{
	Player3Pos();
	PlayerScl();
	PlayerIcon_Img[2]->SetPatternNo(1, 2);
	PlayerIcon_Img[2]->Render();
    Medal_P3_2nd_UI();
}
void CResultUI::Player3Third()
{
	Player3Pos();
	PlayerScl();
	PlayerIcon_Img[2]->SetPatternNo(2, 2);
	PlayerIcon_Img[2]->Render();
    Medal_P3_3rd_UI();
}
void CResultUI::Player3Fourth()
{
	Player3Pos();
	PlayerScl();
	PlayerIcon_Img[2]->SetPatternNo(3, 2);
	PlayerIcon_Img[2]->Render();

}

void CResultUI::Player4First()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(0, 3);
	PlayerIcon_Img[3]->Render();
    Medal_P4_1st_UI();
}
void CResultUI::Player4Second()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(1, 3);
	PlayerIcon_Img[3]->Render();
    Medal_P4_2nd_UI();
}
void CResultUI::Player4Third()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(2, 3);
	PlayerIcon_Img[3]->Render();
    Medal_P4_3rd_UI();
}
void CResultUI::Player4Fourth()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(3, 3);
	PlayerIcon_Img[3]->Render();

}


//プレイヤー壱のメダルUI
void CResultUI::Medal_P1_1st_UI()
{
    Medal_Img[0]->SetPosition(D3DXVECTOR3(P1_MEDAL.GetPos()));
    Medal_Gold_Render();
}
void CResultUI::Medal_P1_2nd_UI()
{
    Medal_Img[1]->SetPosition(D3DXVECTOR3(P1_MEDAL.GetPos()));
    Medal_Silver_Render();
}
void CResultUI::Medal_P1_3rd_UI()
{
    Medal_Img[2]->SetPosition(D3DXVECTOR3(P1_MEDAL.GetPos()));
    Medal_Bronze_Render();
}

//プレイヤー弐のメダルUI
void CResultUI::Medal_P2_1st_UI()
{
    Medal_Img[0]->SetPosition(D3DXVECTOR3(P2_MEDAL.GetPos()));
    Medal_Gold_Render();
}
void CResultUI::Medal_P2_2nd_UI()
{
    Medal_Img[1]->SetPosition(D3DXVECTOR3(P2_MEDAL.GetPos()));
    Medal_Silver_Render();
}
void CResultUI::Medal_P2_3rd_UI()
{
    Medal_Img[2]->SetPosition(D3DXVECTOR3(P2_MEDAL.GetPos()));
    Medal_Bronze_Render();
}

//プレイヤー参のメダルUI
void CResultUI::Medal_P3_1st_UI()
{
    Medal_Img[0]->SetPosition(D3DXVECTOR3(P3_MEDAL.GetPos()));
    Medal_Gold_Render();
}
void CResultUI::Medal_P3_2nd_UI()
{
    Medal_Img[1]->SetPosition(D3DXVECTOR3(P3_MEDAL.GetPos()));
    Medal_Silver_Render();
}
void CResultUI::Medal_P3_3rd_UI()
{
    Medal_Img[2]->SetPosition(D3DXVECTOR3(P3_MEDAL.GetPos()));
    Medal_Bronze_Render();
}

//プレイヤー肆のメダルUI
void CResultUI::Medal_P4_1st_UI()
{
    Medal_Img[0]->SetPosition(D3DXVECTOR3(P4_MEDAL.GetPos()));
    Medal_Gold_Render();
}
void CResultUI::Medal_P4_2nd_UI()
{
    Medal_Img[1]->SetPosition(D3DXVECTOR3(P4_MEDAL.GetPos()));
    Medal_Silver_Render();
}
void CResultUI::Medal_P4_3rd_UI()
{
    Medal_Img[2]->SetPosition(D3DXVECTOR3(P4_MEDAL.GetPos()));
    Medal_Bronze_Render();
}

//メダルのスケール、パターンナンバー、レンダ関数をまとめた関数.
void CResultUI::Medal_Gold_Render()
{
    Medal_Img[0]->SetScale(D3DXVECTOR3(P1_MEDAL.GetScl()));
    Medal_Img[0]->SetPatternNo(0.f, 0.f);
    Medal_Img[0]->Render();
}
void CResultUI::Medal_Silver_Render()
{
    Medal_Img[1]->SetScale(D3DXVECTOR3(P1_MEDAL.GetScl()));
    Medal_Img[1]->SetPatternNo(1.f, 0.f);
    Medal_Img[1]->Render();
}
void CResultUI::Medal_Bronze_Render()
{
    Medal_Img[2]->SetScale(D3DXVECTOR3(P1_MEDAL.GetScl()));
    Medal_Img[2]->SetPatternNo(2.f, 0.f);
    Medal_Img[2]->Render();
}

void CResultUI::Font_P1_UI()
{
    Font_P1_1st_UI();
    Font_P1_2nd_UI();
    Font_P1_3rd_UI();
    Font_P1_4th_UI();
}
void CResultUI::Font_P2_UI()
{
    Font_P2_1st_UI();
    Font_P2_2nd_UI();
    Font_P2_3rd_UI();
    Font_P2_4th_UI();
}
void CResultUI::Font_P3_UI()
{
    Font_P3_1st_UI();
    Font_P3_2nd_UI();
    Font_P3_3rd_UI();
    Font_P3_4th_UI();
}
void CResultUI::Font_P4_UI()
{
    Font_P4_1st_UI();
    Font_P4_2nd_UI();
    Font_P4_3rd_UI();
    Font_P4_4th_UI();
}

//プレイヤー壱.
void CResultUI::Font_P1_1st_UI()
{
    Font_Img[0]->SetPosition(D3DXVECTOR3(P1_FONT_1.GetPos()));
    Font_1st_Render();
}
void CResultUI::Font_P1_2nd_UI()
{
    Font_Img[1]->SetPosition(D3DXVECTOR3(P1_FONT_2.GetPos()));
    Font_2nd_Render();
}
void CResultUI::Font_P1_3rd_UI()
{
    Font_Img[2]->SetPosition(D3DXVECTOR3(P1_FONT_3.GetPos()));
    Font_3rd_Render();
}
void CResultUI::Font_P1_4th_UI()
{
    Font_Img[3]->SetPosition(D3DXVECTOR3(P1_FONT_4.GetPos()));
    Font_4th_Render();
}

//プレイヤー弐.
void CResultUI::Font_P2_1st_UI()
{
    Font_Img[0]->SetPosition(D3DXVECTOR3(P2_FONT_1.GetPos()));
    Font_1st_Render();
}
void CResultUI::Font_P2_2nd_UI()
{
    Font_Img[1]->SetPosition(D3DXVECTOR3(P2_FONT_2.GetPos()));
    Font_2nd_Render();
}
void CResultUI::Font_P2_3rd_UI()
{
    Font_Img[2]->SetPosition(D3DXVECTOR3(P2_FONT_3.GetPos()));
    Font_3rd_Render();
}
void CResultUI::Font_P2_4th_UI()
{
    Font_Img[3]->SetPosition(D3DXVECTOR3(P2_FONT_4.GetPos()));
    Font_4th_Render();
}

//プレイヤー参.
void CResultUI::Font_P3_1st_UI()
{
    Font_Img[0]->SetPosition(D3DXVECTOR3(P3_FONT_1.GetPos()));
    Font_1st_Render();
}
void CResultUI::Font_P3_2nd_UI()
{
    Font_Img[1]->SetPosition(D3DXVECTOR3(P3_FONT_2.GetPos()));
    Font_2nd_Render();
}
void CResultUI::Font_P3_3rd_UI()
{
    Font_Img[2]->SetPosition(D3DXVECTOR3(P3_FONT_3.GetPos()));
    Font_3rd_Render();
}
void CResultUI::Font_P3_4th_UI()
{
    Font_Img[3]->SetPosition(D3DXVECTOR3(P3_FONT_4.GetPos()));
    Font_4th_Render();
}

//プレイヤー肆.
void CResultUI::Font_P4_1st_UI()
{
    Font_Img[0]->SetPosition(D3DXVECTOR3(P4_FONT_1.GetPos()));
    Font_1st_Render();
}
void CResultUI::Font_P4_2nd_UI()
{
    Font_Img[1]->SetPosition(D3DXVECTOR3(P4_FONT_2.GetPos()));
    Font_2nd_Render();
}
void CResultUI::Font_P4_3rd_UI()
{
    Font_Img[2]->SetPosition(D3DXVECTOR3(P4_FONT_3.GetPos()));
    Font_3rd_Render();
}
void CResultUI::Font_P4_4th_UI()
{
    Font_Img[3]->SetPosition(D3DXVECTOR3(P4_FONT_4.GetPos()));
    Font_4th_Render();
}

//フォントのスケール、パターンナンバー、レンダ関数をまとめた関数.
void CResultUI::Font_1st_Render()
{
    Font_Img[0]->SetScale(D3DXVECTOR3(FONT_SCALE.GetScl()));
    Font_Img[0]->SetPatternNo(0.f, 0.f);	//1コマ目：かくとくポイント.
    Font_Img[0]->Render();
}
void CResultUI::Font_2nd_Render()
{
    Font_Img[1]->SetScale(D3DXVECTOR3(FONT_SCALE.GetScl()));
    Font_Img[1]->SetPatternNo(0.f, 1.f);	//2コマ目：具材ポイント.
    Font_Img[1]->Render();
}
void CResultUI::Font_3rd_Render()
{
    Font_Img[2]->SetScale(D3DXVECTOR3(FONT_SCALE.GetScl()));
    Font_Img[2]->SetPatternNo(0.f, 2.f);	//3コマ目：高級具材ポイント.
    Font_Img[2]->Render();
}
void CResultUI::Font_4th_Render()
{
    Font_Img[3]->SetScale(D3DXVECTOR3(FONT_SCALE.GetScl()));
    Font_Img[3]->SetPatternNo(0.f, 3.f);	//4コマ目：合計ポイント.
    Font_Img[3]->Render();
}
void CResultUI::DrawNumber(int Score, const D3DXVECTOR3& Pos)
{
    //上限仮999.
    int S = Score;
    if (S > 999) {
        S = 999;
    }

    int M = 100;	//百の位から.
    for (int i = 0; i < 3; i++)
    {
        Number_Img->SetPosition(D3DXVECTOR3(Pos.x + NUMBER_SIZE_W * i, Pos.y, 0.f));
        Number_Img->SetScale(D3DXVECTOR3(NUMBER_SIZE_W, NUMBER_SIZE_H, 0.f));
        Number_Img->SetPatternNo(static_cast<SHORT>(S / M), 0);
        Number_Img->Render();

        S -= (S / M) * M;
        M /= 10;
    }

    //点.
    Number_Img->SetPosition(D3DXVECTOR3(Pos.x + NUMBER_SIZE_W * 3, Pos.y, 0.f));
    Number_Img->SetScale(D3DXVECTOR3(NUMBER_SIZE_W, NUMBER_SIZE_H, 0.f));
    Number_Img->SetPatternNo(10, 0);
    Number_Img->Render();
}

//CResultDataManagerから取得した合計スコアを表示する.
void CResultUI::Number_P1_UI()
{
    int Score = CResultDataManager::GetInstance()->GetPlayerData(0).m_nScore;
    DrawNumber(Score, D3DXVECTOR3(P1_ALL_SCORE_POS.GetPos()));
}
void CResultUI::Number_P2_UI()
{
    int Score = CResultDataManager::GetInstance()->GetPlayerData(1).m_nScore;
    DrawNumber(Score, D3DXVECTOR3(P2_ALL_SCORE_POS.GetPos()));
}
void CResultUI::Number_P3_UI()
{
    int Score = CResultDataManager::GetInstance()->GetPlayerData(2).m_nScore;
    DrawNumber(Score, D3DXVECTOR3(P3_ALL_SCORE_POS.GetPos()));
}
void CResultUI::Number_P4_UI()
{
    int Score = CResultDataManager::GetInstance()->GetPlayerData(3).m_nScore;
    DrawNumber(Score, D3DXVECTOR3(P4_ALL_SCORE_POS.GetPos()));
}

void CResultUI::ReStart()
{
    //実行中に動かすやつ
    static float ananana = 10;
    static float ananana2 = 10;

    if (GetAsyncKeyState('W') & 0x8000)
    {
        ananana--;
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        ananana++;
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        ananana2++;
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        ananana2--;
    }

    FILE* pFile;
    //stdout（標準出力）を新しく作成したコンソールにリダイレクト
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    std::cout << ananana << std::endl;


    //もう一度プレイ
    ReStart_Img[0]->SetPosition(D3DXVECTOR3(RESTART.GetPos()));
    ReStart_Img[0]->SetScale(D3DXVECTOR3(RESTART.GetScl()));
    ReStart_Img[0]->SetPatternNo(0.f, 0.f);
    ReStart_Img[0]->Render();
    //人数選択画面へ
    ReStart_Img[1]->SetPosition(D3DXVECTOR3(FIN.GetPos()));
    ReStart_Img[1]->SetScale(D3DXVECTOR3(FIN.GetScl()));
    ReStart_Img[1]->SetPatternNo(0.f, 1.f);
    ReStart_Img[1]->Render();
}
