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
	, m_buoyancy(0)
{
	//短縮用
	using  CS = CSpriteManager;
	using  OL = CSpriteManager::enMeshObjList;

	IngredientsSetting[static_cast<int>(Ingredients::none)]		=		{ CS::GetObjMesh(OL::S_NIKU)	,0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::NIKU)]		=		{ CS::GetObjMesh(OL::S_NIKU)	,0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::NINZIN)]	=		{ CS::GetObjMesh(OL::S_NINZIN)	,0.4, {1.f,1.f,1.f}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::DAIKON)]	=		{ CS::GetObjMesh(OL::S_DAIKON)	,0.4, {1.2,1.2,1.2}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::ENOKI)]	=		{ CS::GetObjMesh(OL::S_ENOKI)	,0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::HAKUSAI1)] =		{ CS::GetObjMesh(OL::S_HAKUSAI1),0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::HAKUSEI2)] =		{ CS::GetObjMesh(OL::S_HAKUSAI2),0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::KANI)]		=		{ CS::GetObjMesh(OL::S_KANI)	,0.5, {4,4,4},		 {-1,D3DXToRadian(70),0},10};
	IngredientsSetting[static_cast<int>(Ingredients::KUMANOTE)] =		{ CS::GetObjMesh(OL::S_KUMANOTE),0.4, {2,2,2},		 {D3DXToRadian(-50),D3DXToRadian(280),0},10};
	IngredientsSetting[static_cast<int>(Ingredients::NEGI)]		=		{ CS::GetObjMesh(OL::S_NEGI)	,0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::ROBUSTER)] =		{ CS::GetObjMesh(OL::S_ROBUSTER),0.4, {5,5,5},		 {D3DXToRadian(-50),D3DXToRadian(200),0},10};
	IngredientsSetting[static_cast<int>(Ingredients::SAKANA)]	=		{ CS::GetObjMesh(OL::S_SAKANA)	,0.5, {6,6,6},		 {D3DXToRadian(-90),0,0},10};
	IngredientsSetting[static_cast<int>(Ingredients::SITAKE)]	=		{ CS::GetObjMesh(OL::S_SITAKE)	,0.4, {1.2,1.2,1.2}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::TAI)]		=		{ CS::GetObjMesh(OL::S_TAI)		,0.4, {2.5,2.5,2.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::TARA)]		=		{ CS::GetObjMesh(OL::S_TARA)	,0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::TOUHU)]	=		{ CS::GetObjMesh(OL::S_TOUHU)	,0.4, {1.f,1.f,1.f}, {0,0,0},10 };
	IngredientsSetting[static_cast<int>(Ingredients::UINNER)]	=		{ CS::GetObjMesh(OL::S_UINNER)	,0.4, {1.5,1.5,1.5}, {0,0,0},10 };
	Status = enCharStatus::Standby;
}

CIngredients::~CIngredients()
{
}
void CIngredients::SetIngredients(int i)
{
	IngredientsNo = static_cast<Ingredients>(i);
	m_pMesh = IngredientsSetting[i].Mesh;
	m_Scale = IngredientsSetting[i].Size;
	m_Rotation = IngredientsSetting[i].Rotation;
	if (IngredientsSetting[i].HitSize==0) {
		m_pBSphere->CreatSphereForMesh(*m_pMesh);
	}
	else {
		m_pBSphere->SetRadius(IngredientsSetting[i].HitSize);
	}
	Score = IngredientsSetting[i].Score;
	Fallingforce = 0;
	isBoiled = false;
	m_Boiledc = 0;
	m_BoiledcMAX = (15*(0.8+static_cast<float>(rand()%5)/10)) * 60;
	Status = enCharStatus::Live;
	m_IsCollecting = false;
}

void CIngredients::IsCollecting()
{
	m_IsCollecting = true;	//回収中へ.
}


void CIngredients::Update()
{


	Fallingforce += gravity;
	D3DXVECTOR3  calPos = { m_Position.x,0,m_OldPosition.z };
	if (Nabe!=nullptr) {
		//鍋の外に出ないようにする
		if (!D2CollizionXZ(calPos, m_pBSphere->GetRadius(), Nabe->GetPosition(), Nabe->GetSize())) {

			m_Position.x = m_OldPosition.x;

		};
		calPos = { m_OldPosition.x,0,m_Position.z };
		
		if (!D2CollizionXZ(calPos, m_pBSphere->GetRadius(), Nabe->GetPosition(), Nabe->GetSize() )) {
		m_Position.z = m_OldPosition.z;
		
		};
		//水に沈んだ
		if (m_Position.y <= Nabe->GetPosition().y + Nabe->GetNabeH()) {
		
			Fallingforce *= 0.5;//液体による減速
			// 沈んでいる深さに応じた浮力の計算（深ければ深いほど浮力が強くなる）
			float depth = Nabe->GetNabeH() - m_Position.y; // 沈んでいる深さ
			float buoyancyFactor = 0.15f;               // 浮力の強さ調整用パラメータ

			// 毎フレーム加算ではなく、深さに応じた固定の浮力を設定
			m_buoyancy =  (depth * buoyancyFactor);

			// 落下力から浮力を引く（＝上向きの力を与える）
			Fallingforce -= m_buoyancy;
		}
		else {
			m_buoyancy = 0;
		}
	}
	if (!m_IsCollecting) {
		m_Position.y -= Fallingforce;
	}
	
	m_OldPosition = m_Position;
	//にえていないなら
	if (isBoiled==false) {

		m_Boiledc ++;
		if (m_Boiledc> m_BoiledcMAX) {
			isBoiled = true;
		}

	}

	if (Status == enCharStatus::Dead) {
		Status = enCharStatus::Standby;
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



	//m_pMesh->SetLightCOLOR256({ 247,121,60 });
	if (m_Boiledc > m_BoiledcMAX/2) {
		D3DXVECTOR3 Coler= { 157,101,80 };
		m_pMesh->SetLightCOLOR256(Coler);
		if (IngredientsNo == Ingredients::KANI || IngredientsNo == Ingredients::ROBUSTER) {
			Coler = { 50,50,50 };
			m_pMesh->SetLightCOLOR256(Coler  );
		}

		m_pMesh->SetisCOLOR(true);
		if (isBoiled) {
			D3DXVECTOR3 Coler = { 107,41,0 };
			m_pMesh->SetLightCOLOR256(Coler);
			if (IngredientsNo == Ingredients::KANI || IngredientsNo == Ingredients::ROBUSTER) {
				Coler = { 200,100,75 };
				m_pMesh->SetLightCOLOR256(Coler);
			}
		}


	}
	else {
		m_pMesh->SetisCOLOR(false);
		if (IngredientsNo == Ingredients::KANI || IngredientsNo == Ingredients::ROBUSTER) {
			m_pMesh->SetLightCOLOR256({ 0,0,0 });
			m_pMesh->SetisCOLOR(true);
		}
	}
	//レンダリング.
	m_pMesh->Render(View, Proj, Light, Camera.Position);
}

