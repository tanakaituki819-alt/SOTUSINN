#pragma once
//シーン列挙型
enum class enScene
{
	None,			//なし
	Title,			//タイトル
	BeforeGame,		//ゲーム前
	PlayerSetUp,	//プレイヤーコントローラー接続画面.
	GameMain,		//ゲーム中
	WinnnerResult,	//勝利者リザルト.
};