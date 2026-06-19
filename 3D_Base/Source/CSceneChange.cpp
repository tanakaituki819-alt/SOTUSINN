#include "CSceneChange.h"





CSceneChange::CSceneChange()
	:SceneChangeFlag()
	, SceneChengTime()
	, SceneChengCount()
	, SceneChangeClass(nullptr)
{
}

CSceneChange::~CSceneChange()
{
	SAFE_DELETE(SceneChangeClass);
}

void CSceneChange::Update() {

	if (SceneChangeClass != nullptr) {


	if (SceneChangeFlag == SceneChange::Beforechange) {
		SceneChengCount++;
		if (SceneChangeClass->BeforeUpdate(SceneChengCount, SceneChengTime)) {
			SceneChangeFlag = SceneChange::Underchange;
		}

	}
	if (SceneChangeFlag == SceneChange::Afterchange) {
		SceneChengCount++;
		if (SceneChangeClass->AfterUpdate(SceneChengCount, SceneChengTime)) {
			SceneChangeFlag = SceneChange::none;
			SAFE_DELETE(SceneChangeClass);
		}

	}	
	}
}


void CSceneChange::Draw() {
	if (SceneChangeClass != nullptr&& SceneChengTime!=0) {
		if (SceneChangeFlag == SceneChange::Beforechange) {
			SceneChangeClass->BeforeDraw(static_cast<float> (SceneChengCount)/ static_cast<float> (SceneChengTime));
		}
		if (SceneChangeFlag == SceneChange::Underchange) {
			SceneChangeClass->UnderDraw();
		}
		if (SceneChangeFlag == SceneChange::Afterchange) {
			SceneChangeClass->AfterDraw(static_cast<float> (SceneChengCount) / static_cast<float> (SceneChengTime));
		}
	}



}
