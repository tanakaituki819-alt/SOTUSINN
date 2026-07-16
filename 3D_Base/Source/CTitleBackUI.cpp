#include "CTitleBackUI.h"
#include "CSpriteManager.h"

CTitleBackUI::CTitleBackUI()
	: m_pWhiteBack	()
	, m_pText		()
	, m_pTextFrame	()
{
	m_pWhiteBack = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_WhiteBack);
	//テキストスプライトを設定.
	for (int i = 0; i < Text_Max; i++)
	{
		m_pText[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);
		m_pTextFrame[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TextFrame);
	}
}

CTitleBackUI::~CTitleBackUI()
{
}

void CTitleBackUI::Update()
{
	if (m_pController[0]->IsDown(CXInput::B, false))
	{
		m_TitleBackflag = true;
	}
	if (m_pController[0]->IsDown(CXInput::A, false))
	{
		m_TitleBackflag = false;
	}
}

void CTitleBackUI::Draw()
{
	if (m_TitleBackflag)
	{
		m_pWhiteBack->SetAlpha(0.5f);
		m_pWhiteBack->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_pWhiteBack->SetScale(D3DXVECTOR3(1280.f, 720.f, 0.f));

		m_pWhiteBack->Render();
	}
}
