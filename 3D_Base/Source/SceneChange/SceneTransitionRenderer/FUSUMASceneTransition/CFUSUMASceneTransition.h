#pragma once
#include"SceneChange\SceneTransitionRenderer\CSceneTransitionRenderer.h"
//右からのシャッター
class CFUSUMASceneTransition
	:public CSceneTransitionRenderer
{
public:
	CFUSUMASceneTransition();
	~CFUSUMASceneTransition()override;
	bool BeforeUpdate(float SceneChengCount, float SceneChengTime)override;//変更前のUpdate、trueを返すとシーンが変わる
	bool AfterUpdate(float SceneChengCount, float SceneChengTime) override;//変更後
	void AfterStart() override;
	void BeforeDraw(double transitionProgress)override;
	void UnderDraw();
	void AfterDraw(double transitionProgress)override;
private:
	CSprite2D* Sprite;
	float Quadraticfunction(float i);
};
