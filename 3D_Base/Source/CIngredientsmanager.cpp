#include "CIngredientsmanager.h"

CIngredientsmanager::CIngredientsmanager()
{
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
	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i] != nullptr) {
			m_pIngredients[i]->Update();
			//if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Death) {
			//	m_pIngredients[i]->Reset();
			//}
		}
	}
	for (int i = 0; i < m_pIngredients.size(); i++) {
		for (int c = 0; c < m_pIngredients.size(); c++) {
			if (m_pIngredients[i] != nullptr) {
				if (m_pIngredients[c] != nullptr) {
					if (m_pIngredients[i] != m_pIngredients[c]) {
						if (m_pIngredients[i]->GetBSphere()->IsHit(*m_pIngredients[c]->GetBSphere()))
						{
						D3DXVECTOR3 Vec=m_pIngredients[i]->GetPosition() - m_pIngredients[c]->GetPosition();
						D3DXVec3Normalize(&Vec,&Vec);
						float Radius = m_pIngredients[i]->GetBSphere()->GetRadius() / 2 + m_pIngredients[c]->GetBSphere()->GetRadius() / 2;
						m_pIngredients[i]->SetPosition({ m_pIngredients[i]->GetPosition() + ((Radius/2 ) * Vec) });
						m_pIngredients[c]->SetPosition({ m_pIngredients[c]->GetPosition() + ((Radius/2 ) * -Vec) });
						m_pIngredients[i]->UpdateBSpherePos();
						m_pIngredients[c]->UpdateBSpherePos();
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
	//	if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Live || AllBullet[i]->GetCharStatus() == enCharStatus::Dying) {
			m_pIngredients[i]->Draw(View, Proj, Light, Camera);
		//}

	}
}

void CIngredientsmanager::Create()
{



	//再利用可能なら再利用
	//for (int i = 0; i < m_pIngredients.size(); i++) {
	//	if (m_pBBeam[i]->GetCharStatus() == enCharStatus::Standby) {
	//		m_pBBeam[i]->StratShot(pos, Looc, Size);
	//		return;
	//	}
	//}
	CIngredients* now = new CIngredients;
	now->SetNabe(Nabe);
	now->SetPosition({static_cast<FLOAT>( rand() % 100+1)/100,5,static_cast<FLOAT>(rand() % 100+1) / 100 });
	static int c = -1;
	c++;
	if (c>=17) {
		c = 0;
	}

	now->SetIngredients(c);
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
