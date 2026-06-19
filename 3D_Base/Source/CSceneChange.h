#pragma once
#include "CSceneTransitionRenderer.h"
#include"CFadeSceneTransition.h"
#include"CSceneTransition.h"

//シーンチェンジの動作クラス
class CSceneChange
{
public:
	CSceneChange();
	~CSceneChange();
	enum class SceneChange {
		none,		//なし
		Beforechange,//変更前//フェード中
		Underchange,//変更中//画面が真っ黒になってる状態
		Afterchange,//変更後//フェードを戻す状態
	};
	enum class TransitionType {
		Fade,//フェード
		RSRIDE,//横からのシャッタ
		Shutter,//上からのシャッタ-
	};

	//更新関数(キー入力や動作処理を行う)
	void Update();

	//描画関数(画像の表示処理を行う)
	void Draw();

	SceneChange GetSceneChangeFlag() {
		return SceneChangeFlag;
	};
	//シーンチェンジ開始
	void SetSceneChangeFlag(int Time=60, TransitionType Type= TransitionType::Fade) {
		if (SceneChangeFlag== SceneChange::none) {
			SceneChengTime = Time;
			SceneChengCount = 0;
		}
		switch (Type)
		{
		case CSceneChange::TransitionType::Fade:
			SceneChangeClass = new CFadeSceneTransition();
			break;
		case CSceneChange::TransitionType::RSRIDE:
			SceneChangeClass = new CSceneTransition();
			break;
		}

		SceneChangeFlag = SceneChange::Beforechange;

	};

	//シーンチェンジ終了
	void endSceneChange(int Time = 60) {
		if (SceneChangeFlag == SceneChange::Underchange) {
			SceneChengTime = Time;
			SceneChengCount = 0;
			SceneChangeFlag = SceneChange::Afterchange;
			if (SceneChangeClass !=nullptr) {
				if (Time != 0) {
					SceneChangeClass->AfterStart();
				}

			}
		}


	};

private:
	CSceneTransitionRenderer* SceneChangeClass;//シーンチェンジの仕方を決めるクラス	
	SceneChange SceneChangeFlag;	//シーン変更中
	int SceneChengTime;//シーンチェンジにかかる時間（完全に黒くなるまでの時間）
	int SceneChengCount;//シーンチェンジにかかっている時間

};

