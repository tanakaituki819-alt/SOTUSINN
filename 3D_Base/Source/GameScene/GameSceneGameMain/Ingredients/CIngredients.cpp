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


	Status = enCharStatus::Standby;
}

CIngredients::~CIngredients()
{
}
void CIngredients::SetIngredients(IngredientsSetting* pIngredientsSetting,int i)
{
	IngredientsNo = static_cast<Ingredients>(i);
	m_pMesh = pIngredientsSetting[i].Mesh;
	m_Scale = pIngredientsSetting[i].Size;
	m_Rotation = pIngredientsSetting[i].Rotation;
	if (pIngredientsSetting[i].HitSize==0) {
		m_pBSphere->CreatSphereForMesh(*m_pMesh);
	}
	else {
		m_pBSphere->SetRadius(pIngredientsSetting[i].HitSize);
	}
	Score = pIngredientsSetting[i].Score;
	Fallingforce = 0;
	isBoiled = false;
	m_Boiledc = 0;
	m_BoiledcMAX = (pIngredientsSetting[i].BoiledcTimu*(0.8+static_cast<float>(rand()%5)/10.f)) * 60;//煮えるまでの時間が0.8から1.2倍のランダム
	Cost = pIngredientsSetting[i].Cost;
	Status = enCharStatus::Live;
	m_IsCollecting = false;
	IsInTheWater = false;
}

void CIngredients::IsCollecting()
{
	m_IsCollecting = true;	//回収中へ.
}


void CIngredients::Update()
{

	if (Status == enCharStatus::Live) {
		Fallingforce += gravity;
		D3DXVECTOR3  calPos = { m_Position.x,0,m_OldPosition.z };
		if (Nabe != nullptr) {
			//鍋の外に出ないようにする
			if (!D2CollizionXZ(calPos, m_pBSphere->GetRadius(), Nabe->GetPosition(), Nabe->GetSize())) {

				m_Position.x = m_OldPosition.x;

			};
			calPos = { m_OldPosition.x,0,m_Position.z };

			if (!D2CollizionXZ(calPos, m_pBSphere->GetRadius(), Nabe->GetPosition(), Nabe->GetSize())) {
				m_Position.z = m_OldPosition.z;

			};
			//水に沈んだ
			if (m_Position.y <= Nabe->GetPosition().y + Nabe->GetNabeH()) {
				if (IsInTheWater == false) {
					::EsHandle handle = -1;
					handle = Effect::Play(EFE::NAMI, { m_Position.x ,m_Position.y - 0.2f,m_Position.z });
					Effect::SetScale(handle, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
				}
				IsInTheWater = true;
				Fallingforce *= 0.5;//液体による減速
				// 沈んでいる深さに応じた浮力の計算（深ければ深いほど浮力が強くなる）
				float depth = Nabe->GetNabeH() - m_Position.y; // 沈んでいる深さ
				float buoyancyFactor = 0.15f;               // 浮力の強さ調整用パラメータ

				// 毎フレーム加算ではなく、深さに応じた固定の浮力を設定
				m_buoyancy = (depth * buoyancyFactor);

				// 落下力から浮力を引く（＝上向きの力を与える）
				Fallingforce -= m_buoyancy;
			}
			else {
				m_buoyancy = 0;
			}
		}
		if (IsInTheWater == true) {
			EffC += rand() % 2;
			if (EffC >= 60) {
				::EsHandle handle = -1;
				handle = Effect::Play(EFE::NAMI, { m_Position.x ,m_Position.y - 0.1f,m_Position.z });
				Effect::SetScale(handle, D3DXVECTOR3(0.3f, 0.1f, 0.3f));
				EffC = 0;
			}

		}
		if (!m_IsCollecting) {
			m_Position.y -= Fallingforce;
		}

		m_OldPosition = m_Position;
		//にえていないなら
		if (isBoiled == false) {

			m_Boiledc++;
			if (m_Boiledc > m_BoiledcMAX) {
				::EsHandle handle = -1;
				handle = Effect::Play(EFE::KANSEI, m_Position);
				Effect::SetScale(handle, D3DXVECTOR3(m_Scale.x * 0.15f, m_Scale.y * 0.15f, m_Scale.z * 0.15f));
				isBoiled = true;
			}

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
		D3DXVECTOR3 Coler= { 227,171,150 };
		m_pMesh->SetLightCOLOR256(Coler);
		if (IngredientsNo == Ingredients::KANI || IngredientsNo == Ingredients::ROBUSTER) {
			Coler = { 50,50,50 };
			m_pMesh->SetLightCOLOR256(Coler  );
		}

		m_pMesh->SetisCOLOR(true);
		if (isBoiled) {
			D3DXVECTOR3 Coler = { 110,50,2 };
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

