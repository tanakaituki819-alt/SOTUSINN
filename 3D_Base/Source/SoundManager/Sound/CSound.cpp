#define _CRT_SECURE_NO_WARNINGS
#include "CSound.h"
#include <stdio.h>	//sprintf_s関数を使用するのに必要

//コンストラクタ
CSound::CSound()
		//初期化リスト
	:	m_hWnd			(nullptr)			//初っ端は:で
	,	m_sAlias			()							//その次から，で
	//								↑初期化の値に迷ってるなら()でもOK		0として扱う
{
	//m_hWnd = nullptr;			※中で初期化する方法もあるが
	//m_sAlias;									あまり使わない
}

//デストラクタ
CSound::~CSound()
{
}




//==================================================
// 音ファイルを開く
//==================================================
bool CSound::Open(LPCTSTR sFileName, LPCTSTR sAlias, HWND hWnd)
{
	//初期設定
	SetInitParam(sAlias, hWnd);

	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	wsprintf(cmd, _T("open %s alias %s"), sFileName, m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}

	return false;

}

bool CSound::SEOpen(LPCTSTR sFileName, LPCTSTR sAlias, HWND hWnd, int No)
{
	TCHAR Alias[256];
	wsprintf(Alias, _T("%s%d"), sAlias, No);
	//初期設定
	SetInitParam(Alias, hWnd);

	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	wsprintf(cmd, _T("open %s alias %s"), sFileName, m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}

	return false;
}

//==================================================
// 音ファイルを閉じる
//==================================================
bool CSound::Close()
{
	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	wsprintf(cmd, _T("close %s"), m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}

	return false;
}

//==================================================
//音を再生する
//==================================================
bool CSound::Play( bool bNotify)
{
	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	if (bNotify == true) {
		wsprintf(cmd, _T("play %s notify"), m_sAlias);	//notify 音の状態取得で必要
	}
	else {
		wsprintf(cmd, _T("play %s"), m_sAlias);
	}

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}

	return false;
}

//==================================================
//音を再生する(SEで使う)
//==================================================
bool CSound::PlaySE()
{
	SeekToStart();
	if (Play() == true) {
		return true;
	}

	return false;
}

//==================================================
//音を再生する(ループ)
//==================================================
bool CSound::PlayLoop()
{
	if (IsStopped() == true) {
		SeekToStart();
		if (Play(true) == true) {
			return true;
		}

	}

	return false;
}

//==================================================
//音を停止する
//==================================================
bool CSound::Stop()
{
	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	wsprintf(cmd, _T("stop %s"), m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}

	return false;
}

//==================================================
//音の状態を取得する
//			sStatus の配列数は 256以下にすること
//	※ただし、状態を取得する場合は、再生時に「notify」 を設定し、
//	　ウィンドウハンドルにメッセージを送っておく必要がある
//==================================================
bool CSound::GetStatus(LPTSTR sStatus)
{
	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	wsprintf(cmd, _T("status %s mode"), m_sAlias);

	if (mciSendString(cmd, sStatus, 256, m_hWnd) == 0) {
		return true;
	}

	return false;
}

//==================================================
//音の停止を確認する
//==================================================
bool CSound::IsStopped()
{
	TCHAR sStatus[STR_BUFFER_MAX] = _T("");

	//状態の取得
	if (GetStatus(sStatus) == true) {
		//文字列比較
		if (lstrcmp(sStatus, _T("stopped")) == 0) {
			return true;
		}
		return false;
	}

	return false;
}

//==================================================
//音の再生位置を最初にする
//==================================================
bool CSound::SeekToStart()
{
	//コマンド
	TCHAR cmd[STR_BUFFER_MAX] = _T("");

	wsprintf(cmd, _T("seek %s to start"), m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}

	return false;
}
//==================================================
//初期値を設定する関数
//==================================================
void CSound::SetInitParam(LPCTSTR sAlias, HWND hWnd)
{
	//ウィンドウハンドルを登録
	m_hWnd = hWnd;

	//エイリアスを登録(文字列をコピー)
	lstrcpy(m_sAlias,  sAlias);
}
