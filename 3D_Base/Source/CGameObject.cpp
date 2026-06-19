#include "CGameObject.h"

//コンストラクタ.
CGameObject::CGameObject()
	: m_Position	()
	, m_Rotation	()
	, m_Scale		( 1.0f, 1.0f, 1.0f )
{
}

//デストラクタ.
CGameObject::~CGameObject()
{
}