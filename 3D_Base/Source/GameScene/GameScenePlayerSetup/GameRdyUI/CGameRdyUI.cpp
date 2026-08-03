#include "CGameRdyUI.h"
#include <iostream>

namespace {
	//手の初期ポジション.
	const D3DXVECTOR3 PLAYER1_RIGHTHAND_POS  = { 80.f	, 80.f, 0.f };		//プレイヤー1の右手初期ポジション.
	const D3DXVECTOR3 PLAYER2_RIGHTHAND_POS  = { 404.4f	, 80.f, 0.f };		//プレイヤー2の右手初期ポジション.
	const D3DXVECTOR3 PLAYER3_RIGHTHAND_POS  = { 724.4f	, 80.f, 0.f };		//プレイヤー3の右手初期ポジション.
	const D3DXVECTOR3 PLAYER4_RIGHTHAND_POS  = { 1044.4f	, 80.f, 0.f };	//プレイヤー4の右手初期ポジション.
	//左手の初期ポジション.
	const D3DXVECTOR3 PLAYER1_LEFTHAND_POS	 = { 0.f, 80.f, 0.f };			//プレイヤー1の右手初期ポジション.
	const D3DXVECTOR3 PLAYER2_LEFTHAND_POS	 = { 325.4f	, 80.f, 0.f };		//プレイヤー2の右手初期ポジション.
	const D3DXVECTOR3 PLAYER3_LEFTHAND_POS	 = { 645.4f	, 80.f, 0.f };		//プレイヤー3の右手初期ポジション.
	const D3DXVECTOR3 PLAYER4_LEFTHAND_POS	 = { 965.4f	, 80.f, 0.f };		//プレイヤー4の右手初期ポジション.
	//白背景のポジション.
	const D3DXVECTOR3 WHITEBACK_POS = { 0.f,0.f,0.f };				//白背景のポジション.
	//巻物のポジション.
	const D3DXVECTOR3 SCROLL_POS	= { 650.f, 360.f, 0.f };		//白背景のポジション.
	//テキストのポジション.
	const D3DXVECTOR3 TEXT1_POS		= { 400.f, 484.f, 0.f };		//テキストのポジション.
	const D3DXVECTOR3 TEXT2_POS		= { 860.f, 484.f, 0.f };		//テキストのポジション.
	//セレクトフレームポジション.
	const D3DXVECTOR3 SELECTIONFRAME_POS = { 400.f, 484.f, 0.f };	//セレクトフレームのポジション.
	//タイトルに戻る時のテキスト.
	const D3DXVECTOR3 TITLEBACK1_TEXT_POS = { 340.f, 140.f, 0.f };	//テキストのポジション.
	const D3DXVECTOR3 TITLEBACK2_TEXT_POS = { 748.f, 432.f, 0.f };	//テキストのポジション.
	const D3DXVECTOR3 TITLEBACK3_TEXT_POS = { 262.f, 406.f, 0.f };	//テキストのポジション.
	//準備完了の初期ポジション.
	const D3DXVECTOR3 PLAYER1_GAMEREADY_POS  = { 36.f	, 50.f, 0.f };		//プレイヤー1準備完了のポジション.
	const D3DXVECTOR3 PLAYER2_GAMEREADY_POS  = { 325.4f	, 50.f, 0.f };		//プレイヤー2準備完了のポジション.
	const D3DXVECTOR3 PLAYER3_GAMEREADY_POS  = { 645.4f	, 50.f, 0.f };		//プレイヤー3準備完了のポジション.
	const D3DXVECTOR3 PLAYER4_GAMEREADY_POS  = { 965.4f	, 50.f, 0.f };		//プレイヤー4準備完了のポジション.
	//パターンナンバー.
	const SHORT PATTERN_NONE				= { 0 };			//x座標のpatternナンバー.
	const SHORT RIGHT_HAND_YPATTERNNO		= { 1 };			//x座標のpatternナンバー.
	const SHORT GAMEREADY_YPATTERNNO		= { 5 };			//準備完了のy座標のpatternナンバー.
	const SHORT TITLEBACK2_TEXT_YPATTERNNO	= { 3 };			//準備完了のy座標のpatternナンバー.
	const SHORT TITLEBACK3_TEXT_YPATTERNNO	= { 4 };			//タイトルバックテキストのy座標のpatternナンバー.
	//スケール値.
	const D3DXVECTOR3 PLAYER_HAND_SCALE		 = { 110.f	, 130.f , 0.f };		//プレイヤーの手の初期スケール値.
	const D3DXVECTOR3 PLAYER_GAMEREADY_SCALE = { 350.f	, 170.f , 0.f };		//プレイヤーの準備完了のスケール値.
	const D3DXVECTOR3 WHITEBACK_SCALE		 = { 1280.f , 720.f , 0.f };		//白い背景のスケール値.
	const D3DXVECTOR3 SCROLL_SCALE			 = { 800.f	, 1350.f, 0.f };		//巻物のスケール値.
	const D3DXVECTOR3 TEXT_SCALE			 = { 350.f	, 150.f , 0.f };		//テキストのスケール値.
	const D3DXVECTOR3 SELECTIONFRAME_SCALE	 = { 350.f	, 150.f , 0.f };		//セレクトフレームのスケール値.
	const D3DXVECTOR3 TITLEBACK1_TEXT_SCALE	 = { 600.f	, 260.f , 0.f };		//タイトルに戻る時のテキストスケール.
	const D3DXVECTOR3 TITLEBACK2_TEXT_SCALE	 = { 600.f	, 150.f , 0.f };		//タイトルに戻る時のテキストスケール.		
	//ローテーション.
	const D3DXVECTOR3 PLAYER_GAMEREADY_ROTATION = { 0.f	,0.f ,0.3f };		//準備完了のローテーション(回転)値.
	const D3DXVECTOR3 ROTATION_NONE				= { 0.f, 0.f, 0.f };		//回転が必要ないときのローテーション.
	const D3DXVECTOR3 SCROLL_ROTATION			= { 0.f,0.f,1.57f };		//巻物のローテーション.

	constexpr float	SELECT_CHANGE_INTERVAL   = 15.f;		//セレクトを切り替えるまでのフレーム.
	constexpr float	STICK_DEAD_ZONE			 = 0.5f;		//デッドゾーン(スティックを傾けた際の倒す数値).
	constexpr float	HANDCLAP_POS			 = 39.f;		//手の最終移動位置.
	constexpr float	HAND_SPEED				 = 4.f;			//手の移動速度.
	constexpr float	ALPHA_CONNECTED			 = 1.0f;		//接続時アルファ.
	constexpr float	ALPHA_OFF				 = 0.f;			//非接続時アルファ.
	constexpr float HAND_REACH_THRESHOLD	 = 39.f;		//手が一定のポジションまで移動する数値.
	constexpr float ALPHA_TITLEBACK			 = 0.5f;		//タイトルに戻るときに背景を白くする際のアルファ値.
}

CGameRdyUI::CGameRdyUI()
{
	//手の最大数分(4).
	for (int i = 0; i < hand_Max; i++) {
		m_pRightHand[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);			//手の画像.
		m_pLeftHand[i]  = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);			//手の画像.
		m_pGamerdy[i]	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);			//テキスト画像.
	}
	//テキスト最大数(3).
	for (int i = 0; i < Text_Max; i++) {
		m_pTitleBackText[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);		//タイトルに戻るテキスト.
	}
	//フレームの最大数.
	for (int i = 0; i < Frame_Max; i++) {
		m_pTextFrame[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TextFrame);		//テキストのフレーム.
	}
	m_pWhiteBack		= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_WhiteBack);		//白の背景画像.
	m_pScroll			= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);			//巻物.
	m_pSelectionFrame	= CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_SelectionFrame);	//セレクトフレーム.
}

CGameRdyUI::~CGameRdyUI()
{
}

void CGameRdyUI::Update()
{
	//連続でセレクトを動かせないようにするため(バグ防止、後で数値かえるかも).
	{
		//セレクトフレームを動かせないなら.
		if (!m_SelectTimeOver) {
			++m_SelectchangeTimer;	//タイマー増加.
			//タイマーが一定のフレーム数を超えたなら.
			if (m_SelectchangeTimer > SELECT_CHANGE_INTERVAL) {
				m_SelectTimeOver = true;	//セレクトフレームを動かせるようにする.
				m_SelectchangeTimer = 0;	//タイマー初期化.
			}
		}
	}
	//タイトルに戻る画面が出ているなら.
	if (m_TitleBackflag) {
		//セレクトフレームを移動させていいか
		if (m_SelectTimeOver) {
			//左スティックを左に倒したなら.
			if (m_pController[0]->GetLThumbX() > STICK_DEAD_ZONE) {
				m_IsYesSelected = true;		//はいを選択中.
				m_SelectTimeOver = false;	//セレクトフレームを動かせないようにする.
			}
			//左スティックを右に倒したなら.
			else if (m_pController[0]->GetLThumbX() < -STICK_DEAD_ZONE) {
				m_IsYesSelected = false;	//いいえを選択中.
				m_SelectTimeOver = false;	//セレクトフレームを動かせないようにする.
			}
		}
		//はいを選択中なら.
		if (m_IsYesSelected) {
			//Aボタンを押した.
			if (m_pController[0]->IsDown(CXInput::A, true)) {
				//テキストメッセージ(仮).
				MessageBox(nullptr,
					_T("タイトルに戻りますか？"),
					_T("警告"), MB_YESNO == IDYES);
			}
		}
		//いいえを選択中なら.
		else {
			//Aボタンを押した.
			if (m_pController[0]->IsDown(CXInput::A, true)) {
				m_TitleBackflag = false;	//タイトルに戻る描画をしない.
			}
		}
		return;	//リターンで返して以降通らないようにする.
	}


	//準備完了ではないなら.
	if (!m_Handclapflag[0]) {
		//トリガー判定.
		if (m_pController[0]->IsDown(CXInput::B, true)) {
			m_TitleBackflag = true;		//タイトルに戻る画面を表示する.
			m_IsYesSelected = false;	//いいえを選択している状態に戻す.
			return;		//以降通らないようにする.
		}
	}
	//準備完了状態になる処理.
	//プレイヤーの最大数分.
	for (int i = 0; i < hand_Max; i++) {
		//Aボタン準備完了.
		if (m_pController[i]->IsDown(CXInput::A, true)) {
			m_Handclapflag[i] = true;	//準備完了状態へ.
		}
		//Bボタンを押すと準備完了解除.
		else if (m_pController[i]->IsDown(CXInput::B, true)) {
			//準備完了状態なら.
			if (m_Handclapflag[i]) {
				m_Handclapflag[i] = false;	//準備完了解除.
			}
		}
		//準備完了状態なら.
		if (m_Handclapflag[i]) {
			//手が一定の位置まで移動していなければ.
			if (m_LeftHandclap[i] < HANDCLAP_POS) {
				//手を動かす.
				m_RightHandclap[i] -= HAND_SPEED;
				m_LeftHandclap[i]  += HAND_SPEED;
			}
		}
		else {
			//位置リセット.
			m_RightHandclap[i]	= 0.f;
			m_LeftHandclap[i]	= 0.f;
		}
	}
}

void CGameRdyUI::Draw()
{
	//各関数呼び出し.
	DrawRightHand();
	DrawLeftHand();
	DrawGameRdy();
	DrawTitleBack();
}
void CGameRdyUI::TitleBackUpdate()
{
}
void CGameRdyUI::GameRdyHandUpdate()
{
}
//右手.
void CGameRdyUI::DrawRightHand()
{
	//プレイヤー1のボタンが押されたなら.
	if (m_Handclapflag[0]) {
		m_pRightHand[0]->SetAlpha(ALPHA_OFF);
		m_pRightHand[0]->SetPosition(D3DXVECTOR3(PLAYER1_RIGHTHAND_POS.x + m_RightHandclap[0], PLAYER1_RIGHTHAND_POS.y, PLAYER1_RIGHTHAND_POS.z));
		m_pRightHand[0]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pRightHand[0]->SetPatternNo(PATTERN_NONE, RIGHT_HAND_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[0]->IsConnect()) {
			m_pRightHand[0]->SetAlpha(ALPHA_CONNECTED); //α値を戻す.
		}
		m_pRightHand[0]->Render();
	}
	//プレイヤー2のボタンが押されたなら.
	if (m_Handclapflag[1]) {
		m_pRightHand[1]->SetAlpha(ALPHA_OFF);
		m_pRightHand[1]->SetPosition(D3DXVECTOR3(PLAYER2_RIGHTHAND_POS.x + m_RightHandclap[1], PLAYER2_RIGHTHAND_POS.y, PLAYER2_RIGHTHAND_POS.z));
		m_pRightHand[1]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pRightHand[1]->SetPatternNo(PATTERN_NONE, RIGHT_HAND_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[1]->IsConnect()) {
			m_pRightHand[1]->SetAlpha(ALPHA_CONNECTED); //α値を戻す.
		}
		m_pRightHand[1]->Render();
	}
	//プレイヤー3のボタンが押されたなら.
	if (m_Handclapflag[2]) {
		m_pRightHand[2]->SetAlpha(ALPHA_OFF);
		m_pRightHand[2]->SetPosition(D3DXVECTOR3(PLAYER3_RIGHTHAND_POS.x + m_RightHandclap[2], PLAYER3_RIGHTHAND_POS.y, PLAYER3_RIGHTHAND_POS.z));
		m_pRightHand[2]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pRightHand[2]->SetPatternNo(PATTERN_NONE, RIGHT_HAND_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[2]->IsConnect()) {
			m_pRightHand[2]->SetAlpha(ALPHA_CONNECTED); //α値を戻す.
		}
		m_pRightHand[2]->Render();
	}
	//プレイヤー4のボタンが押されたなら..
	if (m_Handclapflag[3]) {
		m_pRightHand[3]->SetAlpha(ALPHA_OFF);
		m_pRightHand[3]->SetPosition(D3DXVECTOR3(PLAYER4_RIGHTHAND_POS.x + m_RightHandclap[3], PLAYER4_RIGHTHAND_POS.y, PLAYER4_RIGHTHAND_POS.z));
		m_pRightHand[3]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pRightHand[3]->SetPatternNo(PATTERN_NONE, RIGHT_HAND_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[3]->IsConnect()) {
			m_pRightHand[3]->SetAlpha(ALPHA_CONNECTED); //α値を戻す.
		}
		m_pRightHand[3]->Render();
	}
}
//左手.
void CGameRdyUI::DrawLeftHand()
{
	//プレイヤー1のボタンが押されたなら.
	if (m_Handclapflag[0]) {
		m_pLeftHand[0]->SetAlpha(ALPHA_OFF);
		m_pLeftHand[0]->SetPosition(D3DXVECTOR3(PLAYER1_LEFTHAND_POS.x + m_LeftHandclap[0], PLAYER1_LEFTHAND_POS.y, PLAYER1_LEFTHAND_POS.z));
		m_pLeftHand[0]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pLeftHand[0]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		//コントローラーが接続されているか.
		if (m_pController[0]->IsConnect()) {
			m_pLeftHand[0]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pLeftHand[0]->Render();
	}
	//プレイヤー2のボタンが押されたなら.
	if (m_Handclapflag[1]) {
		m_pLeftHand[1]->SetAlpha(ALPHA_OFF);
		m_pLeftHand[1]->SetPosition(D3DXVECTOR3(PLAYER2_LEFTHAND_POS.x + m_LeftHandclap[1], PLAYER2_LEFTHAND_POS.y, PLAYER2_LEFTHAND_POS.z));
		m_pLeftHand[1]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pLeftHand[1]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		//コントローラーが接続されているか.
		if (m_pController[1]->IsConnect()) {
			m_pLeftHand[1]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pLeftHand[1]->Render();
	}
	//プレイヤー3のボタンが押されたなら.
	if (m_Handclapflag[2]) {
		m_pLeftHand[2]->SetAlpha(ALPHA_OFF);
		m_pLeftHand[2]->SetPosition(D3DXVECTOR3(PLAYER3_LEFTHAND_POS.x + m_LeftHandclap[2], PLAYER3_LEFTHAND_POS.y, PLAYER3_LEFTHAND_POS.z));
		m_pLeftHand[2]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pLeftHand[2]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		//コントローラーが接続されているか.
		if (m_pController[2]->IsConnect()) {
			m_pLeftHand[2]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pLeftHand[2]->Render();
	}
	//プレイヤー4のボタンが押されたなら.
	if (m_Handclapflag[3]) {
		m_pLeftHand[3]->SetAlpha(ALPHA_OFF);
		m_pLeftHand[3]->SetPosition(D3DXVECTOR3(PLAYER4_LEFTHAND_POS.x + m_LeftHandclap[3], PLAYER4_LEFTHAND_POS.y, PLAYER4_LEFTHAND_POS.z));
		m_pLeftHand[3]->SetScale(D3DXVECTOR3(PLAYER_HAND_SCALE));
		m_pLeftHand[3]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		//コントローラーが接続されているなら.
		if (m_pController[3]->IsConnect()) {
			m_pLeftHand[3]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pLeftHand[3]->Render();
	}
}
//準備完了状態の描画.
void CGameRdyUI::DrawGameRdy()
{
	if (m_LeftHandclap[0] > HAND_REACH_THRESHOLD) {
		m_pGamerdy[0]->SetAlpha(ALPHA_OFF);
		m_pGamerdy[0]->SetPosition(D3DXVECTOR3(PLAYER1_GAMEREADY_POS));
		m_pGamerdy[0]->SetScale(D3DXVECTOR3(PLAYER_GAMEREADY_SCALE));
		m_pGamerdy[0]->SetRotation(D3DXVECTOR3(PLAYER_GAMEREADY_ROTATION));
		m_pGamerdy[0]->SetPatternNo(PATTERN_NONE, GAMEREADY_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[0]->IsConnect()) {
			m_pGamerdy[0]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pGamerdy[0]->Render();
	}
	if (m_LeftHandclap[1] > HAND_REACH_THRESHOLD) {
		m_pGamerdy[1]->SetAlpha(ALPHA_OFF);
		m_pGamerdy[1]->SetPosition(D3DXVECTOR3(PLAYER2_GAMEREADY_POS.x + m_LeftHandclap[1], PLAYER2_GAMEREADY_POS.y, PLAYER2_GAMEREADY_POS.z));
		m_pGamerdy[1]->SetScale(D3DXVECTOR3(PLAYER_GAMEREADY_SCALE));
		m_pGamerdy[1]->SetRotation(D3DXVECTOR3(PLAYER_GAMEREADY_ROTATION));
		m_pGamerdy[1]->SetPatternNo(PATTERN_NONE, GAMEREADY_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[1]->IsConnect()) {
			m_pGamerdy[1]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pGamerdy[1]->Render();
	}
	//プレイヤー3.
	if (m_LeftHandclap[2] > HAND_REACH_THRESHOLD) {
		m_pGamerdy[2]->SetAlpha(ALPHA_OFF);
		m_pGamerdy[2]->SetPosition(D3DXVECTOR3(PLAYER3_GAMEREADY_POS.x + m_LeftHandclap[2], PLAYER3_GAMEREADY_POS.y, PLAYER3_GAMEREADY_POS.z));
		m_pGamerdy[2]->SetScale(D3DXVECTOR3(PLAYER_GAMEREADY_SCALE));
		m_pGamerdy[2]->SetRotation(D3DXVECTOR3(PLAYER_GAMEREADY_ROTATION));
		m_pGamerdy[2]->SetPatternNo(PATTERN_NONE, GAMEREADY_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[2]->IsConnect()) {
			m_pGamerdy[2]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pGamerdy[2]->Render();
	}
	//プレイヤー4.
	if (m_LeftHandclap[3] > HAND_REACH_THRESHOLD) {
		m_pGamerdy[3]->SetAlpha(ALPHA_OFF);
		m_pGamerdy[3]->SetPosition(D3DXVECTOR3(PLAYER4_GAMEREADY_POS.x + m_LeftHandclap[3], PLAYER4_GAMEREADY_POS.y, PLAYER4_GAMEREADY_POS.z));
		m_pGamerdy[3]->SetScale(D3DXVECTOR3(PLAYER_GAMEREADY_SCALE));
		m_pGamerdy[3]->SetRotation(D3DXVECTOR3(PLAYER_GAMEREADY_ROTATION));
		m_pGamerdy[3]->SetPatternNo(PATTERN_NONE, GAMEREADY_YPATTERNNO);
		//コントローラーが接続されているか.
		if (m_pController[3]->IsConnect()) {
			m_pGamerdy[3]->SetAlpha(ALPHA_CONNECTED);
		}
		m_pGamerdy[3]->Render();
	}
}
//タイトルに戻るときのUI.
void CGameRdyUI::DrawTitleBack()
{
	//タイトルに戻る描画を出していいなら.
	if (m_TitleBackflag) {
		//白背景.
		m_pWhiteBack->SetAlpha(ALPHA_TITLEBACK);
		m_pWhiteBack->SetPosition(D3DXVECTOR3(WHITEBACK_POS));
		m_pWhiteBack->SetScale(D3DXVECTOR3(WHITEBACK_SCALE));
		m_pWhiteBack->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		m_pWhiteBack->SetRotation(D3DXVECTOR3(ROTATION_NONE));
		m_pWhiteBack->Render();
		//巻物.
		m_pScroll->SetAlpha(ALPHA_CONNECTED);
		m_pScroll->SetPosition(D3DXVECTOR3(SCROLL_POS));
		m_pScroll->SetScale(D3DXVECTOR3(SCROLL_SCALE));
		m_pScroll->SetRotation(D3DXVECTOR3(SCROLL_ROTATION));
		m_pScroll->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		m_pScroll->Render();
		//テキストのフレーム.
		m_pTextFrame[0]->SetAlpha(ALPHA_CONNECTED);
		m_pTextFrame[0]->SetPosition(D3DXVECTOR3(TEXT1_POS));
		m_pTextFrame[0]->SetScale(D3DXVECTOR3(TEXT_SCALE));
		m_pTextFrame[0]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
		m_pTextFrame[0]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		m_pTextFrame[0]->Render();

		m_pTextFrame[1]->SetAlpha(ALPHA_CONNECTED);
		m_pTextFrame[1]->SetPosition(D3DXVECTOR3(TEXT2_POS));
		m_pTextFrame[1]->SetScale(D3DXVECTOR3(TEXT_SCALE));
		m_pTextFrame[1]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
		m_pTextFrame[1]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		m_pTextFrame[1]->Render();

		//はいを選択中なら.
		if (m_IsYesSelected) {
			//セレクトフレーム.
			m_pSelectionFrame->SetAlpha(ALPHA_CONNECTED);
			m_pSelectionFrame->SetPosition(D3DXVECTOR3(TEXT2_POS));
			m_pSelectionFrame->SetScale(D3DXVECTOR3(SELECTIONFRAME_SCALE));
			m_pSelectionFrame->SetRotation(D3DXVECTOR3(ROTATION_NONE));
			m_pSelectionFrame->Render();
		}
		//いいえを選択中なら.
		else {
			//セレクトフレーム.
			m_pSelectionFrame->SetAlpha(ALPHA_CONNECTED);
			m_pSelectionFrame->SetPosition(D3DXVECTOR3(SELECTIONFRAME_POS));
			m_pSelectionFrame->SetScale(D3DXVECTOR3(SELECTIONFRAME_SCALE));
			m_pSelectionFrame->SetRotation(D3DXVECTOR3(ROTATION_NONE));
			m_pSelectionFrame->Render();
		}

		//テキスト.(タイトルに戻りますか?).
		m_pTitleBackText[0]->SetAlpha(ALPHA_CONNECTED);
		m_pTitleBackText[0]->SetPosition(D3DXVECTOR3(TITLEBACK1_TEXT_POS));
		m_pTitleBackText[0]->SetScale(D3DXVECTOR3(TITLEBACK1_TEXT_SCALE));
		m_pTitleBackText[0]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
		m_pTitleBackText[0]->SetPatternNo(PATTERN_NONE, PATTERN_NONE);
		m_pTitleBackText[0]->Render();
		//(はい)
		m_pTitleBackText[1]->SetAlpha(ALPHA_CONNECTED);
		m_pTitleBackText[1]->SetPosition(D3DXVECTOR3(TITLEBACK2_TEXT_POS));
		m_pTitleBackText[1]->SetScale(D3DXVECTOR3(TITLEBACK2_TEXT_SCALE));
		m_pTitleBackText[1]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
		m_pTitleBackText[1]->SetPatternNo(PATTERN_NONE, TITLEBACK2_TEXT_YPATTERNNO);
		m_pTitleBackText[1]->Render();
		//(いいえ)
		m_pTitleBackText[2]->SetAlpha(ALPHA_CONNECTED);
		m_pTitleBackText[2]->SetPosition(D3DXVECTOR3(TITLEBACK3_TEXT_POS));
		m_pTitleBackText[2]->SetScale(D3DXVECTOR3(TITLEBACK2_TEXT_SCALE));
		m_pTitleBackText[2]->SetRotation(D3DXVECTOR3(ROTATION_NONE));
		m_pTitleBackText[2]->SetPatternNo(PATTERN_NONE, TITLEBACK3_TEXT_YPATTERNNO);
		m_pTitleBackText[2]->Render();

	}
}
