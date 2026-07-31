#include "CSceneTransition.h"

CSceneTransition::CSceneTransition()
{
	Sprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Fad);
	Sprite->SetScale({ WND_W,WND_H,0 });
	Sprite->SetAlpha(1);

}

CSceneTransition::~CSceneTransition()
{
	Sprite = nullptr;
}

bool CSceneTransition::BeforeUpdate(float SceneChengCount, float SceneChengTime)
{
	if (SceneChengCount > SceneChengTime) {
		return true;
	}
	return false;
}

bool CSceneTransition::AfterUpdate(float SceneChengCount, float SceneChengTime)
{
	if (SceneChengCount > SceneChengTime) {
		return true;
	}
	return false;
}

void CSceneTransition::AfterStart()
{
}

void CSceneTransition::BeforeDraw(double transitionProgress)
{
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(WND_W*(1- transitionProgress));
	Sprite->Render();
	
}

void CSceneTransition::UnderDraw()
{
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(0);
	Sprite->Render();
}

void CSceneTransition::AfterDraw(double transitionProgress)
{
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(WND_W*(transitionProgress));
	Sprite->Render();
}
