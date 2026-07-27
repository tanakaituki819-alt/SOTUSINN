#include "CPlayerParalysisUI.h"

CPlayerParalysisUI::CPlayerParalysisUI()
{
	m_pSprite = CSpriteManager::GetSprite3D(CSpriteManager::enImagList::Img_ButtonPush);
}

CPlayerParalysisUI::~CPlayerParalysisUI()
{
}

void CPlayerParalysisUI::Update()
{
	m_pSprite->SetPatternNo(0.f, 0.f);
	////タイマーが30fを超えたら　．
	//if (m_UIChangeTimer > 15.f) {
	//	m_UIChange = true;	
	//}
	//else if (m_UIChangeTimer < 15.f) {
	//	m_UIChange = false;
	//}

	////UIがプッシュ状態じゃない時.
	//if (!m_UIChange) {
	//	m_pSprite->SetPatternNo(0.f, 0.f);
	//	m_UIChangeTimer += 0.5f;
	//}
	////UIがプッシュ状態なら.
	//else {
	//	m_pSprite->SetPatternNo(0.f, 1.f);
	//	m_UIChangeTimer -= 0.5f;
	//}
}

void CPlayerParalysisUI::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	m_pSprite->SetBillboard(true);
	m_pSprite->SetScale(D3DXVECTOR3(1.f, 1.f, 2.f));
	m_pSprite->Render(View, Proj);
	m_pSprite->SetBillboard(false);
}
//プレイヤーのポジションを受け取る.
void CPlayerParalysisUI::Paralysis(D3DXVECTOR3 Position)
{
	m_pSprite->SetPosition(D3DXVECTOR3(Position.x + 0.5f,Position.y + 1.0f,Position.z + 0.5f));
}

