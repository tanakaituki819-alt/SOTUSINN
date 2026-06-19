#include "CEnemy.h"

CEnemy::CEnemy()
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update()
{
	//手前に移動してくる.
	m_Position.z -= 0.1f;
	if (m_Position.z <= 0.f) {
		m_Position.z = 20.f;
	}
}
