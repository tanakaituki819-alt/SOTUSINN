#include "CBarrgeEncounter.h"
#include "CXInput.h"

CBarrgeEncounter::CBarrgeEncounter()
	: m_Barrage_MAX		(10)
	, m_Barrage1_Count	()
	, m_Barrage2_Count	()

{
}

CBarrgeEncounter::~CBarrgeEncounter()
{
}

void CBarrgeEncounter::Init()
{
	m_Barrage1_Count = 0;
	m_Barrage2_Count = 0;
}

void CBarrgeEncounter::Update()
{
	//もしも1のAボタンが押されたら　if()
	m_Barrage1_Count++;
	//もしも2のAボタンが押されたら　if()
	m_Barrage2_Count++;
	
	if (m_Barrage1_Count <= m_Barrage_MAX)
	{

	}

	if (m_Barrage2_Count <= m_Barrage_MAX)
	{

	}

}

