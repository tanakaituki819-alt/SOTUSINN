#include "CCollisionManager.h"
#include <iostream>

CCollisionManager::CCollisionManager()
	: m_pPlayer()
	, m_pIngredientsManager()
{

}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE(m_pIngredientsManager);
}

void CCollisionManager::Update()
{
	if (!m_pIngredientsManager) return;

	//具材リストを取得.
	std::vector<CIngredients*>Ingredients = m_pIngredientsManager->GetIngredients();

	//--------------------------------------------------------------------
	//	①生の具材に触れたプレイヤーをマヒさせる.
	//	マヒはプレイヤー単位で独立して発生するので、取り合い(連打対決)とは別に判定する.
	//--------------------------------------------------------------------
	for (int i = 0; i < Player_Max; i++) {
		if (!m_pPlayer[i]) continue;
		if (!m_pPlayer[i]->GetConnected()) continue;			//コントローラー未接続なら対象外.
		if (!m_pPlayer[i]->GetIsCollecting()) continue;		//回収動作中でないなら対象外.
		if (m_pPlayer[i]->GetIsParalysis()) continue;			//既にマヒ中なら対象外.
		if (m_pPlayer[i]->GetIsMashBattle()) continue;			//連打対決中は下の②でまとめて処理する.

		for (auto& j : Ingredients) {
			if (j->GetCharStatus() != enCharStatus::Live) continue;	//生存していない具材は対象外.
			if (!m_pPlayer[i]->GetBSphere()->IsHit(*j->GetBSphere())) continue;	//接触していなければ対象外.

			//具材が煮えていないなら触れた瞬間にマヒさせる.
			if (!j->GetBoiledc()) {
				m_pPlayer[i]->OnTouchRawIngredient();
			}
		}
	}

	//--------------------------------------------------------------------
	//	②煮えた具材の取り合い判定.
	//	同じ具材に複数のプレイヤーが触れていたら連打対決を発生させる.
	//	※完全に同じフレームで触れていなくても対決に発展できるように、
	//	  最初に触れてからGRACE_FRAMESの間は他のプレイヤーの参加を待つ
	//	  「猶予期間」を設ける(PendingClaimで管理する).
	//--------------------------------------------------------------------
	for (auto& j : Ingredients) {
		if (j->GetCharStatus() != enCharStatus::Live) continue;	//生存していない具材は対象外.
		if (!j->GetBoiledc()) continue;						//生の具材はここでは扱わない(①でマヒ判定済み).
		if (j->GetCollecting()) continue;						//既に確保済み(通常回収中 or 連打対決中)の具材は対象外.

		//この具材に「今」触れている(取りに来ている)プレイヤーを集める.
		CPlayer* Touchers[Player_Max] = {};
		int TouchCount = 0;

		for (int i = 0; i < Player_Max; i++) {
			if (!m_pPlayer[i]) continue;
			if (!m_pPlayer[i]->GetConnected()) continue;
			if (!m_pPlayer[i]->GetIsCollecting()) continue;
			if (m_pPlayer[i]->GetIsParalysis()) continue;
			if (m_pPlayer[i]->GetIsMashBattle()) continue;		//既に別の対決に参加中なら重複して数えない.
			if (!m_pPlayer[i]->GetBSphere()->IsHit(*j->GetBSphere())) continue;

			Touchers[TouchCount] = m_pPlayer[i];
			TouchCount++;
		}

		//今フレーム誰も触れていないなら、進行中の猶予期間があれば打ち切って次の具材へ.
		if (TouchCount == 0) {
			PendingClaim* pClaim = FindPendingClaim(j);
			if (pClaim) {
				pClaim->pTarget = nullptr;	//誰も欲しがらなくなったので猶予枠を解放する.
			}
			continue;
		}

		//この具材の猶予枠を取得する(まだ無ければ新規に確保して猶予期間を開始する).
		PendingClaim* pClaim = FindPendingClaim(j);
		if (!pClaim) {
			pClaim = FindFreePendingClaimSlot();
			if (!pClaim) continue;	//空き枠が無ければ今回は諦める(次フレーム以降に再判定される).

			pClaim->pTarget = j;
			pClaim->TouchCount = 0;
			pClaim->GraceTimer = GRACE_FRAMES;
		}

		//今フレーム触れたプレイヤーのうち、まだ登録されていない人を猶予枠に追加する.
		for (int t = 0; t < TouchCount; t++) {
			bool AlreadyIn = false;
			for (int k = 0; k < pClaim->TouchCount; k++) {
				if (pClaim->Touchers[k] == Touchers[t]) {
					AlreadyIn = true;
					break;
				}
			}
			if (!AlreadyIn && pClaim->TouchCount < Player_Max) {
				pClaim->Touchers[pClaim->TouchCount] = Touchers[t];
				pClaim->TouchCount++;
			}
		}

		//猶予時間を減らし、時間切れになったらその時点の参加人数で決着をつける.
		pClaim->GraceTimer--;
		if (pClaim->GraceTimer <= 0) {
			ResolvePendingClaim(*pClaim);
			pClaim->pTarget = nullptr;	//猶予枠を解放して次の取り合いに備える.
		}
	}
}

//指定した具材の猶予枠を探す.
CCollisionManager::PendingClaim* CCollisionManager::FindPendingClaim(CIngredients* pTarget)
{
	for (auto& Claim : m_PendingClaims) {
		if (Claim.pTarget == pTarget) return &Claim;
	}
	return nullptr;
}

//空いている猶予枠を探す.
CCollisionManager::PendingClaim* CCollisionManager::FindFreePendingClaimSlot()
{
	for (auto& Claim : m_PendingClaims) {
		if (Claim.pTarget == nullptr) return &Claim;
	}
	return nullptr;
}

//猶予枠を確定させる.
void CCollisionManager::ResolvePendingClaim(PendingClaim& Claim)
{
	if (!Claim.pTarget) return;

	//猶予期間中に触れたのが1人だけなら、今まで通りそのまま獲得させる.
	if (Claim.TouchCount == 1) {
		Claim.Touchers[0]->IngredientsGetter(Claim.pTarget);	//具材を回収する.
		Claim.pTarget->IsCollecting();							//具材回収状態へ.
		return;
	}

	//2人以上いれば連打対決を開始する(最大2組まで同時進行可能).
	//空いているスロットが無い場合は今回は開始せず、具材は未確保のまま次フレーム以降で再判定させる.
	if (m_pButtonMashBattleUI && m_pButtonMashBattleUI->CanStartBattle()) {
		m_pButtonMashBattleUI->StartBattle(Claim.pTarget, Claim.Touchers, Claim.TouchCount);
		Claim.pTarget->IsCollecting();		//決着がつくまで他のプレイヤーが横取りできないようにする.
	}
}

bool CCollisionManager::IsHitForRay(const RAY& pRay,  CStaticMeshObjObject* obj, float* pDistance, D3DXVECTOR3* pIntersect, D3DXVECTOR3* pNormal)
{
	D3DXVECTOR3 vAxis;		//軸ベクトル.
	D3DXMATRIX	mRotationY;	//Y軸回転行列.

	//Y軸回転行列を作成.
	D3DXMatrixRotationY(&mRotationY, pRay.RotationY);
	//軸ベクトルを現在の回転状態に変換する.
	D3DXVec3TransformCoord(&vAxis, &pRay.Axis, &mRotationY);

	//レイの始点と終点.
	D3DXVECTOR3 StartPoint, EndPoint;
	StartPoint = pRay.Position;						//レイの始点を設定.
	EndPoint = pRay.Position + (vAxis * pRay.Length);	//レイの終点を設定.

	//レイを当てたいメッシュが移動している場合も、
	//対象のWorld行列の逆行列を用いれば正しくレイが当たる.
	D3DXMATRIX mWorld, mInverseWorld;

	//移動処理.
	D3DXMATRIX mTran;
	D3DXMatrixTranslation(
		&mTran,
		obj->GetPosition().x, obj->GetPosition().y, obj->GetPosition().z);

	//回転処理.
	//※この行列計算は「CStaticMesh::Render()関数」と同じにする必要あり.
	D3DXMATRIX mRot, mYaw, mPitch, mRoll;
	//Y軸回転行列生成.
	D3DXMatrixRotationY(&mYaw, obj->GetRotation().y);
	//X軸回転行列生成.
	D3DXMatrixRotationX(&mPitch, obj->GetRotation().x);
	//Z軸回転行列生成.
	D3DXMatrixRotationZ(&mRoll, obj->GetRotation().z);
	//回転行列を作成.
	mRot = mYaw * mPitch * mRoll;
	
	//拡縮処理.
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, obj->GetScale().x, obj->GetScale().y, obj->GetScale().z);

	//ワールド行列計算.
	//拡縮×回転×移動　※順番がとても大切！！.
	mWorld = mScale * mRot * mTran;

	//逆行列を求める.
	D3DXMatrixInverse(&mInverseWorld, nullptr, &mWorld);
	//レイの始点と終点に反映.
	D3DXVec3TransformCoord(&StartPoint, &StartPoint, &mInverseWorld);
	D3DXVec3TransformCoord(&EndPoint, &EndPoint, &mInverseWorld);

	//向きと長さ（大きさ）を求める.
	D3DXVECTOR3 vDirection = EndPoint - StartPoint;

	BOOL bHit = FALSE;		//命中フラグ.
	DWORD dwIndex = 0;		//インデックス番号.
	D3DXVECTOR3 Vertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;		//重心ヒット座標.

	//メッシュとレイの交差を調べる.
	D3DXIntersect(
		obj->GetMesh()->GetMeshForRay(),	//対象メッシュ.
		&StartPoint,				//レイの始点.
		&vDirection,				//レイの向きと長さ（大きさ）.
		&bHit,						//(out)判定結果.
		&dwIndex,	//(out)bHitがTRUE時にレイの始点に最も近くの面のインデックス値へのポインタ.
		&U, &V,						//(out)重心ヒット座標.
		pDistance,					//(out)メッシュとの距離.
		nullptr, nullptr);

	//無限に伸びるレイのどこかでメッシュが当たっていたら.
	if (bHit == TRUE)
	{
		//命中したとき.
		FindVerticesPoly(
			obj->GetMesh()->GetMeshForRay(), dwIndex, Vertex);

		//重心座標から交点を算出.
		//ローカル交点は v0 + U*(v1-v0) + V*(v2-v0) で求まる.
		*pIntersect
			= Vertex[0] + U * (Vertex[1] - Vertex[0]) + V * (Vertex[2] - Vertex[0]);

		//モデルデータが「拡縮」「回転」「移動」していれば行列が必要.
		D3DXVec3TransformCoord(pIntersect, pIntersect, &mWorld);

		//法線の出力要求があれば.
		if (pNormal != nullptr) {
			D3DXPLANE p;
			//その頂点(3点)から「平面の方程式」を得る.
			D3DXPlaneFromPoints(&p, &Vertex[0], &Vertex[1], &Vertex[2]);
			//「平面の方程式」の係数が法線の成分.
			*pNormal = D3DXVECTOR3(p.a, p.b, p.c);

			//法線は移動量算出に使用するので回転のみ処理する.
			D3DXVec3TransformCoord(pNormal, pNormal, &mRot);
		}

		//EndPointから見た距離で1.fより小さければ当たっている.
		if (*pDistance < 1.f) {
			return true;	//命中している.
		}
	}

	return false;	//外れている.
}

HRESULT CCollisionManager::FindVerticesPoly(LPD3DXMESH pMesh, DWORD dwPolyindex, D3DXVECTOR3* pVertices)
{
	//頂点ごとのバイト数を取得.
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	//頂点数を取得.
	DWORD dwVertexAmt = pMesh->GetNumVertices();
	//面数を取得.
	DWORD dwPolyAmt = pMesh->GetNumFaces();

	WORD* pwPoly = nullptr;

	//インデックスバッファをロック(読み込みモード).
	pMesh->LockIndexBuffer(
		D3DLOCK_READONLY,
		reinterpret_cast<VOID**>(&pwPoly));

	BYTE* pbVertices = nullptr;		//頂点(バイト型).
	FLOAT* pfVertices = nullptr;	//頂点(float型).
	LPDIRECT3DVERTEXBUFFER9 VB = nullptr;	//頂点バッファ.

	//頂点情報の取得.
	pMesh->GetVertexBuffer(&VB);

	//頂点バッファのロック.
	if (SUCCEEDED(
		VB->Lock(0, 0, reinterpret_cast<VOID**>(&pbVertices), 0)))
	{
		//ポリゴンの頂点１つ目を取得.
		pfVertices
			= reinterpret_cast<FLOAT*>(&pbVertices[dwStride * pwPoly[dwPolyindex * 3]]);
		pVertices[0].x = pfVertices[0];
		pVertices[0].y = pfVertices[1];
		pVertices[0].z = pfVertices[2];

		//ポリゴンの頂点２つ目を取得.
		pfVertices
			= reinterpret_cast<FLOAT*>(&pbVertices[dwStride * pwPoly[dwPolyindex * 3 + 1]]);
		pVertices[1].x = pfVertices[0];
		pVertices[1].y = pfVertices[1];
		pVertices[1].z = pfVertices[2];

		//ポリゴンの頂点３つ目を取得.
		pfVertices
			= reinterpret_cast<FLOAT*>(&pbVertices[dwStride * pwPoly[dwPolyindex * 3 + 2]]);
		pVertices[2].x = pfVertices[0];
		pVertices[2].y = pfVertices[1];
		pVertices[2].z = pfVertices[2];

		pMesh->UnlockIndexBuffer();	//ロック解除.
		VB->Unlock();	//ロック解除.
	}
	VB->Release();	//不要になったので解放.

	return S_OK;
}


