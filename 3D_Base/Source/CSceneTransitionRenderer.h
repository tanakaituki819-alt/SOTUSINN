#pragma once

class CSceneTransitionRenderer
{
public:
	CSceneTransitionRenderer();
	virtual~CSceneTransitionRenderer();
	virtual bool BeforeUpdate(float SceneChengCount,float SceneChengTime) = 0;//変更前のUpdate、trueを返すとシーンが変わる
	virtual bool AfterUpdate(float SceneChengCount, float SceneChengTime) = 0;//変更後

	virtual void AfterStart() = 0;
	//描画関数(画像の表示処理を行う)
	virtual void BeforeDraw(double transitionProgress)=0;//変更前描画(transitionProgressには０から１がはいる)
	virtual void UnderDraw()=0;//画面が埋まった時の描画
	virtual void AfterDraw(double transitionProgress)=0;//変更後の描画
protected:

};
