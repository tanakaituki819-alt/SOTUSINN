#pragma once
//シーン列挙型
enum class enScene
{
	None,		//なし
	Title,		//タイトル
	BeforeGame,//ゲーム前
	GameMain,	//ゲーム中
	GameOver,		//ゲームオーバー
	GameClear,	//ゲームクリア
};