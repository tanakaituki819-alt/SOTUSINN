#include "CButtonMashBattleUI.h"
#include "SpriteManager/CSpriteManager.h"

namespace {
	//---------------------------------------------------
	//	ゲージの見た目に関する定数.
	//---------------------------------------------------
	constexpr float GAUGE_WIDTH   = 150.f;	//ゲージ1本分の表示幅(ピクセル).
	constexpr float GAUGE_HEIGHT  = 36.f;	//ゲージ1本分の表示高さ(ピクセル).
	constexpr float GAUGE_MARGIN  = 8.f;	//ゲージ同士の縦の間隔(ピクセル).
	constexpr float GAUGE_ANCHOR_OFFSET_Y = 70.f;	//具材の画面上の位置から、どれだけ上にゲージをずらすか(ピクセル).
	constexpr float GAUGE_BG_ALPHA = 0.3f;	//連打していない部分(土台)のα値.
}

CButtonMashBattleUI::CButtonMashBattleUI()
{
	//ゲージ画像の取得.
	//※CSpriteManagerはNo単位でインスタンスをキャッシュしているため、
	//  4つとも実体は同じCSprite2Dだが、描画のたびにパターンや座標を
	//  設定し直してから即座にRenderするので問題なく使い分けられる.
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		m_pGaugeImg[i] = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::IMG_Gage);
	}
	//カウントダウン表示用(スコア表示等でも使っている0〜9の数字画像を流用する).
	m_pCountdownDigit = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Digit0_9);

	//全スロットを未使用状態に初期化しておく.
	InitMashBattle();
}

CButtonMashBattleUI::~CButtonMashBattleUI()
{
	//ゲージ画像・数字画像
	//※CSpriteManagerが実体を管理しているため、ここではnullptrに戻すだけでよい.
	for (int i = GAUGE_MAX - 1; i >= 0; i--)
	{
		m_pGaugeImg[i] = nullptr;
	}
	m_pCountdownDigit = nullptr;
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
	//対決中(カウントダウン含む)でなければ何も描画しない.
	if (!IsBattle()) return;

	//参加している可能性のある4人分のゲージ描画を呼び出す.
	//(各関数の内部で不参加、またはカウントダウン中なら何もしない).
	RenderPlayer1Gauge();
	RenderPlayer2Gauge();
	RenderPlayer3Gauge();
	RenderPlayer4Gauge();

	//カウントダウン中のスロットは、参加者ごとに数字を出すと重複してしまうため、
	//スロットにつき1つだけ代表してゲージの中央に表示する.
	for (auto& Slot : m_Slots)
	{
		if (!Slot.IsActive || !Slot.IsCountdown) continue;
		RenderCountdownNumber(Slot);
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

//いずれかの対決が進行中(カウントダウン含む)かどうか.
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

	//空いているスロットを探す(両方使用中なら今回は開始できない).
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
	//(カウントダウン中も含めてロックすることで、対決相手が確定した瞬間から動けなくする).
	for (int i = 0; i < EntryCount && i < GAUGE_MAX; i++)
	{
		CPlayer* pPlayer = pContenders[i];
		if (!pPlayer) continue;

		int No = pPlayer->GetPlayerNo();
		//プレイヤー番号が範囲外(想定外の値)なら登録しない.
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
		//カウントダウンが終わったら、ここから本編(連打受付)を開始する.
		if (Slot.CountdownTimer <= 0)
		{
			Slot.IsCountdown = false;
			Slot.BattleTimer = BATTLE_TIME_LIMIT;	//制限時間はカウントダウン終了後から数える.
			Slot.GoFlashTimer = GO_FLASH_FRAMES;	//「GO!」の光る演出を開始する.
		}
		return;	//カウントダウン中はここで終了(連打判定はしない).
	}

	//GO演出の残りフレームを減らす(見た目だけなので判定には影響しない).
	if (Slot.GoFlashTimer > 0)
	{
		Slot.GoFlashTimer--;
	}

	//制限時間を減らす(0未満にはしない).
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

		//Aボタンが「今まさに押された瞬間」だけを1回としてカウントする.
		//(押しっぱなしで連続カウントされないようにJust=trueで判定).
		if (pInput->IsDown(CXInput::A, true))
		{
			Slot.MashCount[i]++;
		}
	}

	//--------------------------------------------------
	//	勝敗判定.
	//--------------------------------------------------
	int WinnerNo = -1;

	//①：目標連打数に到達したプレイヤーがいれば即座に勝者とする.
	//    (プレイヤー番号が若い方から調べるので、同フレームで複数人到達しても番号が若い方が優先される).
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (!Slot.pEntries[i]) continue;
		if (Slot.MashCount[i] >= MASH_GOAL)
		{
			WinnerNo = i;
			break;
		}
	}

	//②：誰も目標に到達しないまま制限時間が切れたら、その時点で最も連打回数が多いプレイヤーを勝者にする.
	//    (同数の場合はプレイヤー番号が若い方を優先する).
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

	//勝者が決まっていればこのスロットの対決を終了させる.
	if (WinnerNo != -1)
	{
		EndSlot(Slot, WinnerNo);
	}
}

//決着をつけて後片付けをする.
void CButtonMashBattleUI::EndSlot(BattleSlot& Slot, int WinnerNo)
{
	//参加者全員の行動ロックを解除する.
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (!Slot.pEntries[i]) continue;
		Slot.pEntries[i]->SetMashBattleLock(false);
	}

	//勝者がいれば、通常の具材獲得処理と同じ流れに合流させて具材を渡す.
	//(これ以降は普段の「回収中」の処理でお箸が持ち上がっていく).
	if (WinnerNo >= 0 && WinnerNo < GAUGE_MAX && Slot.pEntries[WinnerNo] && Slot.pTargetIngredients)
	{
		Slot.pEntries[WinnerNo]->IngredientsGetter(Slot.pTargetIngredients);
	}

	//このスロットを次の対決に備えて空ける.
	ResetSlot(Slot);
}

//スロットの内容を未使用状態に戻す.
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

//指定したプレイヤー番号のゲージ(またはカウントダウン数字)を描画する.
void CButtonMashBattleUI::RenderGauge(int PlayerNo)
{
	//このプレイヤーが今対決中かどうか、対決中ならどのスロットかを調べる.
	int SlotIndex = FindSlotIndexForPlayer(PlayerNo);
	if (SlotIndex < 0) return;	//対決に参加していなければ描画しない.
	const BattleSlot& Slot = m_Slots[SlotIndex];

	//カウントダウン中はゲージをまだ表示しない(代表の数字をDraw()側でスロットにつき1回だけ表示する).
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

	//ゲージ画像は1列4行構成で、行番号(y)を変えることでプレイヤーごとの色(赤・青・黄・緑)を選べる.
	m_pGaugeImg[PlayerNo]->SetPatternNo(0, static_cast<SHORT>(PlayerNo));
	m_pGaugeImg[PlayerNo]->SetPosition(D3DXVECTOR3(PosX, PosY, 0.f));

	//①土台(ゲージの最大量)を薄く表示する.
	//IMG_GageはTopleftreferenceis(左上基準)が既定でONになっているため、
	//SetPositionで指定した座標が左上端の基準になる.
	m_pGaugeImg[PlayerNo]->SetScale(D3DXVECTOR3(GAUGE_WIDTH, GAUGE_HEIGHT, 0.f));
	m_pGaugeImg[PlayerNo]->SetAlpha(GAUGE_BG_ALPHA);
	m_pGaugeImg[PlayerNo]->Render();

	//②連打した分だけ左詰めで塗りつぶす.
	//左上基準のため、幅(Scale.x)だけを進捗率倍に縮めれば左端の位置は変わらず、
	//右端だけが連打量に応じて伸び縮みするゲージ表現になる.
	m_pGaugeImg[PlayerNo]->SetScale(D3DXVECTOR3(GAUGE_WIDTH * Rate, GAUGE_HEIGHT, 0.f));
	m_pGaugeImg[PlayerNo]->SetAlpha(1.f);
	m_pGaugeImg[PlayerNo]->Render();

	//③カウントダウン終了直後だけ、ゲージ全体をフラッシュさせて「GO!」を演出する.
	//(判定には一切影響しない、見た目だけの効果).
	if (Slot.GoFlashTimer > 0)
	{
		float FlashAlpha = static_cast<float>(Slot.GoFlashTimer) / static_cast<float>(GO_FLASH_FRAMES);
		m_pGaugeImg[PlayerNo]->SetScale(D3DXVECTOR3(GAUGE_WIDTH, GAUGE_HEIGHT, 0.f));
		m_pGaugeImg[PlayerNo]->SetAlpha(FlashAlpha);
		m_pGaugeImg[PlayerNo]->Render();
	}
}

//カウントダウン中の数字(3・2・1)を、スロットにつき1つだけ代表して描画する.
void CButtonMashBattleUI::RenderCountdownNumber(const BattleSlot& Slot)
{
	if (!m_pCountdownDigit) return;

	//このスロットの参加人数を数える(ゲージが縦に何本並ぶ予定か).
	int EntryCount = 0;
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		if (Slot.pEntries[i]) EntryCount++;
	}
	if (EntryCount <= 0) return;

	//本編開始後に参加者のゲージが並ぶ範囲(1番具材側の段の上端〜一番上の段の上端+ゲージ1本分)を求め、
	//その中心に数字を1つだけ表示する.
	const float RowStep = GAUGE_HEIGHT + GAUGE_MARGIN;
	const float NearestRowTop = Slot.ScreenPos.y - GAUGE_ANCHOR_OFFSET_Y;					//具材に一番近い段の上端.
	const float FarthestRowTop = NearestRowTop - (EntryCount - 1) * RowStep;				//一番遠い(上の)段の上端.
	const float StackCenterY = (FarthestRowTop + NearestRowTop + GAUGE_HEIGHT) * 0.5f;	//全段をまとめた範囲の中心.

	//カウントダウン開始からの経過フレーム数を求め、何段階目("3"→"2"→"1")かを算出する.
	int Elapsed = COUNTDOWN_TOTAL_FRAMES - Slot.CountdownTimer;
	if (Elapsed < 0) Elapsed = 0;

	int Step = Elapsed / COUNTDOWN_STEP_FRAMES;	//0:"3"の期間, 1:"2"の期間, 2:"1"の期間.
	int Digit = 3 - Step;
	if (Digit > 3) Digit = 3;
	if (Digit < 1) Digit = 1;

	//ゲージが並ぶ範囲の中央に、ゲージの高さに合わせた正方形で大きく表示する.
	const float Size = GAUGE_HEIGHT;
	m_pCountdownDigit->SetPatternNo(static_cast<SHORT>(Digit), 0);
	m_pCountdownDigit->SetPosition(D3DXVECTOR3(Slot.ScreenPos.x - Size * 0.5f, StackCenterY - Size * 0.5f, 0.f));
	m_pCountdownDigit->SetScale(D3DXVECTOR3(Size, Size, 0.f));
	m_pCountdownDigit->SetAlpha(1.f);
	m_pCountdownDigit->Render();
}

//同じスロット内で、指定したプレイヤーが何番目(0始まり)の参加者かを求める.
int CButtonMashBattleUI::GetLocalRank(const BattleSlot& Slot, int PlayerNo) const
{
	int Rank = 0;
	for (int i = 0; i < PlayerNo; i++)
	{
		if (Slot.pEntries[i]) Rank++;
	}
	return Rank;
}
