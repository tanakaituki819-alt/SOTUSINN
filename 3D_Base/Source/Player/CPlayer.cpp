#include "CPlayer.h"
#include <algorithm>

namespace {
	//回収(お箸を下ろす)
	constexpr int	LOWER_FRAME = 40;		//お箸を下ろし続ける時間(この間に具材をつかめなければ回収をやめる).
	constexpr float	LOWER_SPEED = 0.02f;	//お箸を下ろす速さ(1フレームあたり).

	//マヒ
	constexpr int	PARALYSIS_FRAME = 180;		//マヒが続く時間(60FPSで3秒).
	constexpr int	PARALYSIS_SKIP_FRAME = 10;		//Bボタン1回で短縮できる時間.
	constexpr float	SHAKE_SPEED = 0.8f;		//震える速さ(1フレームで進む角度).
	constexpr float	SHAKE_HEIGHT = 0.06f;	//震える幅.
}

CPlayer::CPlayer()
	: MyController()
{

	m_Rotation.y = D3DXToRadian(90);
	m_Cousor = CSpriteManager::GetSprite2D(CSpriteManager::enImagList::Img_Cusoru);
	Score = 0;
	P_UI = new CPlayerUI();
	PlayerNo = 0;
	//お箸最大数分.
	for (int i = 0; i < Chopsticks_Max; i++) {
		m_pChopsticks[i] = new CStaticMeshObjObject();
		m_pChopsticks[i]->AttachMesh(*CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::Chopsticks));	//お箸(メッシュ).
		m_pBSphere->CreatSphereForMesh(*CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::Chopsticks));	//スフィアのメッシュを設定.
		m_pBSphere->SetRadius(0.1f);		//半径を変更.
	}
	m_OldPosition = { 0,0,0 };
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(P_UI);
	//お箸最大数分.
	for (int i = 0; i < Chopsticks_Max; i++) {
		SAFE_DELETE(m_pChopsticks[i]);
	}
}

void CPlayer::Update()
{

	if (m_IsParalysis) {
		m_Status = 3;	//マヒ中.
	}
	else if (!m_IsParalysis && m_Status == 3) {
		m_Status = 1;	//デフォルト状態.
	}
	if (m_Status == 0) {
		++m_SmileTimer;
		if (m_SmileTimer > 90) {
			m_Status = 1;		//デフォルト状態.
			m_SmileTimer = 0;	//タイマー初期化. 
		}
	}
	//コントローラーが接続されていれば.
	if (MyController->IsConnect() == true) {
		m_IsConnected = true;	//コントローラー接続状態に.
		//回収中じゃないかつマヒ中でないかつ連打対決中でないなら
		if (!m_IsCollecting && !m_IsParalysis && !m_IsMashBattle) {
			//左スティック動作.
			D3DXVECTOR2 VECT = { static_cast<FLOAT>(MyController->GetLThumbX()) ,static_cast<FLOAT>(MyController->GetLThumbY()) };
			D3DXVec2Normalize(&VECT, &VECT);
			VECT *= Speed;

			m_Position.x += VECT.x;
			m_Position.z += VECT.y;
		}
		D3DXVECTOR3  calPos = { m_Position.x,0,m_OldPosition.z };
		D3DXVECTOR3  ENDPos = m_Position;
		float i = atan2(m_Position.z, m_Position.x);
		//鍋の外に出ないようにする
		if (!D2CollizionXZ(calPos, 0.1, { 0,0,0 }, 4)) {

			ENDPos.x = cos(i) * (4.0 - 0.1);

		};
		calPos = { m_OldPosition.x,0,m_Position.z };

		if (!D2CollizionXZ(calPos, 0.1, { 0,0,0 }, 4)) {
			ENDPos.z = sin(i) * (4.0 - 0.1);

		};
		m_Position = ENDPos;
		//回収中じゃないかつマヒ中でないかつ連打対決中でないなら.
		if (!m_IsCollecting && !m_IsParalysis && !m_IsMashBattle) {
			if (MyController->IsDown(CXInput::A, false)) {
				m_IsCollecting = true;	//回収中へ.
			}
		}
		//お箸を下ろす処理.
		UpdateLowerChopsticks();
		//マヒ中の処理.
		if (m_IsParalysis) {
			UpdateParalysis();
		}
		//回収中の処理.
		IngredientsCollecting();
		UpdateBSpherePos();	//当たり判定位置を更新.

		m_OldPosition = m_Position;
	}
	else {
		m_IsConnected = false;	//コントローラー接続状態解除.
	}
}

void CPlayer::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//回収状態じゃないかつマヒ状態じゃないなら.
	if (!m_IsCollecting && !m_IsParalysis) {
		m_Position.y = 1;
		D3DXVECTOR3 i = m_Position;
		i.y = 0.2 + 0.4;
		m_Cousor->SetPosition(GetPosition2D(View, Proj, m_Position));

		m_CousorPosition = GetPosition2D(View, Proj, i);	//カーソルのポジションを2D用のポジションに変換する.
	}
	//回収中.
	else {
		//お箸最大数分.
		for (int i = 0; i < Chopsticks_Max; i++) {
			//お箸1本目.
			if (i == 0) {
				m_pChopsticks[i]->SetPosition(m_Position.x + 0.5f, m_Position.y + 1.f, m_Position.z);
			}
			//2本目.
			else {
				//お箸の2本目を少しずらして表示する.
				m_pChopsticks[i]->SetPosition(m_Position.x + 0.5f, m_Position.y + 1.f, m_Position.z + 0.2f);
			}
			m_pChopsticks[i]->SetRotation(D3DXToRadian(90), D3DXToRadian(-45), D3DXToRadian(0));	//角度調整.
			m_pChopsticks[i]->SetScale(2.f, 2.f, 2.f);

			//プレイヤーの番号に応じて色を変える
			if (MyController->GetPadID() == 0) {
				m_pChopsticks[0]->GetMesh()->SetLightCOLOR256({ 255,0,0 });
				m_pChopsticks[0]->GetMesh()->SetisCOLOR(true);
			}
			if (MyController->GetPadID() == 1) {
				m_pChopsticks[0]->GetMesh()->SetLightCOLOR256({ 0,0,255 });
				m_pChopsticks[0]->GetMesh()->SetisCOLOR(true);
			}
			if (MyController->GetPadID() == 2) {
				m_pChopsticks[0]->GetMesh()->SetLightCOLOR256({ 255,255,0 });
				m_pChopsticks[0]->GetMesh()->SetisCOLOR(true);
			}
			if (MyController->GetPadID() == 3) {
				m_pChopsticks[0]->GetMesh()->SetLightCOLOR256({ 0,255,0 });
				m_pChopsticks[0]->GetMesh()->SetisCOLOR(true);
			}


			m_pChopsticks[i]->Draw(View, Proj, Light, Camera);
		}
	}
}

void CPlayer::DrawUI()
{
	//マヒ中なら、ボタン案内を表示する.
	if (m_IsParalysis) {
		m_ParalysisUI.Draw();
	}

	//m_Cousor->SetRotation({ D3DXToRadian(90),0,0 });
	m_Cousor->SetPosition(m_CousorPosition);
	m_Cousor->SetScale({ 85,85,85 });

	if (!m_IsCollecting && !m_IsParalysis) {
		//プレイヤーの番号に応じて色を変える
		if (MyController->GetPadID() == 0) {
			m_Cousor->SetCOLOR({ 1,0,0 });//
			m_Cousor->SetisCOLOR(true);
		}
		if (MyController->GetPadID() == 1) {
			m_Cousor->SetCOLOR({ 0,0,1 });
			m_Cousor->SetisCOLOR(true);
		}
		if (MyController->GetPadID() == 2) {
			m_Cousor->SetCOLOR({ 1,1,0 });
			m_Cousor->SetisCOLOR(true);
		}
		if (MyController->GetPadID() == 3) {
			m_Cousor->SetCOLOR({ 0,1,0 });
			m_Cousor->SetisCOLOR(true);
		}
		if (MyController->IsConnect() == true) {
			m_Cousor->Render();
		}
	}
	//コントローラーが接続されているのなら.
	if (MyController->IsConnect() == true) {
		P_UI->SetPlayerNo(PlayerNo);
		P_UI->Draw(Score, m_Status);
	}
}
//お箸を下ろす処理(具材をつかむまで、一定時間お箸を下げる).
void CPlayer::UpdateLowerChopsticks()
{
	if (m_pIngredients != nullptr) return;			//具材をつかんだら持ち上げる処理.
	if (!m_IsCollecting || m_IsMashBattle) return;	//回収中でない、または連打対決中なら何もしない.

	++m_Collectingtime;
	//一定時間下ろしても何もつかめなければ回収をやめる.
	if (m_Collectingtime > LOWER_FRAME) {
		m_IsCollecting = false;
		m_Collectingtime = 0;
		return;
	}
	m_Position.y -= LOWER_SPEED;	//お箸を下ろす.
}
//マヒ中の処理(Bボタンで時間短縮、震える動き、ボタン案内UI).
void CPlayer::UpdateParalysis()
{
	//Bボタンを押すたびに、マヒの時間を短くする.
	if (MyController->IsUp(CXInput::B)) {
		m_ParalysisTimer += PARALYSIS_SKIP_FRAME;
		XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::Buttonpush);	//ボタン押下音.
	}
	++m_ParalysisTimer;

	//時間が経てば、マヒを解除する.
	if (m_ParalysisTimer > PARALYSIS_FRAME) {
		EndParalysis();
		return;
	}

	//震える動き.
	const float shakeY = sinf(m_ParalysisTimer * SHAKE_SPEED) * SHAKE_HEIGHT;
	m_Position.y += shakeY - m_ParalysisShakeY;
	m_ParalysisShakeY = shakeY;

	//ボタンUIは、マヒする直前のカーソルの位置に出す.
	m_ParalysisUI.SetPlayerPosition({ m_CousorPosition.x, m_CousorPosition.y, 0.f });
	m_ParalysisUI.Update();
}
//マヒを解除する.
void CPlayer::EndParalysis()
{
	m_Position.y -= m_ParalysisShakeY;	//震えでずらしていた分を戻す.
	m_ParalysisShakeY = 0.f;
	m_ParalysisTimer = 0;
	m_IsParalysis = false;
}
//マヒ状態にするときに呼び出される.
void CPlayer::OnTouchRawIngredient()
{
	m_IsParalysis = true;	//マヒ状態にする.
}

void CPlayer::IngredientsGetter(CIngredients* Ingredients)
{
	if (m_pIngredients != nullptr) {
		m_pIngredients->m_IsCollectingEnd();
	}
	m_pIngredients = Ingredients;
	m_Position = { m_pIngredients->GetPosition() };

	m_IsCollecting = true;	//回収中へ.
	m_Collectingtime = 0;	//タイマーリセット.

}

void CPlayer::IngredientsCollecting()
{
	if (!m_pIngredients)return;
	if (m_Position.y < 2.f) {
		m_Position.y += 0.1f;
		m_pIngredients->SetPosition(m_Position);
	}
	else {
		//GetIngredients(m_pIngredients);
		m_IngredientsGetEffectManager->Stato(this, m_pIngredients, { P_UI->GetPosition().x + P_UI->GetUIW() / 2,P_UI->GetPosition().y + P_UI->GetUIH() / 2,0 });
		m_pIngredients->Setisappear(false);
		m_pIngredients = nullptr;
		m_IsCollecting = false;
		m_Collectingtime = 0;	//タイマーリセット.
		m_Status = 0;			//笑顔状態.
	}

}

void CPlayer::GetIngredients(CIngredients* YASAI)
{
	my_list.push_back(YASAI->GetIngredientsNo());
	Score += YASAI->GetScore();
	//スコアを上限で止める.
	if (Score > SCORE_MAX)
	{
		Score = SCORE_MAX;
	}
	//noneも肉として出てくるので記録しておく.
	if (YASAI->GetIngredientsNo() >= Ingredients::none && YASAI->GetIngredientsNo() < Ingredients::MAX)
	{
		int& IngredientScore = m_IngredientScore[static_cast<int>(YASAI->GetIngredientsNo())];
		IngredientScore += YASAI->GetScore();
		if (IngredientScore > SCORE_MAX) {
			IngredientScore = SCORE_MAX;
		}
	}
	YASAI->DeadCharStatus();
	//m_pIngredients = nullptr;

}