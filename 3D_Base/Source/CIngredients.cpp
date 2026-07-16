#include "CIngredients.h"

namespace {
	using CS = CSpriteManager;
	using OL = CSpriteManager::enMeshObjList;
}

CIngredients::CIngredients()
	:IngredientsNo(Ingredients::none)
	, Nabe(nullptr)
{
	//短縮用
	using  CS = CSpriteManager;
	using  OL = CSpriteManager::enMeshObjList;

	IngredientsSetting[static_cast<int>(Ingredients::none)] = { CS::GetObjMesh(OL::ROBO),0.3,{1,1,1},10 };
	IngredientsSetting[static_cast<int>(Ingredients::Onion)] = { CS::GetObjMesh(OL::ROBO),0,{1,1,1},10 };


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
	static int t = 0;
	t++;
	m_pMesh->SetLightCOLOR({ static_cast<FLOAT>(sin(t) * 0.5 + 0.5),static_cast<FLOAT>(sin(t + D3DXToRadian(120)) * 0.5 + 0.5),static_cast<FLOAT>(sin(t + D3DXToRadian(240)) * 0.5 + 0.5) });
	if (GetAsyncKeyState('H') & 0x8000) {
		m_pMesh->SetisCOLOR(true);
	}
	else {
		m_pMesh->SetisCOLOR(false);
	}
	//レンダリング.
	m_pMesh->Render(View, Proj, Light, Camera.Position);
}

