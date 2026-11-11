#include "CTitleChopsticks.h"

namespace
{
	//お箸1本ぶんの設定(データ)
	struct ChopstickDef
	{
		float signX, signY;				//出発する角(X: +右 / -左、Y: +上 / -下).
		int   pairIndex;				//一緒に動く組(0:右上と左下、1:左上と右下).
		float colorR, colorG, colorB;	//お箸の色(0〜255).
	};
	//お箸の色.
	constexpr ChopstickDef CHOPSTICK_DEFS[] = {
		//signX  signY  pair  R      G      B
		{  1.f,   1.f,  0,    255.f, 0.f,   0.f		},	//[0]右上・赤.
		{ -1.f,   1.f,  1,    255.f, 255.f, 0.f		},	//[1]左上・黄.
		{  1.f,  -1.f,  1,    0.f,   255.f, 0.f		},	//[2]右下・緑.
		{ -1.f,  -1.f,  0,    0.f,   0.f,   255.f	},	//[3]左下・青.
	};

	//お箸の見た目
	constexpr float CHOPSTICKS_SCALE = 2.0f;	//お箸メッシュの拡大率.

	//動き(X方向の移動量で表す。Y方向は BasePos の縦横比から自動で決まる)
	constexpr float APPROACH_SPEED		= 0.18f;			//中心へ向かう速さ(ワールド単位/フレーム).
	constexpr float RETURN_SPEED_RATE	= 1.f / 1.68f;		//衝突後に戻るときの速さの倍率(ゆっくり戻る).
	constexpr float CONTACT_GAP_X		= 0.88f;			//ぶつかる瞬間の、中心から各お箸の原点までのX方向の隙間(元の停止位置 2.5 - 1.62 に合わせた値. 見た目で調整).
	constexpr float RETURN_LIMIT		= -3.0f;			//ここまで戻ったら次の組へ(出発位置より外側).
	constexpr int   HIT_STOP_FRAME		= 40;				//衝突後に止まるフレーム数(60FPSで約0.7秒).

	//---- 演出 ----
	constexpr float CAMERA_ZOOM_DEPTH	= 3.0f;			//最終段階でカメラを寄せる距離.
	constexpr float LIGHTNING_POS_X		= 0.9f;			//雷エフェクトのX位置.
	constexpr float LIGHTNING_POS_Y		= 0.4f;			//雷エフェクトのY位置.
	constexpr float LIGHTNING_POS_Z		= 15.4f;		//雷エフェクトのZ位置.
	constexpr float LIGHTNING_SCALE		= 0.1f;			//雷エフェクトの大きさ.

	float Clamp01(float value)
	{
		if (value < 0.f) {
			return 0.f;		//0より小さければ0にする.
		}
		else if (value > 1.f) {
			return 1.f;		//1より大きければ1にする.
		}
		else {
			return value;	//範囲内ならそのまま.
		}
	}
}

CTitleChopsticks::CTitleChopsticks()
{
	for (int i = 0; i < Chopsticks_Max; i++) {
		const ChopstickDef& def = CHOPSTICK_DEFS[i];

		m_pChopsticks[i] = std::make_unique<CStaticMeshObjObject>();
		m_pChopsticks[i]->AttachMesh(*CSpriteManager::GetObjMesh(CSpriteManager::enMeshObjList::Chopsticks2));	//お箸(メッシュ).
		m_pChopsticks[i]->SetScale(CHOPSTICKS_SCALE, CHOPSTICKS_SCALE, CHOPSTICKS_SCALE);

		//向きは右上を基準に、角の位置に合わせて符号を変える(上下でX、左右でY、斜めの組でZが反転).
		m_pChopsticks[i]->SetRotation(
			def.signY * m_BaseRot.x,
			def.signX * m_BaseRot.y,
			def.signX * def.signY * m_BaseRot.z);
	}
	SetPhase(Phase::FirstPair);	//最初の組から始める.
}

CTitleChopsticks::~CTitleChopsticks() = default;

void CTitleChopsticks::Update()
{
	//Aボタンでシーンスキップ.
	if (m_pController[0]->IsConnect() && m_pController[0]->IsDown(CXInput::A, false)) {
		m_SceneChangeflag = true;
	}
	//衝突後の静止中は動かさずに待つ.
	if (m_StopTimer > 0) {
		UpdateHitStop();
		return;
	}
	UpdateMove();
}

//衝突後の静止時間を数え、終わったら来た道を戻り始める.
void CTitleChopsticks::UpdateHitStop()
{
	if (--m_StopTimer > 0) return;	//まだ止まっている.

	m_MoveSpeed = -m_MoveSpeed;		//向きを反転して戻り始める.
	if (m_Phase != Phase::Final) {
		m_MoveSpeed *= RETURN_SPEED_RATE;	//最終段階以外は、ゆっくり戻る.
	}
}

//お箸を動かし、衝突と次の組への切り替えを判定する.
void CTitleChopsticks::UpdateMove()
{
	m_MoveDist += m_MoveSpeed;	//先に動かしてから、位置に反映する(1フレーム遅れを防ぐ).

	//中心へ向かう.
	if (m_AttackMoveNow) {
		//中心へ向かっている途中.
		if (m_MoveDist >= GetImpactDist()) {
			m_MoveDist = GetImpactDist();	//行き過ぎた分を切り捨て、毎回同じ位置でぶつける.
			OnImpact();	//ぶつかった瞬間の処理.
		}
	}
	//最後のシーンじゃないかつ、一定値まで戻ったら次へ.
	else if (m_Phase != Phase::Final && m_MoveDist < RETURN_LIMIT) {
		//十分に戻ったので、次の組へ.
		if (m_Phase == Phase::FirstPair) {
			SetPhase(Phase::SecondPair);	//1組目の次は2組目.
		}
		else {
			SetPhase(Phase::Final);			//2組目の次は最終段階.
		}	
		return;	
	}
	ApplyPositions();
}

//ぶつかった瞬間の処理.
void CTitleChopsticks::OnImpact()
{
	m_AttackMoveNow = false;		//前進完了.
	m_StopTimer = HIT_STOP_FRAME;	//しばらく止める.
	PlayHitEffect();

	//最終段階はシーンチェンジへ(SEは鳴らさない).
	if (m_Phase == Phase::Final) {
		m_SceneChangeflag = true;
		return;
	}
	XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::ChopsticksHit);
	XAudio2::GetInstance()->SEPlayHandle(XAudio2::SEList::Volt);
}

//雷のエフェクトを出す.
void CTitleChopsticks::PlayHitEffect()
{
	const ::EsHandle handle = Effect::Play(EFE::lightning, { LIGHTNING_POS_X, LIGHTNING_POS_Y, LIGHTNING_POS_Z });
	Effect::SetScale(handle, D3DXVECTOR3(LIGHTNING_SCALE, LIGHTNING_SCALE, LIGHTNING_SCALE));
}

//今の段階で動かすお箸か.
bool CTitleChopsticks::IsActive(int index) const
{
	if (m_Phase == Phase::Final) return true;	//最終段階は4本とも動く.
	return CHOPSTICK_DEFS[index].pairIndex == static_cast<int>(m_Phase);	//FirstPair=0、SecondPair=1.
}

//ぶつかる位置までの移動量(X方向).
float CTitleChopsticks::GetImpactDist() const
{
	return m_BasePos.x - CONTACT_GAP_X;	//BasePos から求めるので、設定値どうしが食い違わない.
}

//移動量から、お箸の位置を決める.
void CTitleChopsticks::ApplyPositions()
{
	//XもYも同じ割合で縮めるので、必ず中心へ向かう.
	const float toCenter = 1.f - m_MoveDist / m_BasePos.x;
	const float x = m_BasePos.x * toCenter;
	const float y = m_BasePos.y * toCenter;

	for (int i = 0; i < Chopsticks_Max; i++) {
		if (!IsActive(i)) continue;
		m_pChopsticks[i]->SetPosition(CHOPSTICK_DEFS[i].signX * x, CHOPSTICK_DEFS[i].signY * y, m_BasePos.z);
	}
}

void CTitleChopsticks::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//最終段階だけ、お箸が中心に近づくほどカメラを寄せる.
	D3DXMATRIX view = View;
	if (m_Phase == Phase::Final) {
		const float zoomRate = Clamp01(m_MoveDist / GetImpactDist());	//中心へ向かう進み具合(0〜1).
		D3DXMATRIX zoom;
		D3DXMatrixTranslation(&zoom, 0.f, 0.f, -zoomRate * CAMERA_ZOOM_DEPTH);	//マイナスで手前へ引き寄せる.
		D3DXMatrixMultiply(&view, &View, &zoom);
	}

	for (int i = 0; i < Chopsticks_Max; i++) {
		if (!IsActive(i)) continue;

		const ChopstickDef& def = CHOPSTICK_DEFS[i];
		//色の設定はメッシュ側にあるので、1本ずつ描画の直前に設定する.
		m_pChopsticks[i]->GetMesh()->SetLightCOLOR256({ def.colorR, def.colorG, def.colorB });
		m_pChopsticks[i]->GetMesh()->SetisCOLOR(true);
		m_pChopsticks[i]->Draw(view, Proj, Light, Camera);
	}
}

//段階を切り替え、初期状態に戻す.
void CTitleChopsticks::SetPhase(Phase next)
{
	m_Phase = next;
	m_AttackMoveNow = true;			//中心へ向かう状態から始める.
	m_StopTimer = 0;
	m_MoveDist = 0.f;				//出発位置から.
	m_MoveSpeed = APPROACH_SPEED;
	ApplyPositions();				//切り替えた直後の描画でも、正しい位置にいるようにする.
}