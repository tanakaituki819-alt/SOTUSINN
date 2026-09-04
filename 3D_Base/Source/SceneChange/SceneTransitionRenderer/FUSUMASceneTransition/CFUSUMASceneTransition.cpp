#include "CFUSUMASceneTransition.h"

CFUSUMASceneTransition::CFUSUMASceneTransition()
{
	Sprite = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_FUSUMA);
	Sprite->SetScale({ WND_W/2,WND_H,0 });
	Sprite->SetAlpha(1);

}

CFUSUMASceneTransition::~CFUSUMASceneTransition()
{
	Sprite = nullptr;
}


bool CFUSUMASceneTransition::BeforeUpdate(float SceneChengCount, float SceneChengTime)
{
	if (SceneChengCount > SceneChengTime) {
		return true;
	}
	return false;
}

bool CFUSUMASceneTransition::AfterUpdate(float SceneChengCount, float SceneChengTime)
{
	if (SceneChengCount > SceneChengTime) {
		return true;
	}
	return false;
}

void CFUSUMASceneTransition::AfterStart()
{
}

void CFUSUMASceneTransition::BeforeDraw(double transitionProgress)
{
	Sprite->SetPatternNo(0, 0);
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(-WND_W*0.5+ (WND_W*0.5)* Quadraticfunction(transitionProgress*0.5f));
	Sprite->Render();
	Sprite->SetPatternNo(1, 0);
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(WND_W- (WND_W * 0.5) * Quadraticfunction(transitionProgress*0.5f));
	Sprite->Render();

}

void CFUSUMASceneTransition::UnderDraw()
{
	Sprite->SetPatternNo(0, 0);
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(0);
	Sprite->Render();
	Sprite->SetPatternNo(1, 0);
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(0);
	Sprite->Render();
}

void CFUSUMASceneTransition::AfterDraw(double transitionProgress)
{
	Sprite->SetPatternNo(0, 0);
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(-WND_W * 0.5 + (WND_W * 0.5) * Quadraticfunction(0.5f-transitionProgress*0.5f));
	Sprite->Render();
	Sprite->SetPatternNo(1, 0);
	Sprite->SetPositionY(0);
	Sprite->SetPositionX(WND_W - (WND_W * 0.5) * Quadraticfunction(0.5f-transitionProgress*0.5f));
	Sprite->Render();
	
}
float CFUSUMASceneTransition::Quadraticfunction(float i)
{
	float result = 0;
	result = -4 * ((i - 0.5) * (i - 0.5)) + 1;
	// 例：最大輝度を 1.0 から 2.0（または 200%）に上げたい場合
	//result = (-4 * ((i - 0.5) * (i - 0.5)) + 1) * 2.0f;
	// 例：i = 0.8 のときに明るさのピークを持ってきたい場合
	//result = -4.0f * ((i - 0.8f) * (i - 0.8f)) + 1.0f
	result = std::max(0.0f, result);
	return result;
}