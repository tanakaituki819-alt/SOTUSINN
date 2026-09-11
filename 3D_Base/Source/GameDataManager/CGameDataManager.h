#pragma once
#include<vector>

//プレイヤーごとのリザルトデータ構造体.
struct SPlayerData
{
    int m_nPlayerIndex;     // プレイヤー番号
    int m_nScore;           // 獲得した合計スコア
    int m_nMeatCount;       // 獲得した肉の数
    int m_nVeggieCount;     // 獲得した野菜の数
    int m_nSpecialCount;    // 獲得した高級食材などの数
    int m_nRank;            // 順位 (1～4)

    //初期化.
    SPlayerData()
        : m_nPlayerIndex(0)
        , m_nScore(0)
        , m_nMeatCount(0)
        , m_nVeggieCount(0)
        , m_nSpecialCount(0)
        , m_nRank(1)
    {
    }




    // =================================================================
// クラス: ゲームデータ管理クラス (シングルトン)
// -----------------------------------------------------------------
// 【概要】
//  シーンを跨いで「プレイ人数」や「各プレイヤーのスコア・獲得情報」を
//  保持・共有するためのマネージャークラスです。
//
// 【使い方ガイド】
//  1. プレイヤー選択画面 (CGameScenePlayerSetup) 側:
//     // 人数が確定したら設定する (例: 3人プレイ)
//     CGameDataManager::GetInstance()->SetPlayerCount(3);
//
//  2. ゲームメイン画面 (CGameSceneGameMain) 側:
//     // ゲーム開始時にデータをリセット
//     CGameDataManager::GetInstance()->ResetGameData();
//
//     // スコア獲得時（またはゲーム終了時にまとめて）スコアを加算
//     CGameDataManager::GetInstance()->AddScore(playerIndex, 100);
//
//  3. 優勝者・リザルト画面 (CGameSceneWinnerResult / CGameSceneResult) 側:
//     // 参加人数の取得
//     int count = CGameDataManager::GetInstance()->GetPlayerCount();
//
//     // 各プレイヤーのデータの取得
//     const SPlayerData& data = CGameDataManager::GetInstance()->GetPlayerData(playerIndex);
//     int score = data.m_nScore;
//
//     // 1位（勝者）のプレイヤーインデックスを取得
//     int winnerIdx = CGameDataManager::GetInstance()->GetWinnerPlayerIndex();
// ===============
};