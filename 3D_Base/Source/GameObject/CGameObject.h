#pragma once

/**************************************************
*	ゲームオブジェクトクラス.
**/
class CGameObject
{
public:
	CGameObject();
	//継承を行う場合は、デストラクタにvirtualを付けて仮想関数にすること.
	virtual ~CGameObject();

	//virtual 型 関数名 = 0; 純粋仮想関数.
	//子クラスに処理をお任せするので、ここでは名前だけ宣言して定義は書かない.
	virtual void Update() = 0;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) = 0;

	//座標設定関数.
	void SetPosition( float x, float y, float z ){
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	};
	void SetPosition(const D3DXVECTOR3& pos) {
		m_Position = pos;
	}
	//座標取得関数.
	D3DXVECTOR3 GetPosition(){
		return m_Position;
	}

	//回転設定関数.
	void SetRotation( float x, float y, float z ){
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	};
	//回転取得関数.
	D3DXVECTOR3 GetRotation(){
		return m_Rotation;
	}

	//拡縮設定関数.
	void SetScale( float x, float y, float z ){
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}
	void SetScale(float xyz)
	{
		m_Scale = D3DXVECTOR3(xyz,xyz,xyz);
	}
	//拡縮取得関数.
	D3DXVECTOR3 GetScale(){
		return m_Scale;
	}


protected://protectedは子クラスのみアクセス可能.
	D3DXVECTOR3	m_Position;
	D3DXVECTOR3	m_Rotation;
	D3DXVECTOR3	m_Scale;
};