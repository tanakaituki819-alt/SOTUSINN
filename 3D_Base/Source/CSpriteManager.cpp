#include "CSpriteManager.h"

CSpriteManager::CSpriteManager()
{
	for (int i = 0;i < static_cast <int>(enImagList::MAX);i++) {
		Sprite2D[i] = nullptr;
		Sprite3D[i] = nullptr;
		SpriteBlock[i] = nullptr;
		SpriteTriangle[i] = nullptr;
	}
	for (int i = 0;i < static_cast <int>(enMeshList::MAX);i++) {
		StaticMesh[i] = nullptr;
	}

	for (int i = 0;i < static_cast <int>(enMeshObjList::MAX);i++) {
		StaticObjMesh[i] = nullptr;
	}
}

CSpriteManager::~CSpriteManager()
{
	allDelete();


}







CSprite2D* CSpriteManager::GetSprite2D(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	
	SPRITE_STATE ST = { {1,1,0} ,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Base,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Stride};
	if(CSpriteManager::GetInstance()->Sprite2D[NO]==nullptr) {
		GetInstance()->Sprite2D[NO] = new CSprite2D();
		if (CSpriteManager::GetInstance()->Img_List[NO].centralstandards == true) {
			GetInstance()->Sprite2D[NO]->Init(*(CSpriteManager::GetInstance()->Dx11),
				CSpriteManager::GetInstance()->Img_List[NO].path, ST);

		}
		else {
			GetInstance()->Sprite2D[NO]->Init(*(CSpriteManager::GetInstance()->Dx11),
				CSpriteManager::GetInstance()->Img_List[NO].path, ST);
		}
	}



	return GetInstance()->Sprite2D[NO];
}

CSprite3D* CSpriteManager::GetSprite3D(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}

	SPRITE_STATE ST = { {1,1,0} ,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Base,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Stride };
	if (CSpriteManager::GetInstance()->Sprite3D[NO] == nullptr) {
		GetInstance()->Sprite3D[NO] = new CSprite3D();
		GetInstance()->Sprite3D[NO]->Init(*(CSpriteManager::GetInstance()->Dx11),
			CSpriteManager::GetInstance()->Img_List[NO].path, ST);

	}
	return GetInstance()->Sprite3D[NO];
}

CSpriteBlock* CSpriteManager::GetSpriteBlock(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}

	SPRITE_STATE ST = { {1,1,0} ,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Base,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Stride };
	if (CSpriteManager::GetInstance()->SpriteBlock[NO] == nullptr) {
		GetInstance()->SpriteBlock[NO] = new CSpriteBlock();
		GetInstance()->SpriteBlock[NO]->Init(*(CSpriteManager::GetInstance()->Dx11),
			CSpriteManager::GetInstance()->Img_List[NO].path, ST);
	}
	return GetInstance()->SpriteBlock[NO];
}

CSpriteTriangle* CSpriteManager::GetSpriteTriangle(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}

	SPRITE_STATE ST = { {1,1,1} ,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Base,CSpriteManager::GetInstance()->Img_List[NO].SPRITE.Stride };
	if (CSpriteManager::GetInstance()->SpriteTriangle[NO] == nullptr) {
		GetInstance()->SpriteTriangle[NO] = new CSpriteTriangle();
		GetInstance()->SpriteTriangle[NO]->Init(*(CSpriteManager::GetInstance()->Dx11),
			CSpriteManager::GetInstance()->Img_List[NO].path, ST);

	}




	return GetInstance()->SpriteTriangle[NO];

}

CStaticMesh* CSpriteManager::GetMesh(enMeshList No)
{
	//番号とモデルのパスを入れた変数の番号があっているか合わせる
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enMeshList::MAX);i++) {
		if (CSpriteManager::GetInstance()->MeshList[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	if (CSpriteManager::GetInstance()->StaticMesh[NO] == nullptr) {
		GetInstance()->StaticMesh[NO] = new CStaticMesh();

		GetInstance()->StaticMesh[NO]->Init(*(CSpriteManager::GetInstance()->Dx9), *(CSpriteManager::GetInstance()->Dx11),
			CSpriteManager::GetInstance()->MeshList[NO].path);
	}
	return GetInstance()->StaticMesh[NO];
}

CStaticObjMesh* CSpriteManager::GetObjMesh(enMeshObjList No)
{
	//番号とモデルのパスを入れた変数の番号があっているか合わせる
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enMeshObjList::MAX);i++) {
		if (CSpriteManager::GetInstance()->MeshObjList[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	if (CSpriteManager::GetInstance()->StaticObjMesh[NO] == nullptr) {
		GetInstance()->StaticObjMesh[NO] = new CStaticObjMesh();
		GetInstance()->StaticObjMesh[NO]->Init(*(CSpriteManager::GetInstance()->Dx9), *(CSpriteManager::GetInstance()->Dx11),
			CSpriteManager::GetInstance()->MeshObjList[NO].path);
	}
	return GetInstance()->StaticObjMesh[NO];

}

void CSpriteManager::DeleteSprite2D(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	SAFE_DELETE(GetInstance()->Sprite2D[NO]);
}

void CSpriteManager::DeleteSprite3D(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	SAFE_DELETE(GetInstance()->Sprite3D[NO]);
}

void CSpriteManager::DeleteSpriteBlock(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	SAFE_DELETE(GetInstance()->SpriteBlock[NO]);
}

void CSpriteManager::DeleteSpriteTriangle(enImagList No)
{
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enImagList::MAX);i++) {
		if (CSpriteManager::GetInstance()->Img_List[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	SAFE_DELETE(GetInstance()->SpriteTriangle[NO]);
}

void CSpriteManager::DeleteMesh(enMeshList No)
{
	//番号とモデルのパスを入れた変数の番号があっているか合わせる
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enMeshList::MAX);i++) {
		if (CSpriteManager::GetInstance()->MeshList[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	SAFE_DELETE(GetInstance()->StaticMesh[NO]);
}

void CSpriteManager::DeleteObjMesh(enMeshObjList No)
{
	//番号とモデルのパスを入れた変数の番号があっているか合わせる
	int NO = 0;
	for (int i = 0;i < static_cast<int>(enMeshObjList::MAX);i++) {
		if (CSpriteManager::GetInstance()->MeshObjList[i].listNo == static_cast<int>(No)) {
			NO = i;
			break;
		}
	}
	SAFE_DELETE(GetInstance()->StaticObjMesh[NO]);
}

void CSpriteManager::allDelete()
{
	for (int i = 0;i < Img_List.size();i++) {
		SAFE_DELETE(GetInstance()->Sprite2D[i]);
		SAFE_DELETE(GetInstance()->Sprite3D[i]);
		SAFE_DELETE(GetInstance()->SpriteBlock[i]);
		SAFE_DELETE(GetInstance()->SpriteTriangle[i]);
	}
	for (int i = 0;i < MeshList.size();i++) {
		SAFE_DELETE(StaticMesh[i]);
	}
	for (int i = 0;i < MeshObjList.size();i++) {
		SAFE_DELETE(StaticObjMesh[i]);
	}
}



bool CSpriteManager::Load(CDirectX11& pDx11, CDirectX9& pDx9)
{
	Dx11 = &pDx11;
	Dx9 = &pDx9;
	//画像サイズとpinの場所登録
	Img_List.push_back({ static_cast<int>(enImagList::Img_TITLE), _T("Data\\Texture\\T_IMG.png"), { {1, 1, 1}, {1280, 720}, {1280, 720} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_GameClear), _T("Data\\Texture\\G_C_IMG.png"), { {1, 1, 1}, {1280, 720}, {1280, 720} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_GameOver), _T("Data\\Texture\\G_O_IMG.png"), { {1, 1, 1}, {1280, 720}, {1280, 720} } });

	Img_List.push_back({ static_cast<int>(enImagList::Img_Fad), _T("Data\\Texture\\IMG_Fad.png"), { {1, 1, 1}, {1280, 720}, {1280, 720} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_BackFire), _T("Data\\Texture\\explosion.png"), { {1, 1, 1}, {1280, 720}, {1280, 720} } });
	Img_List.push_back({ static_cast<int>(enImagList::IMG_Shutter), _T("Data\\Texture\\IMG_Shutter.png"), { {1, 1, 1}, {1280, 720}, {1280, 720} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_BattleArea), _T("Data\\Texture\\BatoruEria.png"), { {1, 1, 1}, {128, 128}, {128, 128} } });
	Img_List.push_back({ static_cast<int>(enImagList::IMG_Base), _T("Data\\Texture\\base.jpg"), { {1, 1, 1}, {128, 128}, {128, 128} } });
	

	Img_List.push_back({ static_cast<int>(enImagList::Img_EnergyGaugeBase), _T("Data\\Texture\\EnergyGaugeBase.png"), { {1, 1, 1}, {128, 128}, {128, 128} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_EnergyGauge), _T("Data\\Texture\\EnergyGauge.png"), { {1, 1, 1}, {128, 128}, {128, 128} }});
	Img_List.push_back({ static_cast<int>(enImagList::Img_HPGaugeBase), _T("Data\\Texture\\HPGaugeBase.png"), { {1, 1, 1}, {128, 128}, {128, 128} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_HPGauge), _T("Data\\Texture\\HPGauge.png"), { {1, 1, 1}, {128, 128}, {128, 128} } });
	Img_List.push_back({ static_cast<int>(enImagList::Img_HPGauge1), _T("Data\\Texture\\HPGauge1.png"), { {1, 1, 1}, {128, 128}, {128, 128} } });


	Img_List.push_back({ static_cast<int>(enImagList::Img_STRBack), _T("Data\\Texture\\STRBack.png"), { {1, 1, 1}, {128, 128}, {128, 128} },true });
	Img_List.push_back({ static_cast<int>(enImagList::DefenseBaseHP), _T("Data\\Texture\\DefenseBaseHP.png"), { {1, 1, 1}, {128, 128}, {128, 128} },true });
	Img_List.push_back({ static_cast<int>(enImagList::DefenseBaseHPback), _T("Data\\Texture\\DefenseBaseHPback.png"), { {1, 1, 1}, {128, 128}, {128, 128} },true });
	
	Img_List.push_back({ static_cast<int>(enImagList::Digit0_9), _T("Data\\Texture\\Digit0_9.png"), { {1, 1, 1}, {10, 1}, {1, 1} } });


	//Mehshも
	MeshList.push_back({ static_cast<int>(enMeshList::Sphere),_T("Data\\Collision\\Sphere.x") });

	//オブジェファイルの読み込み
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::GOZIRA),_T("Data\\Mesh\\Obj\\GOZIRA\\GOZIRA.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::Apartment),_T("Data\\Mesh\\Obj\\Apartment\\Apartment.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::FighterJet),_T("Data\\Mesh\\Obj\\FighterJet\\FighterJet.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::Grassland),_T("Data\\Mesh\\Obj\\Grassland\\Grassland.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::Missile),_T("Data\\Mesh\\Obj\\Missile\\Missile.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::Monster),_T("Data\\Mesh\\Obj\\Monster\\Monster.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::HATO),_T("Data\\Mesh\\Obj\\HATO\\HATO.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::ROBO),_T("Data\\Mesh\\Obj\\ROBO\\RobotooFrito.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::ROBO＿Hi),_T("Data\\Mesh\\Obj\\ROBOHi\\ROBOHi.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::DefenseBase),_T("Data\\Mesh\\Obj\\DefenseBase\\DefenseBase.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::Beam),_T("Data\\Mesh\\Obj\\Beam\\Beam.obj") });
	MeshObjList.push_back({ static_cast<int>(enMeshObjList::ROBOShot),_T("Data\\Mesh\\Obj\\ROBOShot\\robot.obj") });

	return true;
}
