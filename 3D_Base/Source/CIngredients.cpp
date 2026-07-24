#include "CIngredients.h"

namespace {
	using CS = CSpriteManager;
	using OL = CSpriteManager::enMeshObjList;
}

CIngredients::CIngredients()
	:IngredientsNo(Ingredients::none)
	, Nabe(nullptr)
	, isBoiled(false)
	, m_BoiledcMAX(0)
	, m_Boiledc(0)
{
	//短縮用
	using  CS = CSpriteManager;
	using  OL = CSpriteManager::enMeshObjList;

	IngredientsSetting[static_cast<int>(Ingredients::none)] = { CS::GetObjMesh(OL::S_NIKU),0.3,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::NIKU)] = { CS::GetObjMesh(OL::S_NIKU),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::NINZIN)] = { CS::GetObjMesh(OL::S_NINZIN),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::DAIKON)] = { CS::GetObjMesh(OL::S_DAIKON),0,{1,1,1},10 };
	
	IngredientsSetting[static_cast<int>(Ingredients::ENOKI)] = { CS::GetObjMesh(OL::S_ENOKI),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::HAKUSAI1)] = { CS::GetObjMesh(OL::S_HAKUSAI1),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::HAKUSEI2)] = { CS::GetObjMesh(OL::S_HAKUSAI2),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::KANI)] = { CS::GetObjMesh(OL::S_KANI),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::KUMANOTE)] = { CS::GetObjMesh(OL::S_KUMANOTE),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::NEGI)] = { CS::GetObjMesh(OL::S_NEGI),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::ROBUSTER)] = { CS::GetObjMesh(OL::S_ROBUSTER),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::SAKANA)] = { CS::GetObjMesh(OL::S_SAKANA),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::SITAKE)] = { CS::GetObjMesh(OL::S_SITAKE),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::TAI)] = { CS::GetObjMesh(OL::S_TAI),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::TARA)] = { CS::GetObjMesh(OL::S_TARA),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::TOUHU)] = { CS::GetObjMesh(OL::S_TOUHU),0,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::UINNER)] = { CS::GetObjMesh(OL::S_UINNER),0,{1,1,1},10 };

}

CIngredients::~CIngredients()
{
}
void CIngredients::SetIngredients(int i)
{

	m_pMesh = IngredientsSetting[i].Mesh;
	m_Scale = IngredientsSetting[i].Size;
	if (IngredientsSetting[i].HitSize==0) {
		m_pBSphere->CreatSphereForMesh(*m_pMesh);
	}
	else {
		m_pBSphere->SetRadius(IngredientsSetting[i].HitSize);
	}
	Score = IngredientsSetting[i].Score;

	isBoiled = false;
	m_Boiledc = 0;
	m_BoiledcMAX = (15*(0.8+static_cast<float>(rand()%5)/10)) * 60;
		
}


void CIngredients::Update()
{


	m_Position.y-=0.01;
	float Speed = 0.1;
	D3DXVECTOR3  calPos = { m_Position.x,0,m_OldPosition.z };
	if (Nabe!=nullptr) {
		if (!D2CollizionXZ(calPos, 0.5, Nabe->GetPosition(), Nabe->GetSize() )) {

			m_Position.x = m_OldPosition.x;

		};
		calPos = { m_OldPosition.x,0,m_Position.z };
		
		if (!D2CollizionXZ(calPos, 0.5, Nabe->GetPosition(), Nabe->GetSize() )) {
		m_Position.z = m_OldPosition.z;
		
		};
		if (m_Position.y <= Nabe->GetPosition().y + Nabe->GetNabeH()) {
			m_Position.y = Nabe->GetPosition().y + Nabe->GetNabeH();
		}
	}

	m_OldPosition = m_Position;

	if (isBoiled==false) {

		m_Boiledc ++;
		if (m_Boiledc> m_BoiledcMAX) {
			isBoiled = true;
		}

	}

}

void CIngredients::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if (m_pMesh == nullptr) {
		return;
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition(m_Position);
	m_pMesh->SetRotation(m_Rotation);
	m_pMesh->SetScale(m_Scale);

	m_pMesh->SetLightCOLOR256({ 127,51,0 });
	//m_pMesh->SetLightCOLOR256({ 247,121,60 });
	if (isBoiled) {
		m_pMesh->SetisCOLOR(true);
	}
	else {
		m_pMesh->SetisCOLOR(false);
	}
	//レンダリング.
	m_pMesh->Render(View, Proj, Light, Camera.Position);
}

