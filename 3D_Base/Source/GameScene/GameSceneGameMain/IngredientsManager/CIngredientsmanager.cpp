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
						float Radius = m_pIngredients[i]->GetBSphere()->GetRadius()  + m_pIngredients[c]->GetBSphere()->GetRadius() ;
						if (Radius>0.05) {
							Radius = 0.05;
						}
						//真上、真下なら左にずれる；
						if (Vec.x == 0.f && Vec.z == 0.f && Vec.y != 0.f) {
							Vec.x += 0.001;
						}
						m_pIngredients[i]->SetPosition({ m_pIngredients[i]->GetPosition() + ((Radius ) * Vec) });
						m_pIngredients[c]->SetPosition({ m_pIngredients[c]->GetPosition() + ((Radius ) * -Vec) });
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
		if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Live || m_pIngredients[i]->GetCharStatus() == enCharStatus::Dying) {
			m_pIngredients[i]->Draw(View, Proj, Light, Camera);
		}

	}
}

void CIngredientsmanager::Create()
{ 
	int R = 5;
	float NABE = 4*1.2;
	int x = rand() % R;
	int z = rand() % R;
	static int c = -1;
	//再利用可能なら再利用
	for (int i = 0; i < m_pIngredients.size(); i++) {
		if (m_pIngredients[i]->GetCharStatus() == enCharStatus::Standby) {
			m_pIngredients[i]->SetPosition({static_cast<FLOAT>(rand() % 100 + 1) / 100,5,static_cast<FLOAT>(rand() % 100 + 1) / 100});
			m_pIngredients[i]->SetIngredients(c);
			return;
		}
	}
	CIngredients* now = new CIngredients;
	now->SetNabe(Nabe);
	now->SetPosition({ static_cast<float>(x) *(NABE/ static_cast<float>(R-1))- NABE/2.0f ,5,static_cast<float>(z) * (NABE / static_cast<float>(R-1))- NABE/2.0f });

	c++;
	if (c>=17) {
		c = 0;
	}

	now->SetIngredients(7);
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
