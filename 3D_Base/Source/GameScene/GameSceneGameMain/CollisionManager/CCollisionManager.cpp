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
	for (int i = 0; i < Player_Max; i++) {
		//nullptrなら次のプレイヤーへ.
		if (!m_pPlayer[i]) continue;
		if (!m_pIngredientsManager)continue;
		//プレイヤーにコントローラー接続が接続されていないのなら次のプレイヤーへ.
		if (!m_pPlayer[i]->GetConnected()) continue;
		{
			//プレイヤーが回収中でないなら次のプレイヤーへ.
			if (!m_pPlayer[i]->GetIsCollecting()) continue;
			//具材のサイズ読み込み.
			std::vector<CIngredients*>Ingredients = m_pIngredientsManager->GetIngredients();
			//具材の最大数分.
			for (auto& j: Ingredients) {
				//野菜が生きている.
				if (j->GetCharStatus() == enCharStatus::Live) {
					//プレイヤーと具材が接触する.
					if (m_pPlayer[i]->GetBSphere()->IsHit(*j->GetBSphere())) {

						//具材が煮えていないなら.
						if (!j->GetBoiledc()) {
							m_pPlayer[i]->OnTouchRawIngredient();	//マヒ状態にする.
						}
						//具材が煮えている.
						else {
							//回収状態でないなら次のプレイヤーへ.
							if (j->GetCollecting())continue;
							m_pPlayer[i]->IngredientsGetter(j);	//具材を回収する.
							j->IsCollecting();					//具材回収状態へ.
						}
					}
				}
			}
		}
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


