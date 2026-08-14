#include "CResultUI.h"
#include "SpriteManager/CSpriteManager.h"
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

//プレイヤーアイコン
static constexpr Transform PLAYER_1_ICON = {  45.f,100.f,120.f,120.f };
static constexpr Transform PLAYER_2_ICON = { 350.f,100.f,120.f,120.f };
static constexpr Transform PLAYER_3_ICON = { 655.f,100.f,120.f,120.f };
static constexpr Transform PLAYER_4_ICON = { 960.f,100.f,120.f,120.f };

//メダル関連.
static constexpr Transform P1_MEDAL = { 170.f,90.f,85.f,125.f };
static constexpr Transform P2_MEDAL = { 475.f,90.f,85.f,125.f };
static constexpr Transform P3_MEDAL = { 780.f,90.f,85.f,125.f };
static constexpr Transform P4_MEDAL = { 1085.f,90.f,85.f,125.f };

CResultUI::CResultUI()
	: Rank (0)
    , Count(0)
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
}

CResultUI::~CResultUI()
{
	BackGround_Img = nullptr;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
        PlayerBack_Img[i] = nullptr;
		PlayerIcon_Img[i] = nullptr;
	}
    for (int i = 0; i < MEDAL_MAM; i++)
    {
        Medal_Img[i] = nullptr;
    }
}

void CResultUI::Update()
{
    static int Timer = 0; // 切り替え用のタイマー

    Timer++;
    if (Timer >= 10) 
    {
        Timer = 0;   
        Rank++;    

        if (Rank >= 75) 
        {
            Rank = 0;
        }
    }
}

void CResultUI::Draw()
{
	ResultBackUI();
    PlayerBackUI();

    //修正必須!!.
	//仮条件 2026.07.31
    if (Rank == 0)
    {
        Player1First();
        Player2First();
        Player3First();
        Player4First();
    }
    else if (Rank == 1)
    {
        Player1First();
        Player2First();
        Player3First();
        Player4Second();   
    }
    else if (Rank == 2)
    {
        Player1First();
        Player2First();
        Player3Second();
        Player4First();
    }
    else if (Rank == 3)
    {
        Player1First();
        Player2First();
        Player3Second();
        Player4Second();
    }
    else if (Rank == 4)
    {
        Player1First();
        Player2Second();
        Player3First();
        Player4First();
    }
    else if (Rank == 5)
    {
        Player1First();
        Player2Second();
        Player3First();
        Player4Second();
    }
    else if (Rank == 6)
    {
        Player1First();
        Player2Second();
        Player3Second();
        Player4First();
    }
    else if (Rank == 7)
    {
        Player1First();
        Player2Second();
        Player3Second();
        Player4Second();
    }
    else if (Rank == 8)
    {
        Player1Second();
        Player2First();
        Player3First();
        Player4First();
    }
    else if (Rank == 9)
    {
        Player1Second();
        Player2First();
        Player3First();
        Player4Second();
    }
    else if (Rank == 10)
    {
        Player1Second();
        Player2First();
        Player3Second();
        Player4First();
    }
    else if (Rank == 11)
    {
        Player1Second();
        Player2First();
        Player3Second();
        Player4Second();
    }
    else if (Rank == 12)
    {
        Player1Second();
        Player2Second();
        Player3First();
        Player4First();
    }
    else if (Rank == 13)
    {
        Player1Second();
        Player2Second();
        Player3First();
        Player4Second();
    }
    else if (Rank == 14)
    {
        Player1Second();
        Player2Second();
        Player3Second();
        Player4First();
    }
    else if (Rank == 15)
    {
        Player1First();
        Player2First();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 16)
    {
        Player1First();
        Player2First();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 17)
    {
        Player1First();
        Player2Second();
        Player3First();
        Player4Third();
    }
    else if (Rank == 18)
    {
        Player1First();
        Player2Second();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 19)
    {
        Player1First();
        Player2Second();
        Player3Third();
        Player4First();
    }
    else if (Rank == 20)
    {
        Player1First();
        Player2Second();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 21)
    {
        Player1First();
        Player2Second();
        Player3Third();
        Player4Third();
    }
    else if (Rank == 22)
    {
        Player1First();
        Player2Third();
        Player3First();
        Player4Second();
    }
    else if (Rank == 23)
    {
        Player1First();
        Player2Third();
        Player3Second();
        Player4First();
    }
    else if (Rank == 24)
    {
        Player1First();
        Player2Third();
        Player3Second();
        Player4Second();
    }
    else if (Rank == 25)
    {
        Player1First();
        Player2Third();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 26)
    {
        Player1First();
        Player2Third();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 27)
    {
        Player1Second();
        Player2First();
        Player3First();
        Player4Third();
    }
    else if (Rank == 28)
    {
        Player1Second();
        Player2First();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 29)
    {
        Player1Second();
        Player2First();
        Player3Third();
        Player4First();
    }
    else if (Rank == 30)
    {
        Player1Second();
        Player2First();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 31)
    {
        Player1Second();
        Player2First();
        Player3Third();
        Player4Third();
    }
    else if (Rank == 32)
    {
        Player1Second();
        Player2Second();
        Player3First();
        Player4Third();
    }
    else if (Rank == 33)
    {
        Player1Second();
        Player2Second();
        Player3Third();
        Player4First();
    }
    else if (Rank == 34)
    {
        Player1Second();
        Player2Third();
        Player3First();
        Player4First();
    }
    else if (Rank == 35)
    {
        Player1Second();
        Player2Third();
        Player3First();
        Player4Second();
    }
    else if (Rank == 36)
    {
        Player1Second();
        Player2Third();
        Player3First();
        Player4Third();
    }
    else if (Rank == 37)
    {
        Player1Second();
        Player2Third();
        Player3Second();
        Player4First();
    }
    else if (Rank == 38)
    {
        Player1Second();
        Player2Third();
        Player3Third();
        Player4First();
    }
    else if (Rank == 39)
    {
        Player1Third();
        Player2First();
        Player3First();
        Player4Second();
    }
    else if (Rank == 40)
    {
        Player1Third();
        Player2First();
        Player3Second();
        Player4First();
    }
    else if (Rank == 41)
    {
        Player1Third();
        Player2First();
        Player3Second();
        Player4Second();
    }
    else if (Rank == 42)
    {
        Player1Third();
        Player2First();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 43)
    {
        Player1Third();
        Player2First();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 44)
    {
        Player1Third();
        Player2Second();
        Player3First();
        Player4First();
    }
    else if (Rank == 45)
    {
        Player1Third();
        Player2Second();
        Player3First();
        Player4Second();
    }
    else if (Rank == 46)
    {
        Player1Third();
        Player2Second();
        Player3First();
        Player4Third();
    }
    else if (Rank == 47)
    {
        Player1Third();
        Player2Second();
        Player3Second();
        Player4First();
    }
    else if (Rank == 48)
    {
        Player1Third();
        Player2Second();
        Player3Third();
        Player4First();
    }
    else if (Rank == 49)
    {
        Player1Third();
        Player2Third();
        Player3First();
        Player4Second();
    }
    else if (Rank == 50)
    {
        Player1Third();
        Player2Third();
        Player3Second();
        Player4First();
    }
    else if (Rank == 51)
    {
        Player1First();
        Player2Second();
        Player3Third();
        Player4Fourth();
    }
    else if (Rank == 52)
    {
        Player1First();
        Player2Second();
        Player3Fourth();
        Player4Third();
    }
    else if (Rank == 53)
    {
        Player1First();
        Player2Third();
        Player3Second();
        Player4Fourth();
    }
    else if (Rank == 54)
    {
        Player1First();
        Player2Third();
        Player3Fourth();
        Player4Second();
    }
    else if (Rank == 55)
    {
        Player1First();
        Player2Fourth();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 56)
    {
        Player1First();
        Player2Fourth();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 57)
    {
        Player1Second();
        Player2First();
        Player3Third();
        Player4Fourth();
    }
    else if (Rank == 58)
    {
        Player1Second();
        Player2First();
        Player3Fourth();
        Player4Third();
    }
    else if (Rank == 59)
    {
        Player1Second();
        Player2Third();
        Player3First();
        Player4Fourth();
    }
    else if (Rank == 60)
    {
        Player1Second();
        Player2Third();
        Player3Fourth();
        Player4First();
    }
    else if (Rank == 61)
    {
        Player1Second();
        Player2Fourth();
        Player3First();
        Player4Third();
    }
    else if (Rank == 62)
    {
        Player1Second();
        Player2Fourth();
        Player3Third();
        Player4First();
    }
    else if (Rank == 63)
    {
        Player1Third();
        Player2First();
        Player3Second();
        Player4Fourth();
    }
    else if (Rank == 64)
    {
        Player1Third();
        Player2First();
        Player3Fourth();
        Player4Second();
    }
    else if (Rank == 65)
    {
        Player1Third();
        Player2Second();
        Player3First();
        Player4Fourth();
    }
    else if (Rank == 66)
    {
        Player1Third();
        Player2Second();
        Player3Fourth();
        Player4First();
    }
    else if (Rank == 67)
    {
        Player1Third();
        Player2Fourth();
        Player3First();
        Player4Second();
    }
    else if (Rank == 68)
    {
        Player1Third();
        Player2Fourth();
        Player3Second();
        Player4First();
    }
    else if (Rank == 69)
    {
        Player1Fourth();
        Player2First();
        Player3Second();
        Player4Third();
    }
    else if (Rank == 70)
    {
        Player1Fourth();
        Player2First();
        Player3Third();
        Player4Second();
    }
    else if (Rank == 71)
    {
        Player1Fourth();
        Player2Second();
        Player3First();
        Player4Third();
    }
    else if (Rank == 72)
    {
        Player1Fourth();
        Player2Second();
        Player3Third();
        Player4First();
    }
    else if (Rank == 73)
    {
        Player1Fourth();
        Player2Third();
        Player3First();
        Player4Second();
    }
    else if (Rank == 74)
    {
        Player1Fourth();
        Player2Third();
        Player3Second();
        Player4First();
    }

    

}

void CResultUI::ResultBackUI()
{
	BackGround_Img->SetPosition(D3DXVECTOR3(0, 0, 0));
	BackGround_Img->SetScale(D3DXVECTOR3(WND_W, WND_H, 0));
	BackGround_Img->Render();
}

void CResultUI::PlayerBackUI()
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
		PlayerIcon_Img[i]->SetScale(D3DXVECTOR3(PLAYER_1_ICON .GetScl()));
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
	PlayerIcon_Img[1]->SetPatternNo(3, 0);
	PlayerIcon_Img[1]->Render();
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