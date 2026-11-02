#include "CButtonMashBattleUI.h"
#include "SpriteManager/CSpriteManager.h"

namespace {
	//---------------------------------------------------
	//	ゲージの見た目に関する定数.
	//---------------------------------------------------
	constexpr float GAUGE_WIDTH   = 150.f;	//ゲージ1本分の表示幅.
	constexpr float GAUGE_HEIGHT  = 36.f;	//ゲージ1本分の表示高さ.
	constexpr float GAUGE_MARGIN  = 8.f;	//ゲージ同士の縦の間隔.
	constexpr float GAUGE_ANCHOR_OFFSET_Y = 70.f;	//具材の画面上の位置から、どれだけ上にゲージをずらすか.
	constexpr float GAUGE_BG_ALPHA = 0.3f;	//連打していない部分のα値.

	//---------------------------------------------------
	//	カウントダウン中の対戦者表示に関する定数.
	//---------------------------------------------------
	constexpr float NAME_HEIGHT  = 30.f;						//プレイヤー名1つ分の表示高さ.
	constexpr float NAME_WIDTH   = NAME_HEIGHT * 226.f / 59.f;	//プレイヤー名1つ分の表示幅(元画像の縦横比を保つ).
	constexpr float VS_HEIGHT    = 34.f;						//「対」の文字の表示高さ.
	constexpr float VS_WIDTH     = VS_HEIGHT * 50.f / 49.f;		//「対」の文字の表示幅(画像1コマ分の縦横比を保つ).
	constexpr float NAME_SPACING = 6.f;							//プレイヤー名と「対」の間隔.
	constexpr float TOP_ROW_HEIGHT = (VS_HEIGHT > NAME_HEIGHT) ? VS_HEIGHT : NAME_HEIGHT;	//上段(対戦者名)全体の高さ.
	constexpr float ROW_GAP      = 6.f;							//上段(対戦者名)と下段(数字)の間隔.
	constexpr float DIGIT_SIZE   = GAUGE_HEIGHT;				//カウントダウンの数字の表示サイズ.

	//---------------------------------------------------
	//	連打中のボタン表示に関する定数.
	//---------------------------------------------------
	constexpr float BUTTON_WIDTH  = 70.f;					//ボタン表示の幅(マヒ中の表示 100x130 と同じ比率).
	constexpr float BUTTON_HEIGHT = 91.f;					//ボタン表示の高さ.
	constexpr float BUTTON_OFFSET_X = 8.f;					//ゲージの右端からボタン表示までの間隔.
	constexpr int   BUTTON_ANIM_FRAMES = 4;					//ボタンの押す/離すの絵を切り替えるフレーム数.
}

CButtonMashBattleUI::CButtonMashBattleUI()
{
	//ゲージ画像の取得.
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		m_pGaugeImg[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_Gage);
	}
	//カウントダウン表示用(スコア表示等でも使っている0〜9の数字画像を流用する).
	m_pCountdownDigit = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Digit0_9);
	//対戦者表示用(ゲージと同じ色の付いたプレイヤーフォントを流用する).
	m_pPlayerName = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_WinnerFont);
	m_pVsFont = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_VsFont);
	//連打するボタンの表示用(マヒ中に出しているボタン画像を流用する).
	m_pButtonGuide = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_ButtonPush);

	//全スロットを未使用状態に初期化しておく.
	InitMashBattle();
}

CButtonMashBattleUI::~CButtonMashBattleUI()
{
	//ゲージ画像・数字画像
	for (int i = GAUGE_MAX - 1; i >= 0; i--)
	{
		m_pGaugeImg[i] = nullptr;
	}
	m_pCountdownDigit = nullptr;
	m_pPlayerName = nullptr;
	m_pVsFont = nullptr;
	m_pButtonGuide = nullptr;
}

void CButtonMashBattleUI::Update()
{
	//全スロットを個別に更新する(最大BATTLE_SLOT_MAX組が同時進行できる).
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive) continue;
		UpdateSlot(Slot);
	}
}

//対決中の具材の画面上の位置を更新する.
void CButtonMashBattleUI::UpdateScreenPosition(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive) continue;
		if (!Slot.pTargetIngredients) continue;

		//具材の3Dワールド座標を、ゲージ描画に使う2D画面座標に変換しておく.
		Slot.ScreenPos = GetPosition2D(View, Proj, Slot.pTargetIngredients->GetPosition());
	}
}

void CButtonMashBattleUI::Draw()
{
	//対決中でなければ何も描画しない.
	if (!IsBattle()) return;

	//参加している可能性のある4人分のゲージ描画を呼び出す.
	RenderPlayer1Gauge();
	RenderPlayer2Gauge();
	RenderPlayer3Gauge();
	RenderPlayer4Gauge();

	//カウントダウン中のスロットは、参加者ごとに数字を出すと重複してしまうため、
	//スロットにつき1つだけ代表してゲージの中央に表示する.
	//上段に「プレイヤー壱 対 プレイヤー弐」、下段に数字を出し、カウントダウン終了と同時に両方消える.
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive || !Slot.IsCountdown) continue;
		RenderMatchupNames(Slot);
		RenderCountdownNumber(Slot);
	}

	//連打が始まったスロットには、Bボタンを連打することが分かるようにボタンを表示する.
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive || Slot.IsCountdown) continue;
		RenderButtonGuide(Slot);
	}
}

//連打対決の初期化.
void CButtonMashBattleUI::InitMashBattle()
{
	for (auto& Slot : m_Slots)
	{
		ResetSlot(Slot);
	}
}

//新しく対決を開始できる空きスロットがあるかどうか.
bool CButtonMashBattleUI::CanStartBattle() const
{
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive) return true;
	}
	return false;
}

//いずれかの対決が進行中かどうか.
bool CButtonMashBattleUI::IsBattle() const
{
	for (auto& Slot : m_Slots)
	{
		if (Slot.IsActive) return true;
	}
	return false;
}

//連打対決を開始する.
void CButtonMashBattleUI::StartBattle(CIngredients* pTarget, CPlayer* const pContenders[], int EntryCount)
{
	//対象具材が無い、または参加者が1人以下なら対決にならないので開始しない.
	if (!pTarget || EntryCount < 2) return;

	//空いているスロットを探す.
	BattleSlot* pSlot = nullptr;
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive)
		{
			pSlot = &Slot;
			break;
		}
	}
	if (!pSlot) return;

	//スロットを一旦まっさらにしてから開始する.
	ResetSlot(*pSlot);

	pSlot->pTargetIngredients = pTarget;
	pSlot->IsActive = true;
	pSlot->IsCountdown = true;					//いきなり連打を受け付けず、まずカウントダウンから始める.
	pSlot->CountdownTimer = COUNTDOWN_TOTAL_FRAMES;

	//参加者をプレイヤー番号の枠にそのまま登録し、決着がつくまで行動をロックする.
	for (int i = 0; i < EntryCount && i < GAUGE_MAX; i++)
	{
		CPlayer* pPlayer = pContenders[i];
		if (!pPlayer) continue;

		int No = pPlayer->GetPlayerNo();
		//プレイヤー番号が範囲外なら登録しない.
		if (No < 0 || No >= GAUGE_MAX) continue;

		pSlot->pEntries[No] = pPlayer;
		pPlayer->SetMashBattleLock(true);	//対決中は移動や新規の具材取得を禁止する.
	}
}

//1つのスロットの更新.
void CButtonMashBattleUI::UpdateSlot(BattleSlot& Slot)
{
	//カウントダウン中は連打を数えず、カウントダウンの進行だけを行う.
	if (Slot.IsCountdown)
	{
		if (Slot.CountdownTimer > 0)
		{
			Slot.CountdownTimer--;
		}
		//カウントダウンが終わったら、ここから本編を開始する.
		if (Slot.CountdownTimer <= 0)
		{
			Slot.IsCountdown = false;
			Slot.BattleTimer = BATTLE_TIME_LIMIT;	//制限時間はカウントダウン終了後から数える.
			Slot.GoFlashTimer = GO_FLASH_FRAMES;	//「GO!」の光る演出を開始する.
		}
		return;	//カウントダウン中はここで終了.
	}

	//GO演出の残りフレームを減らす.
	if (Slot.GoFlashTimer > 0)
	{
		Slot.GoFlashTimer--;
	}

	//制限時間を減らす.
	if (Slot.BattleTimer > 0)
	{
		Slot.BattleTimer--;
	}

	//参加者それぞれのAボタン入力を確認し、連打回数を加算する.
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		//このプレイヤー番号が参加していなければ次へ.
		if (!Slot.pEntries[i]) continue;

		CXInput* pInput = Slot.pEntries[i]->GetXInput();
		if (!pInput) continue;

		//Bボタンが押された瞬間だけを1回としてカウントする.
		if (pInput->IsDown(CXInput::B, true))
		{
			Slot.MashCount[i]++;
		}
	}

	//--------------------------------------------------
	//	勝敗判定.
	//--------------------------------------------------
	int WinnerNo = -1;

	//目標連打数に到達したプレイヤーがいれば勝者.
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (!Slot.pEntries[i]) continue;
		if (Slot.MashCount[i] >= MASH_GOAL)
		{
			WinnerNo = i;
			break;
		}
	}

	//誰も目標に到達しないまま制限時間が切れたら、その時点で最も連打回数が多いプレイヤーを勝者に.
	if (WinnerNo == -1 && Slot.BattleTimer <= 0)
	{
		int BestCount = -1;
		for (int i = 0; i < GAUGE_MAX; i++)
		{
			if (!Slot.pEntries[i]) continue;
			if (Slot.MashCount[i] > BestCount)
			{
				BestCount = Slot.MashCount[i];
				WinnerNo = i;
			}
		}
	}

	//勝者が決まっていればこの対決を終了させる.
	if (WinnerNo != -1)
	{
		EndSlot(Slot, WinnerNo);
	}
}

void CButtonMashBattleUI::EndSlot(BattleSlot& Slot, int WinnerNo)
{
	//参加者全員の行動ロックを解除する.
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (!Slot.pEntries[i]) continue;
		Slot.pEntries[i]->SetMashBattleLock(false);
	}

	//称号の判定用に、勝者は勝ち、それ以外の参加者は全員負けとして数える.
	if (WinnerNo >= 0 && WinnerNo < GAUGE_MAX && Slot.pEntries[WinnerNo])
	{
		for (int i = 0; i < GAUGE_MAX; i++)
		{
			if (!Slot.pEntries[i]) continue;
			if (i == WinnerNo)	Slot.pEntries[i]->AddMashWin();
			else				Slot.pEntries[i]->AddMashLose();
		}
	}

	//勝者がいれば、通常の具材獲得処理と同じ流れに合流させて具材を渡す.
	if (WinnerNo >= 0 && WinnerNo < GAUGE_MAX && Slot.pEntries[WinnerNo] && Slot.pTargetIngredients)
	{
		Slot.pEntries[WinnerNo]->IngredientsGetter(Slot.pTargetIngredients);
	}

	//このスロットを次の対決に備えて空ける.
	ResetSlot(Slot);
}

//スロットの内容初期化.
void CButtonMashBattleUI::ResetSlot(BattleSlot& Slot)
{
	Slot.IsActive = false;
	Slot.IsCountdown = false;
	Slot.CountdownTimer = 0;
	Slot.BattleTimer = 0;
	Slot.GoFlashTimer = 0;
	Slot.pTargetIngredients = nullptr;

	for (int i = 0; i < GAUGE_MAX; i++)
	{
		Slot.pEntries[i] = nullptr;
		Slot.MashCount[i] = 0;
	}
}

//指定したプレイヤーが今どのスロットで対決中か調べる.
int CButtonMashBattleUI::FindSlotIndexForPlayer(int PlayerNo) const
{
	for (int s = 0; s < BATTLE_SLOT_MAX; s++)
	{
		if (!m_Slots[s].IsActive) continue;
		if (m_Slots[s].pEntries[PlayerNo])
		{
			return s;
		}
	}
	return -1;
}

//プレイヤー壱のゲージの描画.
void CButtonMashBattleUI::RenderPlayer1Gauge()
{
	RenderGauge(0);
}
//プレイヤー弐のゲージの描画.
void CButtonMashBattleUI::RenderPlayer2Gauge()
{
	RenderGauge(1);
}
//プレイヤー参のゲージの描画.
void CButtonMashBattleUI::RenderPlayer3Gauge()
{
	RenderGauge(2);
}
//プレイヤー肆のゲージの描画.
void CButtonMashBattleUI::RenderPlayer4Gauge()
{
	RenderGauge(3);
}

//指定したプレイヤー番号のゲージを描画する.
void CButtonMashBattleUI::RenderGauge(int PlayerNo)
{
	//このプレイヤーが今対決中かどうか、対決中ならどのスロットかを調べる.
	int SlotIndex = FindSlotIndexForPlayer(PlayerNo);
	if (SlotIndex < 0) return;	//対決に参加していなければ描画しない.
	const BattleSlot& Slot = m_Slots[SlotIndex];

	//カウントダウン中はゲージをまだ表示しない.
	if (Slot.IsCountdown) return;

	if (!m_pGaugeImg[PlayerNo]) return;

	//対象の具材の真上に、参加人数分だけ縦に積み重ねて表示する.
	const int LocalRank = GetLocalRank(Slot, PlayerNo);
	const float PosX = Slot.ScreenPos.x - GAUGE_WIDTH * 0.5f;
	const float PosY = Slot.ScreenPos.y - GAUGE_ANCHOR_OFFSET_Y - LocalRank * (GAUGE_HEIGHT + GAUGE_MARGIN);

	//連打の進捗を0.0〜1.0の割合に変換する.
	float Rate = static_cast<float>(Slot.MashCount[PlayerNo]) / static_cast<float>(MASH_GOAL);
	if (Rate > 1.f) Rate = 1.f;
	if (Rate < 0.f) Rate = 0.f;

	//ゲージ画像は1列4行構成で、行番号(y)を変えることでプレイヤーごとの色を選べる.
	m_pGaugeImg[PlayerNo]->SetPatternNo(0, static_cast<SHORT>(PlayerNo));
	m_pGaugeImg[PlayerNo]->SetPosition(D3DXVECTOR3(PosX, PosY, 0.f));

	//ゲージの最大量を薄く表示する.
	m_pGaugeImg[PlayerNo]->SetScale(D3DXVECTOR3(GAUGE_WIDTH, GAUGE_HEIGHT, 0.f));
	m_pGaugeImg[PlayerNo]->SetAlpha(GAUGE_BG_ALPHA);
	m_pGaugeImg[PlayerNo]->Render();

	//連打した分だけ左詰めで塗りつぶす.
	m_pGaugeImg[PlayerNo]->SetScale(D3DXVECTOR3(GAUGE_WIDTH * Rate, GAUGE_HEIGHT, 0.f));
	m_pGaugeImg[PlayerNo]->SetAlpha(1.f);
	m_pGaugeImg[PlayerNo]->Render();

	//カウントダウン終了直後だけ、ゲージ全体をフラッシュさせてGOを演出する.
	if (Slot.GoFlashTimer > 0)
	{
		float FlashAlpha = static_cast<float>(Slot.GoFlashTimer) / static_cast<float>(GO_FLASH_FRAMES);
		m_pGaugeImg[PlayerNo]->SetScale(D3DXVECTOR3(GAUGE_WIDTH, GAUGE_HEIGHT, 0.f));
		m_pGaugeImg[PlayerNo]->SetAlpha(FlashAlpha);
		m_pGaugeImg[PlayerNo]->Render();
	}
}

//カウントダウン中の数字を描画する.
void CButtonMashBattleUI::RenderCountdownNumber(const BattleSlot& Slot)
{
	if (!m_pCountdownDigit) return;

	//カウントダウン開始からの経過フレーム数を求め、何段階目かを算出する.
	int Elapsed = COUNTDOWN_TOTAL_FRAMES - Slot.CountdownTimer;
	if (Elapsed < 0) Elapsed = 0;

	int Step = Elapsed / COUNTDOWN_STEP_FRAMES;	
	int Digit = 3 - Step;
	if (Digit > 3) Digit = 3;
	if (Digit < 1) Digit = 1;

	//対戦者名の下段に、ゲージの高さに合わせた正方形で大きく表示する.
	const float PosY = GetCountdownAreaTop(Slot) + TOP_ROW_HEIGHT + ROW_GAP;
	m_pCountdownDigit->SetPatternNo(static_cast<SHORT>(Digit), 0);
	m_pCountdownDigit->SetPosition(D3DXVECTOR3(Slot.ScreenPos.x - DIGIT_SIZE * 0.5f, PosY, 0.f));
	m_pCountdownDigit->SetScale(D3DXVECTOR3(DIGIT_SIZE, DIGIT_SIZE, 0.f));
	m_pCountdownDigit->SetAlpha(1.f);
	m_pCountdownDigit->Render();
}

//カウントダウン中に、誰と誰の対決かを描画する.
void CButtonMashBattleUI::RenderMatchupNames(const BattleSlot& Slot)
{
	if (!m_pPlayerName || !m_pVsFont) return;

	//このスロットの参加人数を数える.
	int EntryCount = 0;
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (Slot.pEntries[i]) EntryCount++;
	}
	if (EntryCount <= 0) return;

	//「名前 対 名前 対 …」を横一列に並べたときの全体の幅を求め、具材の真上を中心にする.
	const float VsStep = NAME_SPACING + VS_WIDTH + NAME_SPACING;
	const float TotalWidth = EntryCount * NAME_WIDTH + (EntryCount - 1) * VsStep;
	float PosX = Slot.ScreenPos.x - TotalWidth * 0.5f;

	//画面端の具材でも名前が見切れないように、画面内に収める.
	if (PosX + TotalWidth > static_cast<float>(WND_W)) PosX = static_cast<float>(WND_W) - TotalWidth;
	if (PosX < 0.f) PosX = 0.f;

	//上段の中で、名前と「対」をそれぞれ縦方向の中央にそろえる.
	const float RowTop = GetCountdownAreaTop(Slot);
	const float NamePosY = RowTop + (TOP_ROW_HEIGHT - NAME_HEIGHT) * 0.5f;
	const float VsPosY = RowTop + (TOP_ROW_HEIGHT - VS_HEIGHT) * 0.5f;

	int Drawn = 0;
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (!Slot.pEntries[i]) continue;

		//2人目以降は、名前の前に「対」を挟む.
		if (Drawn > 0)
		{
			//画像は1列2行構成(上段:対 下段:VS)で、上段の「対」だけを使う.
			m_pVsFont->SetPatternNo(0, 0);
			m_pVsFont->SetPosition(D3DXVECTOR3(PosX + NAME_SPACING, VsPosY, 0.f));
			m_pVsFont->SetScale(D3DXVECTOR3(VS_WIDTH, VS_HEIGHT, 0.f));
			m_pVsFont->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));
			m_pVsFont->SetAlpha(1.f);
			m_pVsFont->Render();
			PosX += VsStep;
		}

		//プレイヤーフォント画像は1列4行構成で、行番号(y)がプレイヤー番号に対応する.
		//(リザルト画面で回転させて使っているため、回転は毎回リセットしておく)
		m_pPlayerName->SetPatternNo(0, static_cast<SHORT>(i));
		m_pPlayerName->SetPosition(D3DXVECTOR3(PosX, NamePosY, 0.f));
		m_pPlayerName->SetScale(D3DXVECTOR3(NAME_WIDTH, NAME_HEIGHT, 0.f));
		m_pPlayerName->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_pPlayerName->SetAlpha(1.f);
		m_pPlayerName->Render();

		PosX += NAME_WIDTH;
		Drawn++;
	}
}

//連打中に、ゲージの横へボタン表示を描画する.
void CButtonMashBattleUI::RenderButtonGuide(const BattleSlot& Slot)
{
	if (!m_pButtonGuide) return;

	//ゲージが並ぶ範囲の縦の中心を求める(カウントダウン表示の範囲と中心は同じ).
	const float AreaHeight = TOP_ROW_HEIGHT + ROW_GAP + DIGIT_SIZE;
	const float CenterY = GetCountdownAreaTop(Slot) + AreaHeight * 0.5f;

	//ボタン画像は中央基準なので、ゲージの右端からボタンの半分だけずらした位置を中心にする.
	const float CenterX = Slot.ScreenPos.x + GAUGE_WIDTH * 0.5f + BUTTON_OFFSET_X + BUTTON_WIDTH * 0.5f;

	//マヒ中の表示と同じく、ボタン画像は1列3行構成で、
	//0行目(押す前)と2行目(押した瞬間)を交互に切り替えて連打している様子を見せる.
	const int Elapsed = BATTLE_TIME_LIMIT - Slot.BattleTimer;
	const SHORT PatternY = ((Elapsed / BUTTON_ANIM_FRAMES) % 2 == 0) ? 0 : 2;

	m_pButtonGuide->SetPatternNo(0, PatternY);
	m_pButtonGuide->SetPosition(D3DXVECTOR3(CenterX, CenterY, 0.f));
	m_pButtonGuide->SetScale(D3DXVECTOR3(BUTTON_WIDTH, BUTTON_HEIGHT, 1.f));
	m_pButtonGuide->SetRotation(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pButtonGuide->SetAlpha(1.f);
	m_pButtonGuide->Render();
}

//カウントダウン表示(上段:対戦者名 下段:数字)をまとめた範囲の上端を求める.
float CButtonMashBattleUI::GetCountdownAreaTop(const BattleSlot& Slot) const
{
	//このスロットの参加人数を数える.
	int EntryCount = 0;
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (Slot.pEntries[i]) EntryCount++;
	}
	if (EntryCount <= 0) EntryCount = 1;

	//本編開始後に参加者のゲージが並ぶ範囲を求め、
	const float RowStep = GAUGE_HEIGHT + GAUGE_MARGIN;
	const float NearestRowTop = Slot.ScreenPos.y - GAUGE_ANCHOR_OFFSET_Y;					//具材に一番近い段の上端.
	const float FarthestRowTop = NearestRowTop - (EntryCount - 1) * RowStep;				//一番遠い段の上端.
	const float StackCenterY = (FarthestRowTop + NearestRowTop + GAUGE_HEIGHT) * 0.5f;		//全段をまとめた範囲の中心.

	//その中心に、上段と下段をまとめた範囲の中心が来るようにする.
	const float AreaHeight = TOP_ROW_HEIGHT + ROW_GAP + DIGIT_SIZE;
	return StackCenterY - AreaHeight * 0.5f;
}

//同じスロット内で、指定したプレイヤーが何番目の参加者かを求める.
int CButtonMashBattleUI::GetLocalRank(const BattleSlot& Slot, int PlayerNo) const
{
	int Rank = 0;
	for (int i = 0; i < PlayerNo; i++)
	{
		if (Slot.pEntries[i]) Rank++;
	}
	return Rank;
}
