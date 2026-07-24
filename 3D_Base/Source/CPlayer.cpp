#include "CPlayer.h"

CPlayer::CPlayer()
	: MyController		()
{

	m_Rotation.y = D3DXToRadian(90);
	m_Cousor = CSpriteManager::GetSprite3D(CSpriteManager::enImagList::Img_Cusoru);
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
	//コントローラーが接続されていれば.
	if (MyController->IsConnect()==true) {
		m_IsConnected = true;	//コントローラー接続状態に.
		//回収中じゃないかつマヒ中でないなら
		if (!m_IsCollecting && !m_IsParalysis) {
			//左スティック動作.
			D3DXVECTOR2 VECT = { static_cast<FLOAT>(MyController->GetLThumbX()) ,static_cast<FLOAT>(MyController->GetLThumbY()) };
			D3DXVec2Normalize(&VECT, &VECT);
			VECT *= Speed;
			m_Position.x += VECT.x;
			m_Position.z += VECT.y;
		}
		//回収中じゃないかつマヒ中でないなら.
		if (!m_IsCollecting && !m_IsParalysis) {
			if (MyController->IsDown(CXInput::A, false)) {
				m_IsCollecting = true;	//回収中へ.
			}
		}
		//具材回収中なら.
		if ( m_IsCollecting ) {
			++m_Collectingtime;	//タイム増加.
			//タイムが指定した時間経過すれば.
			if (m_Collectingtime > 40) {
				m_IsCollecting = false;	//回収中解除.
				m_Collectingtime = 0;	//タイムを初期化.
			}
			//タイムが指定した時間を超えていないなら.
			else {
				m_Position.y -= 0.01f;	//お箸を下におろす.
			}
		}
		//マヒ中なら.
		if (m_IsParalysis) {
			Paralysis();	//マヒ動作.
			//マヒUIが生成されていなければ.
			if (!m_pPlayerParalysisUI) {
				m_pPlayerParalysisUI = new CPlayerParalysisUI();	//インスタンス作成.
				m_pPlayerParalysisUI->Paralysis(m_Position);		//プレイヤーのポジションを渡す.
			}
			m_pPlayerParalysisUI->Update();
		}
		else {
			//マヒUIが生成されていれば.
			if (m_pPlayerParalysisUI) {
				SAFE_DELETE(m_pPlayerParalysisUI);	//破棄.
			}
		}
		//スコア増加(仮).
		if (MyController->IsDown(CXInput::A, false)&& MyController->GetPadID() ==0 ) {
			Score++;
		}
		UpdateBSpherePos();	//当たり判定位置を更新.
	}
	else {
		m_IsConnected = false;	//コントローラー接続状態解除.
	}
}

void CPlayer::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//回収状態じゃないかつマヒ状態じゃないなら.
	if (!m_IsCollecting && !m_IsParalysis) {
		m_Position.y = 1 + MyController->GetPadID() * 0.1;	//重なって見えなくならないようにずらす
		m_Cousor->SetPosition(m_Position);					
		m_Cousor->SetRotation({ D3DXToRadian(90),0,0 });
		m_Cousor->SetScale(m_Scale);
		m_Cousor->Render(View, Proj);
	}
	//回収中.
	else {
		//お箸最大数分.
		for (int i = 0; i < Chopsticks_Max; i++) {
			//お箸1本目なら
			if (i == 0) {
				m_pChopsticks[i]->SetPosition(m_Position.x +0.5f,m_Position.y + 1.f ,m_Position.z);
			}
			else {
				//お箸の2本目を少しずらして表示する.
				m_pChopsticks[i]->SetPosition(m_Position.x+ 0.5f,m_Position.y + 1.f ,m_Position.z + 0.2f);
			}
			m_pChopsticks[i]->SetRotation(D3DXToRadian(90), D3DXToRadian(-45), D3DXToRadian(0));
			m_pChopsticks[i]->SetScale(2.f, 2.f, 2.f);
			m_pChopsticks[i]->Draw(View, Proj, Light, Camera);
		}
	}
	//マヒ中なら.
	if (m_IsParalysis) {
		//マヒUIが生成されていれば.
		if (m_pPlayerParalysisUI) {
			m_pPlayerParalysisUI->Draw(View, Proj);	//マヒUI描画
		}
	}
}

void CPlayer::DrawUI()
{
	P_UI->SetPlayerNo(PlayerNo);
	P_UI->Draw(Score);
}
//マヒ状態処理.
void CPlayer::Paralysis()
{
	m_ParalysisTimer++;			//タイマー増加.
	if (m_ParalysisTimer > 120) {
		m_IsParalysis = false;   //マヒ解除.
		m_ParalysisTimer = 0;    //タイマーリセット.
		return;	//以降通らなくていいのでリターン.
	}
	m_Position.y += sinf(m_ParalysisTimer * m_Speed) * m_Amount;	//マヒの動き.
}
//マヒ状態にするときに呼び出される.
void CPlayer::OnTouchRawIngredient()
{
	m_IsParalysis = true;	//マヒ状態にする.
}

void CPlayer::GetIngredients(CIngredients* YASAI)
{
	my_list.push_back(YASAI->GetIngredientsNo());
	Score += YASAI->GetScore();

}


