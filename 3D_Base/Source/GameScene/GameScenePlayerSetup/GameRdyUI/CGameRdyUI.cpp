#include "CGameRdyUI.h"

namespace {
	//画像1枚ぶんの表示設定
	struct SpriteLayout
	{
		D3DXVECTOR3	pos;		//位置.
		D3DXVECTOR3	scale;		//大きさ.
		D3DXVECTOR3	rot;		//回転.
		SHORT		patternY;	//画像の何段目を使うか(横は常に0列目).
	};

	constexpr SHORT		PATTERN_X = 0;					//画像の横のパターン番号(常に0列目).
	const D3DXVECTOR3	ROT_NONE = { 0.f, 0.f, 0.f };	//回転なし.

	//プレイヤーごとの手と「準備完了」の位置(X座標)
	struct PlayerLayout
	{
		float rightHandX;	//右手の初期位置.
		float leftHandX;	//左手の初期位置.
		float readyTextX;	//「準備完了」の位置.
	};
	const PlayerLayout PLAYER_LAYOUTS[] = {
		//右手		左手		準備完了
		{ 80.f,		0.f,		16.f },		//1P.
		{ 404.4f,	325.4f,		345.4f },	//2P.
		{ 724.4f,	645.4f,		645.4f },	//3P.
		{ 1044.4f,	965.4f,		985.4f },	//4P.
	};
	//X座標はプレイヤーごとの値を足して使うのでここでは0にしておく.
	const SpriteLayout RIGHT_HAND_LAYOUT =	{ { 0.f, 80.f, 0.f },{ 110.f, 130.f, 0.f }, ROT_NONE,			1 };
	const SpriteLayout LEFT_HAND_LAYOUT =	{ { 0.f, 80.f, 0.f },{ 110.f, 130.f, 0.f }, ROT_NONE,			0 };
	const SpriteLayout READY_TEXT_LAYOUT =	{ { 0.f, 50.f, 0.f },{ 350.f, 170.f, 0.f },{ 0.f, 0.f, 0.3f },	5 };

	//タイトルに戻る画面.
	const SpriteLayout WHITEBACK_LAYOUT = { { 0.f,   0.f,   0.f },{ 1280.f, 720.f,  0.f }, ROT_NONE,		0 };
	const SpriteLayout SCROLL_LAYOUT = { { 650.f, 360.f, 0.f },{ 800.f,  1350.f, 0.f },{ 0.f, 0.f, 1.57f },	0 };	//約90度回して横向きにする.

	constexpr int FRAME_NO = 0;		//「いいえ」の枠の番号.
	constexpr int FRAME_YES = 1;	//「はい」の枠の番号.
	const SpriteLayout TEXT_FRAME_LAYOUTS[] = {
		{ { 400.f, 484.f, 0.f },{ 350.f, 150.f, 0.f }, ROT_NONE, 0 },	//[0]左:いいえ.
		{ { 860.f, 484.f, 0.f },{ 350.f, 150.f, 0.f }, ROT_NONE, 0 },	//[1]右:はい.
	};
	const SpriteLayout TITLEBACK_TEXT_LAYOUTS[] = {
		{ { 340.f, 140.f, 0.f },{ 600.f, 260.f, 0.f }, ROT_NONE, 0 },	//[0]タイトルに戻りますか?
		{ { 748.f, 432.f, 0.f },{ 600.f, 150.f, 0.f }, ROT_NONE, 3 },	//[1]はい.
		{ { 262.f, 406.f, 0.f },{ 600.f, 150.f, 0.f }, ROT_NONE, 4 },	//[2]いいえ.
	};

	//全員準備完了
	const SpriteLayout ALL_READY_LAYOUT = { { -150.f, 250.f, 0.f },{ 1550.f, 450.f, 0.f },{ 0.f, 0.f, -0.08f }, 0 };

	//動き・操作
	constexpr int	SELECT_COOLDOWN_FRAME = 16;	//選択を切り替えたあと、次に切り替えられるまでのフレーム.
	constexpr float	STICK_DEAD_ZONE = 0.5f;		//スティックをこれ以上倒したら入力とみなす.
	constexpr float	HANDCLAP_POS = 39.f;		//手を合わせきる移動量.
	constexpr float	HAND_SPEED = 4.f;			//手の移動速度(1フレームあたり).

	//透明度
	constexpr float	ALPHA_OPAQUE = 1.0f;		//不透明.
	constexpr float	ALPHA_WHITEBACK = 0.5f;		//タイトルに戻るときに、背景を白くぼかす.
	constexpr float	ALPHA_ALL_READY = 0.9f;		//全員準備完了の表示.

	//同じ画像を何か所にも描くので、描画のたびに全部の設定をし直す.
	void DrawSprite(CSprite2D* sprite, const SpriteLayout& layout, float alpha, float offsetX = 0.f)
	{
		sprite->SetPosition(D3DXVECTOR3(layout.pos.x + offsetX, layout.pos.y, layout.pos.z));
		sprite->SetScale(layout.scale);
		sprite->SetRotation(layout.rot);
		sprite->SetPatternNo(PATTERN_X, layout.patternY);
		sprite->SetAlpha(alpha);
		sprite->Render();
	}
}

CGameRdyUI::CGameRdyUI()
{
	m_pHand = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Hand);
	m_pText = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Text);
	m_pTextFrame = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_TextFrame);
	m_pSelectionFrame = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_SelectionFrame);
	m_pWhiteBack = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_WhiteBack);
	m_pScroll = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Scroll);
	m_pAllReady = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_ScrollRed);
}

CGameRdyUI::~CGameRdyUI() = default;	//画像もコントローラーも借りているだけなので、ここでは解放しない.

//更新.
void CGameRdyUI::Update()
{
	//選択の連続切り替えを防ぐ待ち時間を減らす.
	if (m_SelectCooldown > 0) {
		--m_SelectCooldown;
	}

	//タイトルに戻る画面が出ている間は、その操作だけを受け付ける.
	if (m_TitleBackflag) {
		UpdateTitleBack();
		return;
	}
	if (TryOpenTitleBack()) return;

	UpdateReadyInput();
	//全員準備完了の判定は、前のフレームの結果を使う.
	//(最後の1人が準備完了にしたAボタンで、そのままゲームが始まらないようにするため).
	UpdateAllReady();
	UpdateConnection();
}

//タイトルに戻る画面での選択と決定.
void CGameRdyUI::UpdateTitleBack()
{
	CXInput* pad = m_pController[0];	//タイトルに戻る画面は1Pだけが操作する.

	//スティックで「はい/いいえ」を切り替える.
	if (m_SelectCooldown == 0) {
		const float stickX = pad->GetLThumbX();
		const bool prevYesSelected = m_IsYesSelected;	//変更前の選択を保存.


		if (stickX > STICK_DEAD_ZONE) {
			m_IsYesSelected = true;		//はい選択中.
		}
		else if (stickX < -STICK_DEAD_ZONE) {
			m_IsYesSelected = false;	//いいえ選択中.
		}
		//選択が変わったときだけ、音を鳴らして待ち時間に入る.
		if (m_IsYesSelected != prevYesSelected) {
			m_SelectCooldown = SELECT_COOLDOWN_FRAME;
			XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::SelectChange);
		}
	}

	//Aボタンで決定.
	if (!pad->IsDown(CXInput::A, true)) return;

	XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::Decision);
	//「はい」を選んでいたらタイトルに戻る、そうでなければ画面を閉じる.
	if (m_IsYesSelected) {
		m_TitleSceneChangeflag = true;	//はい:タイトルに戻る.
	}
	else {
		m_TitleBackflag = false;		//いいえ:画面を閉じる.
	}
}

//1Pが準備完了でないとき、Bボタンでタイトルに戻る画面を開く.
bool CGameRdyUI::TryOpenTitleBack()
{
	if (m_IsReady[0]) return false;	//準備完了中のBは「解除」に使う.
	if (!m_pController[0]->IsDown(CXInput::B, true)) return false;

	m_TitleBackflag = true;
	m_IsYesSelected = false;	//誤ってタイトルに戻らないよう、「いいえ」から始める.
	XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::Cancel);
	return true;
}

//各プレイヤーの準備完了・解除と、手の動き.
void CGameRdyUI::UpdateReadyInput()
{
	for (int i = 0; i < hand_Max; i++) {
		CXInput* pad = m_pController[i];

		//Aボタンで準備完了、Bボタンで解除.
		if (!m_IsReady[i]) {
			if (pad->IsDown(CXInput::A, true)) {
				m_IsReady[i] = true;
				XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::Handclap);
			}
		}
		else if (pad->IsDown(CXInput::B, true)) {
			m_IsReady[i] = false;
		}

		//準備完了中は手を合わせるまで動かし、解除されたら元の位置に戻す.
		if (!m_IsReady[i]) {
			m_ClapOffset[i] = 0.f;
		}
		else if (m_ClapOffset[i] < HANDCLAP_POS) {
			m_ClapOffset[i] += HAND_SPEED;
		}
	}
}

//全員準備完了のときの演出と、ゲーム開始.
void CGameRdyUI::UpdateAllReady()
{
	if (!m_IsAllReady) {
		m_AllReadySEPlayed = false;	//次に全員が揃ったとき、また鳴らせるようにする.
		return;
	}
	//揃った瞬間に1回だけSEを鳴らす.
	if (!m_AllReadySEPlayed) {
		XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::AllrdyCutin);
		m_AllReadySEPlayed = true;
	}
	//1PのAボタンでゲーム開始.
	if (m_pController[0]->IsDown(CXInput::A, true)) {
		m_GameMainSceneChangeflag = true;
	}
}

//接続数を数え、全員準備完了かを判定する.
void CGameRdyUI::UpdateConnection()
{
	int connectedCount = 0;	//接続されているコントローラーの数.
	int readyCount = 0;		//そのうち準備完了の数.

	for (int i = 0; i < hand_Max; i++) {
		if (!IsConnected(i)) {
			m_WasConnected[i] = false;	//次に接続されたとき、また鳴らせるようにする.
			continue;
		}
		//接続された瞬間に1回だけSEを鳴らす.
		if (!m_WasConnected[i]) {
			XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::Connected);
			m_WasConnected[i] = true;
		}
		++connectedCount;
		if (m_IsReady[i]) {
			++readyCount;
		}
	}
	//2人以上が接続していて、その全員が準備完了なら開始できる.
	m_IsAllReady = (connectedCount >= 2 && connectedCount == readyCount);
}
//描画.
void CGameRdyUI::Draw()
{
	DrawHands();
	DrawReadyTexts();
	if (m_TitleBackflag) {
		DrawTitleBack();
	}
	if (m_IsAllReady) {
		DrawAllReady();
	}
}

//準備完了したプレイヤーの手.
void CGameRdyUI::DrawHands()
{
	for (int i = 0; i < hand_Max; i++) {
		if (!m_IsReady[i] || !IsConnected(i)) continue;

		const PlayerLayout& layout = PLAYER_LAYOUTS[i];
		DrawSprite(m_pHand, RIGHT_HAND_LAYOUT, ALPHA_OPAQUE, layout.rightHandX - m_ClapOffset[i]);	//右手は左へ.
		DrawSprite(m_pHand, LEFT_HAND_LAYOUT, ALPHA_OPAQUE, layout.leftHandX + m_ClapOffset[i]);	//左手は右へ.
	}
}

//手を合わせ終わったプレイヤーの「準備完了」.
void CGameRdyUI::DrawReadyTexts()
{
	for (int i = 0; i < hand_Max; i++) {
		if (!IsClapFinished(i) || !IsConnected(i)) continue;
		DrawSprite(m_pText, READY_TEXT_LAYOUT, ALPHA_OPAQUE, PLAYER_LAYOUTS[i].readyTextX);
	}
}

//タイトルに戻る確認画面.
void CGameRdyUI::DrawTitleBack()
{
	DrawSprite(m_pWhiteBack, WHITEBACK_LAYOUT, ALPHA_WHITEBACK);
	DrawSprite(m_pScroll, SCROLL_LAYOUT, ALPHA_OPAQUE);

	for (const SpriteLayout& frame : TEXT_FRAME_LAYOUTS) {
		DrawSprite(m_pTextFrame, frame, ALPHA_OPAQUE);
	}
	//選んでいる方の枠に、セレクトフレームを重ねる.
	if (m_IsYesSelected) {
		DrawSprite(m_pSelectionFrame, TEXT_FRAME_LAYOUTS[FRAME_YES], ALPHA_OPAQUE);	//右の枠(はい)に重ねる.
	}
	else {
		DrawSprite(m_pSelectionFrame, TEXT_FRAME_LAYOUTS[FRAME_NO], ALPHA_OPAQUE);	//左の枠(いいえ)に重ねる.
	}
	for (const SpriteLayout& text : TITLEBACK_TEXT_LAYOUTS) {
		DrawSprite(m_pText, text, ALPHA_OPAQUE);
	}
}

//全員準備完了の表示.
void CGameRdyUI::DrawAllReady()
{
	DrawSprite(m_pAllReady, ALL_READY_LAYOUT, ALPHA_ALL_READY);
}

//==================================================
//	補助.
//==================================================
bool CGameRdyUI::IsConnected(int player) const
{
	//SetXInput が呼ばれていない番号でも落ちないよう、nullptr を確認する.
	return m_pController[player] != nullptr && m_pController[player]->IsConnect();
}

bool CGameRdyUI::IsClapFinished(int player) const
{
	return m_ClapOffset[player] >= HANDCLAP_POS;
}