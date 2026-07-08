#pragma once

//-----------------------------.
//	連打対決クラス.
//-----------------------------.
class CBarrgeEncounter
{
public:
	CBarrgeEncounter();
	~CBarrgeEncounter();

	void Init();

	void Update();

private:
	int m_Barrage_MAX;	//連打MAX値

	int m_Barrage1_Count;	//連打カウント１
	int m_Barrage2_Count;	//連打カウント２
};