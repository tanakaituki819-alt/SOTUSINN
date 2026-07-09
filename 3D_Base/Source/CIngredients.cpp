#include "CIngredients.h"

CIngredients::CIngredients()
	:IngredientsNo(Ingredients::none)
{
	m_pMesh = CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::GOZIRA);
}

CIngredients::~CIngredients()
{
}

void CIngredients::Update()
{
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
