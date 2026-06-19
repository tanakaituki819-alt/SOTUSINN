#pragma once
#include"CSceneTransitionRenderer.h"
//フェード
class CFadeSceneTransition
	:public CSceneTransitionRenderer
{
public:
	CFadeSceneTransition();
	~CFadeSceneTransition()override;
	bool BeforeUpdate(float SceneChengCount, float SceneChengTime)override;//変更前のUpdate、trueを返すとシーンが変わる
	bool AfterUpdate(float SceneChengCount, float SceneChengTime) override;//変更後
	void AfterStart() override;
	void BeforeDraw(double transitionProgress)override;
	void UnderDraw();
	void AfterDraw(double transitionProgress)override;
private:
	CSprite2D* Sprite;
};

