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

CResultUI::CResultUI()
	: Rank (0)
    , Count(0)
{
	BackGround_Img = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_BackGround);	//背景.
	for (int i = 0; i < Player_Max; i++)
	{
        PlayerBack_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_PlayerBackground);	//背景.
		PlayerIcon_Img[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Playericon);	//プレイヤーアイコン.
	}
}

CResultUI::~CResultUI()
{
	BackGround_Img = nullptr;
	for (int i = 0; i < Player_Max; i++)
	{
        PlayerBack_Img[i] = nullptr;
		PlayerIcon_Img[i] = nullptr;
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

void CResultUI::Player1BackUI()
{
    PlayerBack_Img[0]->SetPosition(D3DXVECTOR3(0, 0, 0));
    PlayerBack_Img[0]->SetScale(D3DXVECTOR3(100, 100, 0));
    PlayerBack_Img[0]->SetPatternNo(0, 0);
    PlayerBack_Img[0]->Render();

}

void CResultUI::Player2BackUI()
{
}

void CResultUI::Player3BackUI()
{
}

void CResultUI::Player4BackUI()
{
}

void CResultUI::Player1Pos()
{
	PlayerIcon_Img[0]->SetPosition(D3DXVECTOR3(100, 0, 0));
}

void CResultUI::Player2Pos()
{
	PlayerIcon_Img[1]->SetPosition(D3DXVECTOR3(200, 0, 0));
}

void CResultUI::Player3Pos()
{
	PlayerIcon_Img[2]->SetPosition(D3DXVECTOR3(300, 0, 0));
}

void CResultUI::Player4Pos()
{
	PlayerIcon_Img[3]->SetPosition(D3DXVECTOR3(400, 0, 0));
}

void CResultUI::PlayerScl()
{
	for (int i = 0; i < Player_Max; i++)
	{
		PlayerIcon_Img[i]->SetScale(D3DXVECTOR3(100, 100, 0));
	}
}


void CResultUI::Player1First()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(0,0);
	PlayerIcon_Img[0]->Render();
}

void CResultUI::Player1Second()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(1, 0);
	PlayerIcon_Img[0]->Render();
}

void CResultUI::Player1Third()
{
	Player1Pos();
	PlayerScl();
	PlayerIcon_Img[0]->SetPatternNo(2, 0);
	PlayerIcon_Img[0]->Render();
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

}

void CResultUI::Player2Second()
{
	Player2Pos();
	PlayerScl();
	PlayerIcon_Img[1]->SetPatternNo(1, 1);
	PlayerIcon_Img[1]->Render();

}

void CResultUI::Player2Third()
{
	Player2Pos();
	PlayerScl();
	PlayerIcon_Img[1]->SetPatternNo(2, 1);
	PlayerIcon_Img[1]->Render();

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

}

void CResultUI::Player3Second()
{
	Player3Pos();
	PlayerScl();
	PlayerIcon_Img[2]->SetPatternNo(1, 2);
	PlayerIcon_Img[2]->Render();

}

void CResultUI::Player3Third()
{
	Player3Pos();
	PlayerScl();
	PlayerIcon_Img[2]->SetPatternNo(2, 2);
	PlayerIcon_Img[2]->Render();

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

}

void CResultUI::Player4Second()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(1, 3);
	PlayerIcon_Img[3]->Render();

}

void CResultUI::Player4Third()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(2, 3);
	PlayerIcon_Img[3]->Render();

}

void CResultUI::Player4Fourth()
{
	Player4Pos();
	PlayerScl();
	PlayerIcon_Img[3]->SetPatternNo(3, 3);
	PlayerIcon_Img[3]->Render();

}
