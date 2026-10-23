#include "CIngredientsmanager.h"






CIngredientsmanager::CIngredientsmanager()
	:Nabe(nullptr)
	, StartSettingis(true)
	, TimuC(0)
	, IngredientsSettingTimu(0)
{


	JsonFileIO::LoadEncrypted(filePath, LoadData);


	// 1. 乱数生成器の初期化
	std::random_device rd;
	//ハードウェア由来の非決定論的な乱数（シード値）を取得しています
	gen.seed(rd());
	//それを使ってgenを初期化


	//短縮用
	using  CS = CSpriteManager;
	using  OL = CSpriteManager::enMeshObjList;

	//SAVE();

	IngredientsStetoLoad(static_cast<int>(Ingredients::none), CS::GetObjMesh(OL::S_NIKU));
	IngredientsStetoLoad(static_cast<int>(Ingredients::NIKU), CS::GetObjMesh(OL::S_NIKU));
	IngredientsStetoLoad(static_cast<int>(Ingredients::NINZIN), CS::GetObjMesh(OL::S_NINZIN));
	IngredientsStetoLoad(static_cast<int>(Ingredients::DAIKON), CS::GetObjMesh(OL::S_DAIKON));
	IngredientsStetoLoad(static_cast<int>(Ingredients::ENOKI), CS::GetObjMesh(OL::S_ENOKI));
	IngredientsStetoLoad(static_cast<int>(Ingredients::HAKUSEI2), CS::GetObjMesh(OL::S_HAKUSAI2));
	IngredientsStetoLoad(static_cast<int>(Ingredients::HAKUSAI1), CS::GetObjMesh(OL::S_HAKUSAI1));

	IngredientsStetoLoad(static_cast<int>(Ingredients::KANI), CS::GetObjMesh(OL::S_KANI));
	IngredientsStetoLoad(static_cast<int>(Ingredients::KUMANOTE), CS::GetObjMesh(OL::S_KUMANOTE));
	IngredientsStetoLoad(static_cast<int>(Ingredients::NEGI), CS::GetObjMesh(OL::S_NEGI));
	IngredientsStetoLoad(static_cast<int>(Ingredients::ROBUSTER), CS::GetObjMesh(OL::S_ROBUSTER));
	IngredientsStetoLoad(static_cast<int>(Ingredients::SAKANA), CS::GetObjMesh(OL::S_SAKANA));
	IngredientsStetoLoad(static_cast<int>(Ingredients::SITAKE), CS::GetObjMesh(OL::S_SITAKE));

	IngredientsStetoLoad(static_cast<int>(Ingredients::TAI), CS::GetObjMesh(OL::S_TAI));
	IngredientsStetoLoad(static_cast<int>(Ingredients::TARA), CS::GetObjMesh(OL::S_TARA));
	IngredientsStetoLoad(static_cast<int>(Ingredients::TOUHU), CS::GetObjMesh(OL::S_TOUHU));
	IngredientsStetoLoad(static_cast<int>(Ingredients::UINNER), CS::GetObjMesh(OL::S_UINNER));






	std::vector<float> m_Normalbability;
	std::vector<float> m_Rarebility;
	
	for (int i = 0;i < static_cast<int> (Ingredients::MAX);i++) {
		if (i == static_cast<int>(Ingredients::KANI) ||
			i == static_cast<int>(Ingredients::KUMANOTE) ||
			i == static_cast<int>(Ingredients::SAKANA) ||
			i == static_cast<int>(Ingredients::ROBUSTER) ||
			i == static_cast<int>(Ingredients::TAI)
			) {
			m_Normalbability.push_back(0);
		}
		else {
			m_Normalbability.push_back(IngredientsSetting[i].probability);
		}

		if (i== static_cast<int>(Ingredients::KANI)||
			i == static_cast<int>(Ingredients::KUMANOTE) ||
			i == static_cast<int>(Ingredients::SAKANA) ||
			i == static_cast<int>(Ingredients::ROBUSTER) ||
			i == static_cast<int>(Ingredients::TAI)
			) {
			m_Rarebility.push_back(IngredientsSetting[i].probability);
		}
		else {
			m_Rarebility.push_back(0);
		}
	}
	std::vector<float> m_Gatya = {5.0f,95.f };//レア確率５

	//  離散分布（偏りのある分布）を作成(distに入る)
	Normaldist = std::discrete_distribution(m_Normalbability.begin(), m_Normalbability.end());
	Raredist = std::discrete_distribution(m_Rarebility.begin(), m_Rarebility.end());
	Gacha = std::discrete_distribution(m_Gatya.begin(), m_Gatya.end());


	
	LoadData = nullptr;            //（内部のデータを全部解放して、型も null になる）

}

CIngredientsmanager::~CIngredientsmanager()
{
	for (int i = 0; i < m_pIngredients.size(); i++) {
		SAFE_DELETE(m_pIngredients[i]);
	}
	m_pIngredients.clear();
	
}

void CIngredientsmanager::Update()
{
	TimuC++;

	if (gameTimu >0) {
		if (StartSettingis) {
			if (TimuC > 15) {
				TimuC = 0;
				Create();
			}

		}
		else {
			if (TimuC > IngredientsSettingTimu) {
				TimuC = 0;
				IngredientsSettingTimu = (0.5 * (0.8 + static_cast<float>(rand() % 5) / 10.f)) * 60;
				if (gameTimu<0.2) {
					IngredientsSettingTimu = (0.1 * (0.8 + static_cast<float>(rand() % 5) / 10.f)) * 60;
				}
				Create();
			}
		}
	}




	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i] != nullptr) {
			m_pIngredients[i]->Update();
		}
	}
	for (int i = 0; i < m_pIngredients.size(); i++) {
		if(m_pIngredients[i]->GetCharStatus()==enCharStatus::Live&& !m_pIngredients[i]->GetCollecting()){
			for (int c = 0; c < m_pIngredients.size(); c++) {
				if (m_pIngredients[c]->GetCharStatus() == enCharStatus::Live && !m_pIngredients[c]->GetCollecting()) {
					if (m_pIngredients[i] != nullptr) {
						if (m_pIngredients[c] != nullptr) {
							if (m_pIngredients[i] != m_pIngredients[c]) {
								if (m_pIngredients[i]->GetBSphere()->IsHit(*m_pIngredients[c]->GetBSphere()))
								{
									D3DXVECTOR3 Vec = m_pIngredients[i]->GetPosition() - m_pIngredients[c]->GetPosition();
									D3DXVec3Normalize(&Vec, &Vec);
									float Radius = m_pIngredients[i]->GetBSphere()->GetRadius() + m_pIngredients[c]->GetBSphere()->GetRadius();
									if (Radius > 0.05) {
										Radius = 0.05;
									}
									//真上、真下なら左にずれる；
									if (Vec.x == 0.f && Vec.z == 0.f && Vec.y != 0.f) {
										Vec.x += cos(rand()%360+1)*0.0001f;
										Vec.z += sin(rand()%360+11)*0.0001f;
									}
									m_pIngredients[i]->SetPosition({ m_pIngredients[i]->GetPosition() + ((Radius)*Vec) });
									m_pIngredients[c]->SetPosition({ m_pIngredients[c]->GetPosition() + ((Radius) * -Vec) });
									m_pIngredients[i]->UpdateBSpherePos();
									m_pIngredients[c]->UpdateBSpherePos();
								}
							}
						}
					}
				}
			}
		}
		
	}



}

void CIngredientsmanager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{

	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Live || m_pIngredients[i]->GetCharStatus() == enCharStatus::Dying) {
			m_pIngredients[i]->Draw(View, Proj, Light, Camera);
		}

	}
}

void CIngredientsmanager::Create()
{ 
	//std::cout << GetIngredientsliveing() << std::endl;
	if (GetIngredientsliveing() >= 40) {
		StartSettingis = false;
		return;
	}
	int index;
	if (StartSettingis) {
		index = Normaldist(gen);//最初の配置ならレア役が出ない
	}
	else {
		if (Gacha(gen)==0) {//レアなら０
			index = Raredist(gen); 
		}
		else {
			index = Normaldist(gen); // 確率に基づいたインデックスが返る
		}
		
	} 

	int R = 5;
	float NABE = 4*1.2;
	int x = rand() % R;
	int z = rand() % R;
	
	//再利用可能なら再利用
	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Standby) {
			m_pIngredients[i]->SetPosition({static_cast<FLOAT>(rand() % 100 + 1) / 100,5,static_cast<FLOAT>(rand() % 100 + 1) / 100});
			m_pIngredients[i]->SetIngredients(IngredientsSetting, index);
			return;
		}
	}
	CIngredients* now = new CIngredients;
	now->SetNabe(Nabe);
	now->SetPosition({ static_cast<float>(x) *(NABE/ static_cast<float>(R-1))- NABE/2.0f ,5,static_cast<float>(z) * (NABE / static_cast<float>(R-1))- NABE/2.0f });



	now->SetIngredients(IngredientsSetting, index);
	m_pIngredients.push_back(now);


}

void CIngredientsmanager::UpdateBSpherePos()
{

	for (int i = 0; i < m_pIngredients.size(); i++) {
		m_pIngredients[i]->UpdateBSpherePos();
	};
}

void CIngredientsmanager::AllIngredientsUpdate()
{
	//もし複数クラス作るならここで１つのベクターにしてまとめて渡せるようにする
	
	//AllBullet.clear();
	//AllBullet.insert(AllBullet.end(), m_pBMissile.begin(), m_pBMissile.end());
	//AllBullet.insert(AllBullet.end(), m_pBBeam.begin(), m_pBBeam.end());
}

std::vector<CIngredients*>& CIngredientsmanager::GetIngredients()
{
	return m_pIngredients;
}

float CIngredientsmanager::GetIngredientsliveingScore()
{
	float cost=0;
	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i]->GetCharStatus()==enCharStatus::Live) {
			cost += m_pIngredients[i]->GetCost();
		}
		
	};
	return cost;
}

int CIngredientsmanager::GetIngredientsliveing()
{
	float cost = 0;
	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Live) {
			cost++;
		}

	};
	return cost;
;
}

void CIngredientsmanager::SAVE()
{
	//短縮用
	using  CS = CSpriteManager;
	using  OL = CSpriteManager::enMeshObjList;
	IngredientsSetting[static_cast<int>(Ingredients::none)] = { CS::GetObjMesh(OL::S_NIKU)		,1,0.5, {1.5,1.5,1.5}, {0,0,0}								,10,1, };
	IngredientsSetting[static_cast<int>(Ingredients::NIKU)] = { CS::GetObjMesh(OL::S_NIKU)		,1,0.5, {1.5,1.5,1.5}, {0,0,0}								,10,1, };
	IngredientsSetting[static_cast<int>(Ingredients::NINZIN)] = { CS::GetObjMesh(OL::S_NINZIN)	,1,0.5, {1.f,1.f,1.f}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::DAIKON)] = { CS::GetObjMesh(OL::S_DAIKON)	,1,0.5, {1.2,1.2,1.2}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::ENOKI)] = { CS::GetObjMesh(OL::S_ENOKI)		,1,0.5, {1.5,1.5,1.5}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::HAKUSEI2)] = { CS::GetObjMesh(OL::S_HAKUSAI2)	,1,0.5, {1.5,1.5,1.5}, {D3DXToRadian(-60),D3DXToRadian(90),0},5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::HAKUSAI1)] = { CS::GetObjMesh(OL::S_HAKUSAI1)	,0,0.5, {1.5,1.5,1.5}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::KANI)] = { CS::GetObjMesh(OL::S_KANI)		,0.1,0.7, {4,4,4},	   {-1,D3DXToRadian(70),0}				,30,4, };
	IngredientsSetting[static_cast<int>(Ingredients::KUMANOTE)] = { CS::GetObjMesh(OL::S_KUMANOTE)	,0.1,0.7, {2,2,2},	   {D3DXToRadian(-50),D3DXToRadian(280),0},30,3, };
	IngredientsSetting[static_cast<int>(Ingredients::NEGI)] = { CS::GetObjMesh(OL::S_NEGI)		,1,0.5, {1.5,1.5,1.5}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::ROBUSTER)] = { CS::GetObjMesh(OL::S_ROBUSTER)	,0.1,0.7, {5,5,5},	   {D3DXToRadian(-50),D3DXToRadian(200),0},30,3, };
	IngredientsSetting[static_cast<int>(Ingredients::SAKANA)] = { CS::GetObjMesh(OL::S_SAKANA)	,0.05,0.8, {6,6,6},	   {D3DXToRadian(-90),0,0}				,50,3, };
	IngredientsSetting[static_cast<int>(Ingredients::SITAKE)] = { CS::GetObjMesh(OL::S_SITAKE)	,1,0.5, {1.2,1.2,1.2}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::TAI)] = { CS::GetObjMesh(OL::S_TAI)		,0.1,0.7, {2.0,2.0,2.0}, {0,D3DXToRadian(70),0}				,30,2, };
	IngredientsSetting[static_cast<int>(Ingredients::TARA)] = { CS::GetObjMesh(OL::S_TARA)		,1,0.5, {1.5,1.5,1.5}, {0,D3DXToRadian(70),0}				,10,1, };
	IngredientsSetting[static_cast<int>(Ingredients::TOUHU)] = { CS::GetObjMesh(OL::S_TOUHU)		,1,0.5, {1.f,0.5f,1.f}, {0,0,0}								,5,1, };
	IngredientsSetting[static_cast<int>(Ingredients::UINNER)] = { CS::GetObjMesh(OL::S_UINNER)	,1,0.5, {1.5,1.5,1.5}, {0,0,0}								,10,1, };

	nlohmann::json SAVEData;
	SAVEData["IngredientsSteto"] = nlohmann::json::array();

	for (int i = 0;i < static_cast<int>(Ingredients::MAX);i++) {
		SAVEData["IngredientsSteto"].push_back({
			{"No",          i},
			{"probability", IngredientsSetting[i].probability},
			{"HitSize",     IngredientsSetting[i].HitSize},
			{"大きさ",        nlohmann::json::array({ ( IngredientsSetting[i].Size.x),     (IngredientsSetting[i].Size.y),     (IngredientsSetting[i].Size.z )})},
			{"回転",    nlohmann::json::array({ D3DXToDegree(IngredientsSetting[i].Rotation.x), D3DXToDegree(IngredientsSetting[i].Rotation.y), D3DXToDegree(IngredientsSetting[i].Rotation.z) })},
			{"スコア",       IngredientsSetting[i].Score},
			{"コスト",        IngredientsSetting[i].Cost},
			{"完成時間", IngredientsSetting[i].BoiledcTimu}
			});
	}

	JsonFileIO::SaveEncrypted(filePath, SAVEData);
	return;
}

void CIngredientsmanager::IngredientsStetoLoad(int i, CStaticObjMesh* adoresu)
{

	D3DXVECTOR3 Size = {};
	D3DXVECTOR3 Rotation = {};

	      //IngredientsStetoがあって               それがオブジェクトなら 
	if (LoadData.contains("IngredientsSteto") && LoadData["IngredientsSteto"].is_array()) {
		if (LoadData["IngredientsSteto"][i].contains("大きさ") && LoadData["IngredientsSteto"][i]["大きさ"].is_array()) {
			//数値型なら
			if (LoadData["IngredientsSteto"][i]["大きさ"][0].is_number()) {
				Size.x = LoadData["IngredientsSteto"][i]["大きさ"][0];
			}
			if (LoadData["IngredientsSteto"][i]["大きさ"][1].is_number()) {
				Size.y = LoadData["IngredientsSteto"][i]["大きさ"][1];
			}
			if (LoadData["IngredientsSteto"][i]["大きさ"][2].is_number()) {
				Size.z = LoadData["IngredientsSteto"][i]["大きさ"][2];
			}
		}
		if (LoadData["IngredientsSteto"][i].contains("回転") && LoadData["IngredientsSteto"][i]["回転"].is_array()) {
			//数値型なら
			if (LoadData["IngredientsSteto"][i]["回転"][0].is_number()) {
				Rotation.x = D3DXToRadian(LoadData["IngredientsSteto"][i]["回転"][0]);
			}
			if (LoadData["IngredientsSteto"][i]["回転"][1].is_number()) {
				Rotation.y = D3DXToRadian(LoadData["IngredientsSteto"][i]["回転"][1]);
			}
			if (LoadData["IngredientsSteto"][i]["回転"][2].is_number()) {
				Rotation.z = D3DXToRadian(LoadData["IngredientsSteto"][i]["回転"][2]);
			}		
		}
		IngredientsSetting[i] = { adoresu		,LoadData["IngredientsSteto"][i].value("probability", 0.f),LoadData["IngredientsSteto"][i].value("HitSize", 0.f)
			,Size, Rotation	,LoadData["IngredientsSteto"][i].value("スコア", 0),LoadData["IngredientsSteto"][i].value("コスト", 1.f),LoadData["IngredientsSteto"][i].value("完成時間", 15.f) };
	}





}
