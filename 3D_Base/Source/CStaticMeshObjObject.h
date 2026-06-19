#pragma once

#include "CGameObject.h"
#include "CStaticMesh.h"
#include "CBoundingSphere.h"
class CStaticMeshObjObject
	: public CGameObject
{
public:
	CStaticMeshObjObject();
	~CStaticMeshObjObject();
	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

	//メッシュを接続する.
	void AttachMesh(CStaticObjMesh& pMesh) {
		m_pMesh = &pMesh;
	}
	//メッシュを切り離す.
	void DetachMesh() {
		m_pMesh = nullptr;
	}
	//バウンディングスフィア取得.
	CBoundingSphere* GetBSphere() const {
		return m_pBSphere;
	}
	//モデルに合わせたバウンディングスフィア作成のラッパー関数.
	HRESULT CreatSphereForMesh(const CStaticObjMesh& pMesh) {
		return m_pBSphere->CreatSphereForMesh(pMesh);
	}
	//バウンディングスフィアをオブジェクト位置に合わせる.
	//※モデルの原点が中心の場合を想定.
	void UpdateBSpherePos() {
		m_pBSphere->SetPosition(m_Position);
	}

protected:
	CStaticObjMesh* m_pMesh;
	CBoundingSphere* m_pBSphere;
};
