#include "CSoundManager.h"
#include "MyMacro.h"

CSoundManager::CSoundManager()
{
	//インスタンス生成
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
			m_pSound_SE[i][List] = new CSound();
		}
	}

	for (int List = 0; List < enSingleSoundList::SingleSound_MAX; List++) {
		m_pSound_BGM[List] = new CSound();
	}

}

CSoundManager::~CSoundManager()
{
	Release();

	//インスタンス破棄
	//SE
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
			SAFE_DELETE(m_pSound_SE[i][List]);
		}
	}
	//BGM
	for (int List = 0; List < enSingleSoundList::SingleSound_MAX; List++) {
		SAFE_DELETE(m_pSound_BGM[List]);
	}
}

//サウンドデータ読込関数
bool CSoundManager::Load(HWND hWnd)
{
#if 0
	//MIDIまたはmp3ファイルのオープン.
	if (m_pSound[enList::BGM_Title]->Open(
		"Data\\Sound\\BGM\\Title.mp3", "BGM_Title",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::SE_Shot]->Open(
		"Data\\Sound\\SE\\Shot.mp3", "SE_Shot",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::BGM_Result]->Open(
		"Data\\Sound\\BGM\\Result.mp3", "BGM_Result",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::BGM_Bonus]->Open(
		"Data\\Sound\\BGM\\BonusGameHouse.mp3", "BGM_Bonus",
		hWnd) == false) 
	{
		return false;
	}

	if (m_pSound[enList::SE_Jump]->Open(
		"Data\\Sound\\SE\\Jump.wav", "SE_Jump",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::SE_Clear]->Open(
		"Data\\Sound\\SE\\Clear.wav", "SE_Clear",
		hWnd) == false)
	{
		return false;
	}
#else
	struct MultiSoundList
	{
		int listNo;		//enList列挙型を設定
		TCHAR path[256];	//ファイルの名前(パス付)
		TCHAR alias[32];	//エイリアス名
	};
	//重複してなる物
	MultiSoundList SE_List[] =
	{
		{enMultiSoundList::Clear,	_T("Data\\Sound\\SE\\Clear.wav"		), _T("Clear")},
		{enMultiSoundList::Jump,		_T("Data\\Sound\\SE\\Jump.wav"), _T("Jump")},
	

	
	};

	//配列の最大要素数を算出(配列全体のサイズ/配列１つ分のサイズ)
	//SE用(配列)に手を加えている
	int SE_List_max = sizeof(SE_List) / sizeof(SE_List[0]);
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < SE_List_max; List++) {
			if (m_pSound_SE[i][SE_List[List].listNo]->SEOpen(
				SE_List[List].path,
				SE_List[List].alias,
				hWnd,i) == false)
			{
				return false;
			}
		}
	}

	struct SingleSoundList
	{
		int listNo;		//enList列挙型を設定
		TCHAR path[256];	//ファイルの名前(パス付)
		TCHAR alias[32];	//エイリアス名
	};
	//単体で鳴らすもの
	SingleSoundList BGM_List[] =
	{
			{enSingleSoundList::BonusGameHouse,	_T("Data\\Sound\\BGM\\BonusGameHouse.mp3"), _T("BonusGameHouse")},



	};

	//配列の最大要素数を算出(配列全体のサイズ/配列１つ分のサイズ)
	int BGM_List_max = sizeof(BGM_List) / sizeof(BGM_List[0]);
	for (int i = 0; i < BGM_List_max; i++) {
		if (m_pSound_BGM[BGM_List[i].listNo]->Open(
			BGM_List[i].path,
			BGM_List[i].alias, 
			hWnd) == false)
		{
			return false;
		}
	}
	return true;
#endif
}

//サウンドデータ開放処理
void CSoundManager::Release()
{
	//開いた時と逆順で閉じる
	//SE
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
			if (m_pSound_SE[i][List] != nullptr) {
				m_pSound_SE[i][List]->Close();
			}
		}
	}
	//BGM
	for (int List = 0; List < enSingleSoundList::SingleSound_MAX; List++) {
		if (m_pSound_BGM[List] != nullptr) {
			m_pSound_BGM[List]->Close();
		}
	}
}
