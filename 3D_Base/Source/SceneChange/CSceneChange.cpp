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
			if (SceneChangeClass->BeforeUpdate(SceneChengCount, SceneChengTime)) {
				SceneChangeFlag = SceneChange::Beforechangeend;
			}
			SceneChengCount++;
		}
		if (SceneChangeFlag == SceneChange::Underchange) {
			if (SceneChangeClass->UnderUpdate(SceneChengCount, SceneChengTime)) {
				SceneChangeFlag = SceneChange::Underchangeend;
			}
			SceneChengCount++;
		}
		if (SceneChangeFlag == SceneChange::Afterchange) {

			if (SceneChangeClass->AfterUpdate(SceneChengCount, SceneChengTime)) {
				SceneChangeFlag = SceneChange::none;
				SAFE_DELETE(SceneChangeClass);
			}
			SceneChengCount++;
		}
	}
}


void CSceneChange::Draw() {
	if (SceneChangeClass != nullptr&& SceneChengTime!=0) {
		if (SceneChangeFlag == SceneChange::Beforechange|| SceneChangeFlag == SceneChange::Beforechangeend) {
			SceneChangeClass->BeforeDraw(static_cast<float> (SceneChengCount)/ static_cast<float> (SceneChengTime));
		}
		if (SceneChangeFlag == SceneChange::Underchange|| SceneChangeFlag == SceneChange::Underchangeend) {
			SceneChangeClass->UnderDraw(static_cast<float> (SceneChengCount) / static_cast<float> (SceneChengTime));
		}
		if (SceneChangeFlag == SceneChange::Afterchange) {
			SceneChangeClass->AfterDraw(static_cast<float> (SceneChengCount) / static_cast<float> (SceneChengTime));
		}
	}



}
