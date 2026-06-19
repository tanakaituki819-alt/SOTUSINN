#include "CFadeSceneTransition.h"

CFadeSceneTransition::CFadeSceneTransition()
{
	
	Sprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Fad);
	Sprite->SetScale({ WND_W,WND_H,0 });
	Sprite->SetPosition({ 0,0,0 });
	Sprite->SetAlpha(1);
}

CFadeSceneTransition::~CFadeSceneTransition()
{
	Sprite = nullptr;
}

bool CFadeSceneTransition::BeforeUpdate(float SceneChengCount, float SceneChengTime)
{
	if (SceneChengCount > SceneChengTime) {
		return true;
	}
	return false;
}

bool CFadeSceneTransition::AfterUpdate(float SceneChengCount, float SceneChengTime)
{
	if (SceneChengCount > SceneChengTime) {
		return true;
	}
	return false;
}

void CFadeSceneTransition::AfterStart()
{
}

void CFadeSceneTransition::BeforeDraw(double transitionProgress)
{
	Sprite->SetAlpha(transitionProgress);
	Sprite->Render();
	Sprite->SetAlpha(1);
}

void CFadeSceneTransition::UnderDraw()
{
	Sprite->Render();
}

void CFadeSceneTransition::AfterDraw(double transitionProgress)
{
	
	Sprite->SetAlpha(1- transitionProgress);
	Sprite->Render();
	Sprite->SetAlpha(1);
}
